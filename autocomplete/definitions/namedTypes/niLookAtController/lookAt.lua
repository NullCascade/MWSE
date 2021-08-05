return {
	type = "value",
	description = [[The object that this controller will point towards. This is not a reference counted pointer, so this controller must be removed or lookAt set to nil before the lookAt object is deleted. This was probably designed to avoid reference cycles where controllers keep an ancestor node alive.]],
	valuetype = "niAVObject",
}