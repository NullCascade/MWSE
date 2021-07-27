--
-- ReadTheDocs generator for MWSE-lua definitions.
--

local lfs = require("lfs")
local common = require("builders.common")
common.defaultNoDescriptionText = "No description available."

common.log("Starting build of ReadTheDocs rst files...")

-- Recreate output folders.
local docsSourceFolder = lfs.join(common.pathAutocomplete, "..\\docs\\source")
local docsLuaSourceFolder = lfs.join(docsSourceFolder, "lua")

local rstHeaders = {
	"====================================================================================================",
	"----------------------------------------------------------------------------------------------------",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
}


--
-- Handle link caching to optimize RTD build times.
--

local cachingLinks = -1
local writtenLinks = {}
local originalFileWrite = nil

local originalIOOpen = io.open

-- Handle nested caching begin/stop
local function beginCachingLinks()
	cachingLinks = cachingLinks + 1
end

local function isCachingLinks()
    return cachingLinks > -1
end

local function stopCachingLinks()
	cachingLinks = cachingLinks - 1
end

local function isLinkCached(file, link)
	return writtenLinks[file] and writtenLinks[file][link] == true
end

local function cachedWrite(self, str, ...)
	if (isCachingLinks()) then
		writtenLinks[self] = writtenLinks[self] or {}
		for capture in string.gmatch(str, "`.-`_") do
			writtenLinks[self][string.sub(capture, 2, -3)] = true
		end
	end

	return originalFileWrite(self, str)
end

function io.open(...)
	local file = originalIOOpen(...)
	if (file) then
		if (not originalFileWrite) then
			local mt = getmetatable(file)
			originalFileWrite = mt.write
			mt.write = cachedWrite
		end
		writtenLinks[file] = writtenLinks[file] or {}
	end
	return file
end


--
-- Compile data
--

--- @type table<string, packageLib>
local libraries = {}
common.compilePath(lfs.join(common.pathDefinitions, "global"), libraries, "lib")

--- @type table<string, packageClass>
local classes = {}
common.compilePath(lfs.join(common.pathDefinitions, "namedTypes"), classes, "class")

--- @type table<string, package>
local events = {}
-- common.compilePath(lfs.join(common.pathDefinitions, "events\\standard"), events)

local typeLinks = {
	["bool"] = "lua/type/boolean",
	["boolean"] = "lua/type/boolean",
	["function"] = "lua/type/function",
	["nil"] = "lua/type/nil",
	["number"] = "lua/type/number",
	["string"] = "lua/type/string",
	["table"] = "lua/type/table",
}
for key, _ in pairs(classes) do
	typeLinks[key] = common.urlJoin("lua/type", key)
end

--
-- Build output
--

local function breakoutMultipleTypes(str)
	return "`" .. table.concat(string.split(str, "|"), "`_, `") .. "`_"
end

local function getPackageDescription(package)
	local prefix = ""
	if (package.type == "value" and package.valuetype) then
		prefix = prefix .. breakoutMultipleTypes(package.valuetype) .. ". "
	end
	if (package.readOnly) then
		prefix = prefix .. "Read-only. "
	end
	return prefix .. (package.description or common.defaultNoDescriptionText)
end

local function getArgumentDescription(argument)
	local prefix = ""
	if (argument.default) then
		prefix = prefix .. string.format("Default: ``%s``. ", tostring(argument.default))
	elseif (argument.optional) then
		prefix = prefix .. "Optional. "
	end
	return prefix .. (argument.description or common.defaultNoDescriptionText)
end

--- comment
--- @param package package
--- @return number
local function getParentCount(package)
	local count = 0
	while (package.parent) do
		count = count + 1
		package = package.parent
	end
	return count
end

--- comment
--- @param package table
--- @param field any
--- @param results any
--- @return table
local function getPackageComponentsDictionary(package, field, results)
	local results = results or {}

	local onThis = package[field]
	if (onThis) then
		for k, v in pairs(table.values(onThis)) do
			if (results[k] == nil) then
				results[k] = v
			end
		end
	end

	if (package.inherits and classes[package.inherits]) then
		local inheritClass = classes[package.inherits]
		return getPackageComponentsDictionary(inheritClass, field, results)
	end

	return results
end

--- comment
--- @param package table
--- @param field any
--- @param results any
--- @return table
local function getPackageComponentsArray(package, field, results)
	local results = results or {}

	local onThis = package[field]
	if (onThis) then
		for _, v in ipairs(table.values(onThis)) do
			if (results[v.key] == nil) then
				results[v.key] = v
			end
		end
	end

	if (package.inherits and classes[package.inherits]) then
		local inheritClass = classes[package.inherits]
		return getPackageComponentsArray(inheritClass, field, results)
	end

	return results
