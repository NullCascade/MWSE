local common = {}

--- A wrapper around `print` that allows format strings.
--- @param fmt string The format string.
--- @vararg any Arguments for formatting.
function common.log(fmt, ...)
	print(fmt:format(...))
end


--
-- String library extensions
--

--- @param str string
--- @param sep string
--- @return table
function string.split(str, sep)
	if sep == nil then
		sep = "%s"
	end
	local t = {}
	for str in string.gmatch(str, "([^" .. sep .. "]+)") do
		table.insert(t, str)
	end
	return t
end

--- @param s string
--- @return string
function string.trim(s)
	return string.match(s, '^()%s*$') and '' or string.match(s, '^%s*(.*%S)')
end


--
-- Table library extensions
--

--- @param t table
--- @return boolean
function table.empty(t)
	for _, _ in pairs(t) do
		return false
	end
	return true
end

--- @param t table
--- @param sort boolean|function|nil
--- @return table
function table.keys(t, sort)
	local keys = {}
	for k, _ in pairs(t) do
		table.insert(keys, k)
	end

	if (sort) then
		if (sort == true) then
			sort = nil
		end
		table.sort(keys, sort)
	end

	return keys
end

--- @param t table
--- @param sort boolean|function|nil
--- @return table
function table.values(t, sort)
	local values = {}
	for _, v in pairs(t) do
		table.insert(values, v)
	end

	if (sort) then
		if (sort == true) then
			sort = nil
		end
		table.sort(values, sort)
	end

	return values
end

--- @param t table
--- @param d table|nil
--- @return table
function table.copy(t, d)
	if (d == nil) then
		d = {}
	elseif (type(t) ~= "table" or type(d) ~= "table") then
		error("Arguments for table.copy must be tables.")
	end

	for k, v in pairs(t) do
		d[k] = v
	end

	return d
end

--- @param t table
--- @return table
function table.deepcopy(t)
	local copy = nil
	if type(t) == "table" then
		copy = {}
		for k, v in next, t, nil do
			copy[table.deepcopy(k)] = table.deepcopy(v)
		end
		setmetatable(copy, table.deepcopy(getmetatable(t)))
	else
		copy = t
	end
	return copy
end


--
-- json library extensions
--

local json = require("dkjson")

--- @param path string
--- @return table|nil decoded
function json.loadfile(path)
	-- Load the contents of the file.
	local f = io.open(path, "r")
	if (f == nil) then
		return nil
	end
	local fileContents = f:read("*all")
	f:close()

	-- Return decoded json.
	return json.decode(fileContents)
end

--- @param path string
--- @param object table
--- @param config table
function json.savefile(path, object, config)
	local f = assert(io.open(path, "w"))
	f:write(json.encode(object, config))
	f:close()
end


--
-- lfs library extensions
--

local lfs = require("lfs")

--- @vararg string
--- @return string
function lfs.join(...)
	return table.concat({ ... }, "\\")
end

--- @param path string
--- @return boolean
function lfs.isdir(path)
	return lfs.attributes(path, "mode") == "directory"
end

--- @param path string
--- @return boolean
function lfs.isfile(path)
	return lfs.attributes(path, "mode") == "directory"
end

function lfs.copyfile(from, to)
	local source = io.open(from, "r")
	local destination = io.open(to, "w")

	destination:write(source:read("*a"))

	source:close()
	destination:close()
end

-- Cache the original lfs.rmdir and replace it with a version that supports recursion.
lfs.rmdir_old = lfs.rmdir

---@param dir string
---@param recursive boolean
---@return boolean
function lfs.rmdir(dir, recursive)
	-- Default to not being recursive.
	local recursive = recursive or false
	if (recursive) then
		for file in lfs.dir(dir) do
			local path = dir .. "\\" .. file
			if (file ~= "." and file ~= "..") then
				if (lfs.attributes(path, "mode") == "file") then
					os.remove(path)
				elseif (lfs.attributes(path, "mode") == "directory") then
					lfs.rmdir(path, true)
				end
			end
		end
	end

	-- Call the original function at the end.
	return lfs.rmdir_old(dir)
end

---@param path string
function lfs.remakedir(path)
	assert(lfs.rmdir(path, true))
	assert(lfs.mkdir(path))
end


--
-- Add some common paths and urls.
--

--- The path to the autocomplete folder. `MWSE/autocomplete`
--- @type string
common.pathAutocomplete = lfs.currentdir()

--- The path to the definitions folder. `MWSE/autocomplete/definitions`
--- @type string
common.pathDefinitions = lfs.join(common.pathAutocomplete, "definitions")

--- @vararg string
--- @return string
function common.urlJoin(...)
	local paths = { ... }
	return table.concat(paths, "/")
end

common.urlBase = "https://mwse.readthedocs.io/en/latest/lua"


--
-- Reused language strings
--

