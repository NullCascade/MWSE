local config = json.loadfile("config\\openmw_compat_harness")
if not config or config.enabled ~= true then
	return
end

local socket = require("socket")
local PROTOCOL_VERSION = 1
local HEARTBEAT_SECONDS = 2
local commandPath = config.runtimeDirectory .. "\\command.json"
local eventsPath = config.runtimeDirectory .. "\\events.jsonl"
local readyPath = config.runtimeDirectory .. "\\ready.json"
local sequence = 0
local loading = false
local lastHeartbeat = 0
local pendingEventRequests = {}
local pendingScreenshots = {}
local shutdownRequested = false
local shutdownPosted = false

local function now()
	return socket.gettime()
end

local function timestamp()
	return os.date("!%Y-%m-%dT%H:%M:%SZ")
end

local function readRecentLogLines(limit)
	local file = io.open("MWSE.log", "r")
	if not file then
		return {}
	end

	local lines = {}
	for line in file:lines() do
		lines[#lines + 1] = line
		if #lines > limit then
			table.remove(lines, 1)
		end
	end
	file:close()
	return lines
end

local function gameState()
	local onMainMenu = false
	local paused = false
	pcall(function() onMainMenu = tes3.onMainMenu() end)
	pcall(function() paused = tes3.menuMode() end)

	local player
	pcall(function() player = tes3.player end)
	local playerValid = player ~= nil
	local cell
	if playerValid then
		pcall(function()
			local playerCell = player.cell
			if playerCell then
				if playerCell.isInterior then
					cell = { interior = true, name = playerCell.name }
				else
					cell = { interior = false, gridX = playerCell.gridX, gridY = playerCell.gridY }
				end
			end
		end)
	end

	local stateName = "mainMenu"
	if loading then
		stateName = "loading"
	elseif playerValid and not onMainMenu then
		stateName = "inGame"
	end

	return {
		name = stateName,
		mainMenu = onMainMenu,
		loading = loading,
		inGame = stateName == "inGame",
		paused = paused,
		currentCell = cell,
		playerValid = playerValid,
	}
end

local function appendMessage(messageType, fields)
	sequence = sequence + 1
	local message = fields or {}
	message.protocolVersion = PROTOCOL_VERSION
	message.runId = config.runId
	message.sequence = sequence
	message.timestamp = timestamp()
	message.type = messageType

	local file = assert(io.open(eventsPath, "a"))
	file:write(json.encode(message), "\n")
	file:flush()
	file:close()
	return message
end

local function writeAtomicJson(path, value)
	local temporaryPath = path .. ".tmp"
	local file = assert(io.open(temporaryPath, "w"))
	file:write(json.encode(value, { indent = true }))
	file:flush()
	file:close()
	os.remove(path)
	assert(os.rename(temporaryPath, path))
end

local function diagnosticSnapshot()
	return {
		gameState = gameState(),
		recentLogLines = readRecentLogLines(30),
	}
end

local function respond(request, ok, result, errorData)
	appendMessage("response", {
		requestId = request.requestId,
		command = request.command,
		ok = ok,
		result = result,
		error = errorData,
		gameState = gameState(),
	})
end

local function emitAssertion(request, name, passed, actual, expected, details)
	appendMessage("assertion", {
		requestId = request.requestId,
		name = name,
		passed = passed,
		actual = actual,
		expected = expected,
		details = details,
		gameState = gameState(),
	})
end

local probes = {}

function probes.mwseInitialization(request)
	local passed = type(mwse.buildNumber) == "number" and mwse.buildNumber > 0
	emitAssertion(request, "mwse-initialized", passed, mwse.buildNumber, "positive build number")
	return { buildNumber = mwse.buildNumber, luaVersion = _VERSION }
end

local function decodeOpenMWHostReport()
	assert(mwse.openmwCompatibility, "MWSE OpenMW compatibility bridge is unavailable.")
	local encoded = mwse.openmwCompatibility.getReport()
	assert(type(encoded) == "string" and #encoded > 0, "OpenMW host returned no report.")
	return json.decode(encoded)
end

function probes.openMWLuaHostReport(request)
	local report = decodeOpenMWHostReport()
	local bridge = report.bridge or {}
	local containers = report.containers or {}
	local handlers = report.handlers or {}
	local instances = containers.instances or {}

	emitAssertion(request, "openmw-support-dll-loaded", mwse.openmwCompatibility.isLoaded() == true,
		mwse.openmwCompatibility.isLoaded(), true)
	emitAssertion(request, "openmw-bridge-abi-accepted", bridge.abiAccepted == true and bridge.abiVersion == 2,
		bridge, { abiAccepted = true, abiVersion = 2 })
	emitAssertion(request, "openmw-runtime-independent", bridge.runtimeOwnedAllocator == true
		and bridge.importsMwseLua == false and bridge.runtimeVersion ~= _VERSION,
		{ host = bridge.runtimeVersion, mwse = _VERSION, allocator = bridge.runtimeOwnedAllocator,
			importsMwseLua = bridge.importsMwseLua }, "independent LuaJIT runtime")
	emitAssertion(request, "openmw-container-counts", containers.menuDefinitions == 1
		and containers.globalDefinitions == 1 and containers.playerDefinitions == 1,
		{ menu = containers.menuDefinitions, global = containers.globalDefinitions, player = containers.playerDefinitions },
		{ menu = 1, global = 1, player = 1 })

	local environments = {}
	local interfaces = {}
	local isolated = #instances == 3
	for _, instance in ipairs(instances) do
		isolated = isolated and environments[instance.environmentId] == nil and instance.mwseGlobalVisible == false
		environments[instance.environmentId] = true
		interfaces[instance.interfaceName] = instance.hasInterface
	end
	emitAssertion(request, "openmw-distinct-script-environments", isolated, instances,
		"three unique environments with no MWSE global")
	emitAssertion(request, "openmw-interfaces-registered", interfaces.MenuFixture == true
		and interfaces.GlobalFixture == true and interfaces.PlayerFixture == true,
		interfaces, { MenuFixture = true, GlobalFixture = true, PlayerFixture = true })

	local engine = handlers.engineHandlerOrder or {}
	local directOrder = #engine >= 3 and string.find(engine[1], "MENU:menu.lua:onUpdate", 1, true)
		and string.find(engine[2], "GLOBAL:global.lua:onUpdate", 1, true)
		and string.find(engine[3], "PLAYER:player.lua:onUpdate", 1, true)
	emitAssertion(request, "openmw-engine-handler-direct-order", directOrder ~= nil,
		{ engine[1], engine[2], engine[3] }, { "MENU", "GLOBAL", "PLAYER" })

	local events = handlers.eventHandlerOrder or {}
	local reverseOrder = #events >= 3 and string.find(events[1], "PLAYER:player.lua:Milestone3Event", 1, true)
		and string.find(events[2], "GLOBAL:global.lua:Milestone3Event", 1, true)
		and string.find(events[3], "MENU:menu.lua:Milestone3Event", 1, true)
	emitAssertion(request, "openmw-event-handler-reverse-order", reverseOrder ~= nil,
		{ events[1], events[2], events[3] }, { "PLAYER", "GLOBAL", "MENU" })
	emitAssertion(request, "openmw-delayed-event-delivered", #(handlers.delayedDeliveries or {}) >= 1,
		handlers.delayedDeliveries, "one or more deterministic delayed deliveries")
	local failureIsolated = #(handlers.diagnostics or {}) >= 1 and #events >= 3
	emitAssertion(request, "openmw-handler-failure-isolated", failureIsolated,
		{ diagnostics = handlers.diagnostics, handlersAfterFailure = events[3] },
		"structured failure plus continued handler delivery")
	emitAssertion(request, "mwse-lua-functional-with-openmw-host", type(mwse.buildNumber) == "number"
		and mwse.buildNumber > 0 and _VERSION == "Lua 5.1-DW",
		{ buildNumber = mwse.buildNumber, luaVersion = _VERSION }, "normal MWSE Lua state")
	return report
end

function probes.openMWLuaFoundationReport(request)
	local report = decodeOpenMWHostReport()
	local bridge = report.bridge or {}
	local containers = report.containers or {}
	local foundation = report.foundation or {}
	local probesReport = foundation.probes or {}
	local required = {
		"util-vector-color",
		"interfaces-lookup-readonly",
		"async-registered-simulation",
		"async-unsavable-game",
		"async-callback-callable",
		"storage-global-subscription",
		"storage-player-subscription",
		"storage-context-permissions",
		"storage-menu-player-scope",
		"core-time-content-gmst",
		"core-delayed-global-event",
		"self-player-context",
		"self-context-rejected-global",
		"self-context-rejected-menu",
	}
	local missing = {}
	for _, name in ipairs(required) do
		if probesReport[name] ~= true then
			missing[#missing + 1] = name
		end
	end
	emitAssertion(request, "openmw-foundation-bridge-v2", bridge.abiAccepted == true and bridge.abiVersion == 2
		and bridge.bridgeVersion == 2 and bridge.gmstCallbackAvailable == true
		and bridge.contentFilesCallbackAvailable == true,
		bridge, { abiVersion = 2, bridgeVersion = 2, gmstCallbackAvailable = true, contentFilesCallbackAvailable = true })
	emitAssertion(request, "openmw-foundation-container-counts", containers.menuDefinitions == 1
		and containers.globalDefinitions == 2 and containers.playerDefinitions == 1,
		{ menu = containers.menuDefinitions, global = containers.globalDefinitions, player = containers.playerDefinitions },
		{ menu = 1, global = 2, player = 1 })
	emitAssertion(request, "openmw-foundation-package-probes", #missing == 0,
		{ missing = missing, probes = probesReport }, "all required foundation probes true")
	emitAssertion(request, "openmw-foundation-timers", foundation.timers and foundation.timers.scheduled == 2
		and foundation.timers.fired == 2 and foundation.timers.pending == 0,
		foundation.timers, { scheduled = 2, fired = 2, pending = 0 })
	emitAssertion(request, "openmw-foundation-storage", foundation.storage and foundation.storage.globalSections == 1
		and foundation.storage.playerSections == 2 and foundation.storage.notifications == 2,
		foundation.storage, { globalSections = 1, playerSections = 2, notifications = 2 })
	emitAssertion(request, "openmw-foundation-interfaces", foundation.interfaces and foundation.interfaces.lookups >= 1,
		foundation.interfaces, { lookups = ">=1" })
	emitAssertion(request, "openmw-foundation-runtime-independent", bridge.runtimeOwnedAllocator == true
		and bridge.importsMwseLua == false and bridge.runtimeVersion ~= _VERSION,
		{ host = bridge.runtimeVersion, mwse = _VERSION, allocator = bridge.runtimeOwnedAllocator,
			importsMwseLua = bridge.importsMwseLua }, "independent LuaJIT runtime")
	emitAssertion(request, "mwse-lua-functional-with-foundation-host", type(mwse.buildNumber) == "number"
		and mwse.buildNumber > 0 and _VERSION == "Lua 5.1-DW",
		{ buildNumber = mwse.buildNumber, luaVersion = _VERSION }, "normal MWSE Lua state")
	return report
end

function probes.reloadOpenMWLuaHost(request)
	local before = decodeOpenMWHostReport()
	local reloaded = mwse.openmwCompatibility.reload()
	local after = decodeOpenMWHostReport()
	local passed = reloaded == true and after.bridge.runtimeGeneration == before.bridge.runtimeGeneration + 1
		and after.reload.reloadCount == before.reload.reloadCount + 1
	emitAssertion(request, "openmw-explicit-reload", passed,
		{ reloaded = reloaded, before = before.reload, after = after.reload },
		"runtime generation and reload count incremented")
	emitAssertion(request, "mwse-lua-functional-after-openmw-reload", type(mwse.buildNumber) == "number"
		and mwse.buildNumber > 0 and _VERSION == "Lua 5.1-DW",
		{ buildNumber = mwse.buildNumber, luaVersion = _VERSION }, "normal MWSE Lua state")
	return after
end

function probes.shutdownOpenMWLuaHost(request)
	assert(mwse.openmwCompatibility, "MWSE OpenMW compatibility bridge is unavailable.")
	local stopped = mwse.openmwCompatibility.shutdown()
	local report = decodeOpenMWHostReport()
	local passed = stopped == true and report.reload.state == 6 and report.reload.cleanShutdown == true
	emitAssertion(request, "openmw-clean-runtime-shutdown", passed,
		{ stopped = stopped, reload = report.reload }, { state = 6, cleanShutdown = true })
	emitAssertion(request, "mwse-lua-functional-after-openmw-shutdown", type(mwse.buildNumber) == "number"
		and mwse.buildNumber > 0 and _VERSION == "Lua 5.1-DW",
		{ buildNumber = mwse.buildNumber, luaVersion = _VERSION }, "normal MWSE Lua state")
	return report
end

function probes.playerAndCell(request)
	local state = gameState()
	local passed = state.playerValid and state.currentCell ~= nil
	emitAssertion(request, "player-and-cell-valid", passed, state, { playerValid = true, currentCell = "non-null" })
	return state
end

function probes.writeReferencePersistentValue(request, arguments)
	assert(tes3.player, "A loaded player is required.")
	local key = assert(arguments.key, "Probe argument 'key' is required.")
	local value = arguments.value
	tes3.player.data.openmwCompatHarness = tes3.player.data.openmwCompatHarness or {}
	tes3.player.data.openmwCompatHarness[key] = value
	emitAssertion(request, "reference-persistent-write", tes3.player.data.openmwCompatHarness[key] == value, tes3.player.data.openmwCompatHarness[key], value)
	return { key = key, value = value }
end

function probes.readReferencePersistentValue(request, arguments)
	assert(tes3.player, "A loaded player is required.")
	local key = assert(arguments.key, "Probe argument 'key' is required.")
	local values = tes3.player.data.openmwCompatHarness or {}
	local value = values[key]
	if arguments.expected ~= nil then
		emitAssertion(request, "reference-persistent-read", value == arguments.expected, value, arguments.expected)
	end
	return { key = key, value = value }
end

function probes.saveGame(request, arguments)
	local file = assert(arguments.file, "Probe argument 'file' is required.")
	local name = arguments.name or file
	local saved = tes3.saveGame({ file = file, name = name })
	emitAssertion(request, "save-request-accepted", saved == true, saved, true)
	return { file = file, name = name, saved = saved }
end

function probes.openMWAddonState(request, arguments)
	local expectedAlias = assert(arguments.expectedAlias, "Probe argument 'expectedAlias' is required.")
	local expectedOrdinaryFiles = arguments.expectedOrdinaryFiles or {}
	local activeFiles = {}
	local activeByName = {}
	for _, gameFile in ipairs(tes3.dataHandler.nonDynamicData.activeMods) do
		activeFiles[#activeFiles + 1] = gameFile.filename
		activeByName[string.lower(gameFile.filename)] = true
	end

	local aliasActive = activeByName[string.lower(expectedAlias)] == true
	emitAssertion(request, "openmw-addon-native-alias-active", aliasActive, activeFiles, expectedAlias)
	local ordinaryUnchanged = true
	local missingOrdinary = {}
	for _, filename in ipairs(expectedOrdinaryFiles) do
		if not activeByName[string.lower(filename)] then
			ordinaryUnchanged = false
			missingOrdinary[#missingOrdinary + 1] = filename
		end
	end
	emitAssertion(request, "ordinary-content-files-unchanged", ordinaryUnchanged, missingOrdinary, {})

	local majorMultiplier = assert(tes3.findGMST("iLevelupMajorMult"), "iLevelupMajorMult was not found.")
	local majorValue = majorMultiplier.value
	emitAssertion(request, "ncg-gmst-live-value", majorValue == 0, majorValue, 0)

	local block = assert(tes3.getSkill(tes3.skill.block), "Block skill record was not found.")
	local description = block.description
	local expectedDescriptionText = "Develops your agility and endurance a good amount each"
	local skillChanged = type(description) == "string" and string.find(description, expectedDescriptionText, 1, true) ~= nil
	emitAssertion(request, "ncg-skill-description-live-value", skillChanged, description, expectedDescriptionText)

	return {
		activeFiles = activeFiles,
		aliasActive = aliasActive,
		ordinaryFilesUnchanged = ordinaryUnchanged,
		gmst = { id = majorMultiplier.id, value = majorValue },
		skill = { id = block.id, description = description },
	}
end

local allowedEvents = {
	cellChanged = tes3.event.cellChanged,
	initialized = tes3.event.initialized,
	loaded = tes3.event.loaded,
	saved = tes3.event.saved,
}

local function eventResult(eventName, eventData)
	local result = { event = eventName }
	if eventData then
		result.filename = eventData.filename
		result.newGame = eventData.newGame
	end
	return result
end

local function observeEvent(eventName, eventData)
	for requestId, pending in pairs(pendingEventRequests) do
		if pending.event == eventName then
			pendingEventRequests[requestId] = nil
			emitAssertion(pending.request, "native-event-" .. eventName, true, eventName, eventName)
			respond(pending.request, true, eventResult(eventName, eventData))
		end
	end
end

event.register(tes3.event.load, function()
	loading = true
end)

event.register(tes3.event.loaded, function(e)
	loading = false
	observeEvent("loaded", e)
end)

event.register(tes3.event.saved, function(e)
	observeEvent("saved", e)
end)

event.register(tes3.event.cellChanged, function(e)
	observeEvent("cellChanged", e)
end)

local commands = {}

function commands.ping(request)
	respond(request, true, { pong = true, gameTime = now() })
end

function commands.getState(request)
	respond(request, true, gameState())
end

function commands.evalNamedProbe(request)
	local arguments = request.arguments or {}
	local probeName = assert(arguments.name, "Probe name is required.")
	local probe = assert(probes[probeName], "Unknown named probe: " .. tostring(probeName))
	respond(request, true, { name = probeName, value = probe(request, arguments.arguments or {}) })
end

function commands.loadGame(request)
	local filename = assert(request.arguments and request.arguments.filename, "Save filename is required.")
	loading = true
	tes3.loadGame(filename)
	-- The native binding does not consistently return its documented boolean.
	-- Completion is authoritatively reported through the loaded event.
	respond(request, true, { issued = true, filename = filename })
end

function commands.newGame(request)
	loading = true
	tes3.newGame()
	respond(request, true, { accepted = true })
end

function commands.teleport(request)
	assert(tes3.player, "A loaded player is required.")
	local arguments = request.arguments or {}
	local position = arguments.position or { 0, 0, 0 }
	local executed = tes3.positionCell({
		reference = tes3.player,
		cell = arguments.cell,
		position = position,
		orientation = arguments.orientation,
		forceCellChange = arguments.forceCellChange ~= false,
		suppressFader = true,
		teleportCompanions = false,
	})
	respond(request, executed == true, { executed = executed, cell = arguments.cell, position = position })
end

function commands.waitForEvent(request)
	local eventName = assert(request.arguments and request.arguments.event, "Event name is required.")
	assert(allowedEvents[eventName], "Unsupported event name: " .. tostring(eventName))
	pendingEventRequests[request.requestId] = {
		request = request,
		event = eventName,
		expiresAt = now() + ((request.timeoutMs or 10000) / 1000),
	}
end

function commands.screenshot(request)
	local filename = (request.arguments and request.arguments.filename) or (request.requestId .. ".png")
	assert(filename:match("^[%w_.-]+$"), "Screenshot filename contains unsupported characters.")
	local path = "screenshots\\" .. filename
	mge.saveScreenshot({ path = path, captureWithUI = request.arguments and request.arguments.captureWithUI == true })
	pendingScreenshots[request.requestId] = {
		request = request,
		path = path,
		expiresAt = now() + ((request.timeoutMs or 10000) / 1000),
	}
end

function commands.shutdown(request)
	respond(request, true, { accepted = true })
	appendMessage("shutdown", { requestId = request.requestId, reason = "requested", gameState = gameState() })
	shutdownRequested = true
end

local function processCommand()
	local file = io.open(commandPath, "r")
	if not file then
		return
	end
	local contents = file:read("*all")
	file:close()
	os.remove(commandPath)

	local request, _, decodeError = json.decode(contents)
	if not request then
		appendMessage("fatal", { error = { code = "invalid_json", message = tostring(decodeError) }, diagnostic = diagnosticSnapshot() })
		return
	end

	if request.protocolVersion ~= PROTOCOL_VERSION or request.runId ~= config.runId or type(request.requestId) ~= "string" then
		respond(request, false, nil, { code = "invalid_envelope", message = "Protocol version, run ID, or request ID is invalid." })
		return
	end

	local handler = commands[request.command]
	if not handler then
		respond(request, false, nil, { code = "unknown_command", message = "Unknown command: " .. tostring(request.command) })
		return
	end

	appendMessage("log", { level = "debug", requestId = request.requestId, message = "Executing " .. request.command, gameState = gameState() })
	local ok, errorMessage = xpcall(function() handler(request) end, debug.traceback)
	if not ok then
		respond(request, false, nil, {
			code = "command_failed",
			message = tostring(errorMessage),
			traceback = tostring(errorMessage),
			diagnostic = diagnosticSnapshot(),
		})
	end
end

local function poll()
	processCommand()

	local currentTime = now()
	if currentTime - lastHeartbeat >= HEARTBEAT_SECONDS then
		lastHeartbeat = currentTime
		appendMessage("heartbeat", { gameState = gameState() })
	end

	for requestId, pending in pairs(pendingEventRequests) do
		if currentTime >= pending.expiresAt then
			pendingEventRequests[requestId] = nil
			respond(pending.request, false, nil, {
				code = "request_timeout",
				message = "Timed out waiting for event " .. pending.event,
				diagnostic = diagnosticSnapshot(),
			})
		end
	end

	for requestId, pending in pairs(pendingScreenshots) do
		if lfs.fileexists(pending.path) then
			pendingScreenshots[requestId] = nil
			respond(pending.request, true, { path = pending.path, saved = true })
		elseif currentTime >= pending.expiresAt then
			pendingScreenshots[requestId] = nil
			respond(pending.request, false, nil, {
				code = "screenshot_timeout",
				message = "MGE did not materialize the queued screenshot.",
				diagnostic = diagnosticSnapshot(),
			})
		end
	end

	if shutdownRequested and not shutdownPosted then
		shutdownPosted = true
		local ok, errorMessage = pcall(function()
			local ffi = require("ffi")
			ffi.cdef("int __stdcall PostMessageA(void* hWnd, unsigned int Msg, uintptr_t wParam, intptr_t lParam);")
			local posted = ffi.C.PostMessageA(ffi.cast("void*", tes3.game.windowHandle), 0x0010, 0, 0)
			assert(posted ~= 0, "PostMessageA(WM_CLOSE) failed.")
		end)
		if not ok then
			appendMessage("log", { level = "warning", message = "In-process WM_CLOSE was unavailable; launcher close is required: " .. tostring(errorMessage), gameState = gameState() })
		end
	end
end

event.register(tes3.event.enterFrame, poll, { priority = -1000 })

local ready = {
	protocolVersion = PROTOCOL_VERSION,
	runId = config.runId,
	timestamp = timestamp(),
	type = "ready",
	mwseBuildNumber = mwse.buildNumber,
	gameState = gameState(),
}
writeAtomicJson(readyPath, ready)
appendMessage("ready", ready)
appendMessage("log", { level = "info", message = "OpenMW compatibility harness enabled for explicit test run.", gameState = gameState() })
