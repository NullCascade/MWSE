return {
	type = "function",
	description = [[Decode string into a table.]],
	link = "http://dkolf.de/src/dkjson-lua.fsl/wiki?name=Documentation",
	arguments = {
		{ name = "s", type = "string" },
		{ name = "position", type = "number", default = 1 },
		{ name = "nullValue", type = "string|nil", default = "nil" },
	},
	valuetype = "table",
}