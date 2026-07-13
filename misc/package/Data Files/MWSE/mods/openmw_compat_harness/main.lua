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
