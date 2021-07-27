--
-- EmmyLua meta files generator for MWSE-lua definitions.
--

local lfs = require("lfs")
local common = require("builders.common")

common.log("Starting build of EmmyLua meta files...")

-- Recreate meta folder.
local metaFolder = lfs.join(common.pathAutocomplete, "..\\misc\\package\\Data Files\\MWSE\\core\\meta")
lfs.remakedir(lfs.join(metaFolder, "lib"))
lfs.remakedir(lfs.join(metaFolder, "class"))

-- Base containers to hold our compiled data.
local libraries = {}
local classes = {}


--
-- Utility functions.
--

common.log("Definitions folder: %s", common.pathDefinitions)

local function getPackageLink(package)
	local tokens = { common.urlBase, package.key .. ".html" }
	local up = package.parent
	while (up) do
		table.insert(tokens, 2, up.key)
		if (up.parent == nil) then
			if (up.type == "lib") then
				table.insert(tokens, 2, "api")
			elseif (up.type == "class") then
				table.insert(tokens, 2, "type")
			end
		end
		up = up.parent
	end
	return table.concat(tokens, "/")
end

local function writeExamples(package, file)
	if (package.examples) then
		file:write(string.format("---\n--- [Examples available in online documentation](%s).\n", getPackageLink(package)))
	end
end

local function formatLineBreaks(str)
	return string.gsub(str, "\n", "\n--- ")
end

local function formatDescription(description)
	return "--- " .. formatLineBreaks(description)
end

local function getAllPossibleVariationsOfType(type)
	if (not type) then
		return nil
	end

	local types = {}
	for _, t in ipairs(string.split(type, "|")) do
		local class = classes[t]
		if (class) then
			table.insert(types, class.allDescendentKeys or t)
		else
			table.insert(types, t)
		end
	end

	return table.concat(types, "|")
end

local function getParamNames(package)
	local params = {}
	for _, param in ipairs(package.arguments or {}) do
		if (param.type == "variadic") then
			table.insert(params, "...")
		else
			table.insert(params, param.name or "unknown")
		end
	end
	return params
end

local function writeFunction(package, file, namespaceOverride)
	file:write(formatDescription(package.description or common.defaultNoDescriptionText) .. "\n")
	writeExamples(package, file)

	for _, argument in ipairs(package.arguments or {}) do
		local type = argument.type
		local description = argument.description or common.defaultNoDescriptionText
		if (argument.tableParams) then
			type = package.namespace .. ".params"
			description = "This table accepts the following values:"
			for _, tableArgument in ipairs(argument.tableParams) do
				description = description .. string.format("\n\n``%s``: %s — %s", tableArgument.name or "unknown", getAllPossibleVariationsOfType(tableArgument.type) or "any", formatLineBreaks(tableArgument.description or common.defaultNoDescriptionText))
			end
		end
		if (argument.type == "variadic") then
			file:write(string.format("--- @vararg %s %s\n", getAllPossibleVariationsOfType(argument.variadicType) or "any", formatLineBreaks(description)))
		else
			file:write(string.format("--- @param %s %s %s\n", argument.name or "unknown", getAllPossibleVariationsOfType(type), formatLineBreaks(description)))
		end
	end

	for _, returnPackage in ipairs(common.getConsistentReturnValues(package) or {}) do
		file:write(string.format("--- @return %s %s %s\n", getAllPossibleVariationsOfType(returnPackage.type) or "any", returnPackage.name or "result", returnPackage.description or common.defaultNoDescriptionText))
	end

	file:write(string.format("function %s(%s) end\n\n", namespaceOverride or package.namespace, table.concat(getParamNames(package), ", ")))

	if (package.arguments and #package.arguments > 0 and package.arguments[1].tableParams) then
		file:write(string.format("---Table parameter definitions for ``%s``.\n", package.namespace))
		file:write(string.format("--- @class %s.params\n", package.namespace))
		for _, param in ipairs(package.arguments[1].tableParams) do
			file:write(string.format("--- @field %s %s %s\n", param.name, getAllPossibleVariationsOfType(param.type), param.description or common.defaultNoDescriptionText))
		end
		file:write("\n")
	end
end


--
-- Compile data
--

common.compilePath(lfs.join(common.pathDefinitions, "global"), libraries, "lib")
common.compilePath(lfs.join(common.pathDefinitions, "namedTypes"), classes, "class")


--
-- Building
--

local function buildParentChain(className)
	local package = assert(classes[className])
	if (package.inherits) then
		return className .. ", " .. buildParentChain(package.inherits)
	end
	return className
end

local function build(package)
	-- Load our base package.
	common.log("Building " .. package.type .. ": " .. package.key .. " ...")

	-- Get the package.
	local outPath = lfs.join(metaFolder, package.type, package.key .. ".lua")
	local file = assert(io.open(outPath, "w"))

	-- Mark the file as a meta file.
	file:write("--- @meta\n\n")

	-- Write description.
	file:write(formatDescription(package.description or common.defaultNoDescriptionText) .. "\n")
	writeExamples(package, file)
	if (package.type == "lib") then
		file:write(string.format("--- @class %slib\n", package.key))
	elseif (package.type == "class") then
		file:write(string.format("--- @class %s%s\n", package.key, package.inherits and (" : " .. buildParentChain(package.inherits)) or ""))
	end

	-- Write out fields.
	for _, value in ipairs(package.values or {}) do
		file:write(string.format("--- @field %s %s %s\n", value.key, getAllPossibleVariationsOfType(value.valuetype) or "any", formatLineBreaks(value.description or common.defaultNoDescriptionText)))
	end

	-- Finalize the main class definition.
	file:write(string.format("%s = {}\n\n", package.key))

	-- Write out functions.
	for _, value in ipairs(package.functions or {}) do
		writeFunction(value, file)
	end

	-- Write out methods.
	if (package.type == "class") then
		for _, value in ipairs(package.methods or {}) do
			writeFunction(value, file, package.key .. ":" .. value.key)
		end
	end

	-- Close up shop.
	file:close()
end

for _, package in pairs(libraries) do
	build(package)
end

for _, package in pairs(classes) do
	build(package)
end
