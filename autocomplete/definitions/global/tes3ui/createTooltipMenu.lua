return {
	type = "function",
	description = [[Creates a tooltip menu, which can be an empty menu or an item tooltip. This should be called from within a tooltip event callback. These automatically follow the mouse cursor, and are also destroyed automatically when the mouse leaves the originating element. Creating an item tooltip will invoke the uiObjectTooltip event.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "item", type = "tes3item|string", optional = true, description = "The item to create a tooltip for. If not specified, the tooltip will be empty." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data for the item." },
		},
	}},
	valuetype = "tes3uiElement",
	examples = {
		["itemTooltip"] = {
			title = "Add an item tooltip to a new element",
			description = "This demonstrates adding an item tooltip to a button using the help event."
		},
	}
}
