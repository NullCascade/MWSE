return {
	type = "function",
	description = [[Returns both the viewport width and the viewport height. Note that this value is not necessarily the real resolution of the screen. For that value, see the same-named function in the tes3 namespace. To get the scale used, check getViewportScale.]],
	returns = {
		{ name = "width", type = "number", description = "The scaled width of the viewport." },
		{ name = "height", type = "number", description = "The scaled height of the viewport." },
	}
}
