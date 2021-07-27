return {
	type = "method",
	description = [[Creates a selectable line of text, with configurable hover, click, and disabled colours. Can be used to create a list box by placing them in a ScrollPane. ``state`` sets the initial interaction state.]],
	arguments = {
		{ name = "id", type = "number", description = "A registered identifier to help find this element later.", optional = true },
		{ name = "text", type = "string", description = "The text to display.", optional = true },
		{ name = "state", type = "number", description = "The initial interaction state. Defaults to normal.", optional = true },
	},
    valuetype = "tes3uiWidgetTextSelect",
}