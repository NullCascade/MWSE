return {
	type = "function",
	description = [[Creates a timer.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "type", type = "number" },
			{ name = "duration", type = "number" },
			{ name = "callback", type = "function" },
			{ name = "iterations", type = "number", optional = true },
		}
	}},
	returns = "timer",
	valuetype = "mwseTimer",
}