end

local function sortPackagesByKey(A, B)
	return A.key:lower() < B.key:lower()
end

--- comment
--- @param package packageClass
--- @param outDir string
local function build(package, outDir)
	-- Load our base package.
	common.log("Building %s: %s ...", package.type, package.key)

	-- Get the package.
	local outPath = lfs.join(outDir, package.key .. ".rst")
	local file = assert(io.open(outPath, "w+"))
	beginCachingLinks()

	--
	file:write(string.format("%s\n%s\n\n", package.key, rstHeaders[1]))
	file:write(string.format("%s\n\n", getPackageDescription(package)))

	if (package.type == "function" or package.type == "method") then
		-- Show return values.
		local returns = common.getConsistentReturnValues(package)
		if (returns) then
			file:write(string.format("Returns\n%s\n\n", rstHeaders[2]))
			if (#returns > 1) then
				file:write("The function has more than one return value.\n\n")
			end
			for _, ret in pairs(returns) do
				file:write(string.format("%s (%s)\n    %s\n\n", ret.name, breakoutMultipleTypes(ret.type or "any"), getArgumentDescription(ret)))
			end
			-- file:write("\n") -- TODO: Re-add this once output is matched.
		end

		-- Show function arguments.
		if (package.arguments and #package.arguments > 0) then
			file:write(string.format("Parameters\n%s\n\n", rstHeaders[2]))
			if (package.arguments[1].tableParams) then
				file:write("Accepts parameters through a table with the given keys:\n\n")
			else
				file:write("Accepts parameters in the following order:\n\n")
			end
			for _, arg in pairs(package.arguments) do
				file:write(string.format("%s (%s)\n    %s\n\n", arg.name or "unnamed", breakoutMultipleTypes(arg.type or "any"), getArgumentDescription(arg)))
			end
			-- file:write("\n") -- TODO: Re-add this once output is matched.
		end
	elseif (package.type == "lib" or package.type == "class") then
		if (package.type == "class" and package.inherits) then
			local types = {}
			local inherits = classes[package.inherits]
			while (inherits) do
				table.insert(types, inherits.key)
				inherits = classes[inherits.inherits]
			end
			file:write(string.format("This type inherits from the following parent types: %s\n\n", breakoutMultipleTypes(table.concat(types, "|"))))
		end

		--
		local values = table.values(getPackageComponentsArray(package, "values"), sortPackagesByKey)
		if (#values > 0) then
			file:write(string.format("Values\n%s\n\n", rstHeaders[2]))
			file:write(".. toctree::\n    :maxdepth: 1\n\n")
			for _, fn in ipairs(values) do
				file:write(string.format("    %s/%s\n", package.key, fn.key))
			end
			file:write("\n")
		end

		--
		local functions = table.values(getPackageComponentsArray(package, "functions"), sortPackagesByKey)
		if (#functions > 0) then
			file:write(string.format("Functions\n%s\n\n", rstHeaders[2]))
			file:write(".. toctree::\n")
			file:write("    :maxdepth: 1\n\n")
			for _, fn in ipairs(functions) do
				file:write(string.format("    %s/%s\n", package.key, fn.key))
			end
			file:write("\n")
		end

		--
		local methods = table.values(getPackageComponentsArray(package, "methods"), sortPackagesByKey)
		if (#methods > 0) then
			file:write(string.format("Methods\n%s\n\n", rstHeaders[2]))
			file:write(".. toctree::\n")
			file:write("    :maxdepth: 1\n\n")
			for _, fn in ipairs(methods) do
				file:write(string.format("    %s/%s\n", package.key, fn.key))
			end
			file:write("\n")
		end
	end

	-- Write out the links we've written.
	stopCachingLinks()
	local linkPositionReset = string.rep("../", getParentCount(package) + 2)
	for _, link in ipairs(table.keys(writtenLinks[file], true)) do
		file:write(string.format(".. _`%s`: %slua/type/%s.html\n", link, linkPositionReset, link))
	end

	-- Close up shop.
	writtenLinks[file] = nil
	file:close()

	-- Write out children in a subdirectory.
	local children = table.values(getPackageComponentsDictionary(package, "children"), sortPackagesByKey)
	if (#children > 0) then
		lfs.mkdir(lfs.join(outDir, package.key))
		for _, child in ipairs(children) do
			build(child, lfs.join(outDir, package.key))
		end
	end
end

local function buildBuilder(collection, outdir)
	lfs.remakedir(lfs.join(docsLuaSourceFolder, outdir))
	for _, package in pairs(collection) do
		build(package, lfs.join(docsLuaSourceFolder, outdir))
	end
end
buildBuilder(libraries, "api")
buildBuilder(classes, "type")
-- buildBuilder(libraries, "events")
