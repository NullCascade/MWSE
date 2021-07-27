return {
	type = "function",
	description = [[Create a string representing the object. Object can be a table, a string, a number, a boolean, nil, json.null or any object with a function __tojson in its metatable. A table can only use strings and numbers as keys and its values have to be valid objects as well. It raises an error for any invalid data types or reference cycles.]],
	link = "http://dkolf.de/src/dkjson-lua.fsl/wiki?name=Documentation",
	arguments = {
		{ name = "object", type = "table" },
		{ name = "state", type = "table" },
	},
	valuetype = "string",
}