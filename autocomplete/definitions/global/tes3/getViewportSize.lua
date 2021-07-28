return {
	type = "function",
	description = [[Returns both the viewport width and the viewport height. Note that this the real resolution of the screen. For a value scaled by MGE's menu scaling, see the same-named function in the tes3ui namespace. To get the scale used, check getViewportScale in the tes3ui namespace.]],
	returns = {
		{ name = "width", type = "number", description = "The width of the viewport." },
		{ name = "height", type = "number", description = "The height of the viewport." },
	}
}