--- @type string
common.defaultNoDescriptionText = "No description yet available."


--
-- Package compilation
--

--- @class package
--- @field key package The name of the file that generated this package.
--- @field type string The type definitionf or the package.
--- @field folder string The folder that the package was created from.
--- @field parent package The package this package is a child of.
--- @field namespace string The full namespace of the package.

--- @class packageLib : package
--- @field children table<string, package>|nil
--- @field functions package[]|nil
--- @field values package[]|nil

--- @class packageClass : packageLib
--- @field inherits string The class that this class descends from.
--- @field isAbstract boolean
--- @field methods package[]|nil
--- @field allDescendents string
--- @field directDescendents table<string, packageClass>

--- @class packageFunction : package

--- @class packageMethod : packageFunction
--- @field returns table

--- comment
--- @param package packageFunction
--- @return table
function common.getConsistentReturnValues(package)
	if (type(package.returns) == "string" and package.valuetype == nil) then
		return { { name = "result", type = package.returns } }
	elseif (type(package.returns) == "string" and package.valuetype ~= nil) then
		return { { name = package.returns, type = package.valuetype } }
	elseif (package.valuetype) then
		return { { name = "result", type = package.valuetype } }
	elseif (type(package.returns) == "table") then
		if (package.returns.name or package.returns.type) then
			return { package.returns }
		elseif (#package.returns > 0) then
			return package.returns
		end
		error("Invalid parameters table.")
	end
end

--- comment
--- @param package package
--- @return string
local function getFullPackageNamespace(package)
	if (package.parent) then
		return (package.parent.namespace or getFullPackageNamespace(package.parent)) .. "." .. package.key
	end
	return package.key
end

--- comment
--- @param folder string
--- @param key string
--- @param parent package
function common.compileEntry(folder, key, parent)
	-- Load our base package.
	local path = lfs.join(folder, key .. ".lua")
	--- @type package
	local package = dofile(path)
	if (package == nil) then
		error("Could not execute typed entry: " .. path)
	end

	-- Setup basic package info.
	package.key = key
	package.folder = folder
	package.parent = parent
	package.namespace = getFullPackageNamespace(package)

	-- Setup children access on parents.
	parent.children = parent.children or {}
	parent.children[key] = package

	-- Add to things.
	local collection = package.type .. "s"
	parent[collection] = parent[collection] or {}
	table.insert(parent[collection], package)
end

--- comment
--- @param folder string
--- @param key string
--- @param owningCollection table<string, package>
--- @param acceptedType string
function common.compile(folder, key, owningCollection, acceptedType)
	-- Load our base package.
	local path = lfs.join(folder, key .. ".lua")
	common.log("Compiling %s: %s ...", acceptedType or "package", key)
	local package = dofile(path)
	package.key = key
	package.namespace = getFullPackageNamespace(package)

	-- We only care about libraries for now.
	if (acceptedType and package.type ~= acceptedType) then
		return
	end

	-- Write out children.
	if (acceptedType ~= "event") then
		for entry in lfs.dir(lfs.join(folder, key)) do
			local extension = entry:match("[^.]+$")
			if (extension == "lua") then
				common.compileEntry(lfs.join(folder, key), entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), package)
			end
		end
	end

	-- Store it for later.
	owningCollection[key] = package
end

--- comment
--- @param path string
--- @param owningCollection table<string, package>
--- @param acceptedType string
function common.compilePath(path, owningCollection, acceptedType)
	for entry in lfs.dir(path) do
		local extension = entry:match("[^.]+$")
		if (extension == "lua") then
			common.compile(path, entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), owningCollection, acceptedType)
		end
	end

	if (acceptedType == "class") then
		common.compileInheritances(owningCollection)
	end
end

--- @param classes table<string, packageClass>
function common.compileInheritances(classes)-- Figure out inheritances.
	for _, class in pairs(classes) do
		if (class.inherits) then
			local parent = classes[class.inherits]
			if (parent) then
				parent.directDescendents = parent.directDescendents or {}
				parent.directDescendents[class.key] = class

				while (parent) do
					parent.allDescendents = parent.allDescendents or {}
					parent.allDescendents[class.key] = class
					parent = classes[parent.inherits]
				end
			end
		end
	end

	-- Update allDescendentKeys
	-- Explosions. I should have kept calling them explosion keys. Way cooler.
	for _, class in pairs(classes) do
		if (class.allDescendents) then
			local allDescendentKeys = {}
			if (not class.isAbstract) then
				table.insert(allDescendentKeys, class.key)
			end
			for _, descendent in pairs(class.allDescendents) do
				if (not descendent.isAbstract) then
					table.insert(allDescendentKeys, descendent.key)
				end
			end
			if (#allDescendentKeys > 0) then
				table.sort(allDescendentKeys)
				class.allDescendentKeys = table.concat(allDescendentKeys, "|")
			end
		end
	end
end


return common