return {
	type = "class",
	description = [[Orients an axis of the controlled object towards a lookAt target. The axis is selectable.

Warning: The lookAt property is not reference counted, so this controller must be removed, or lookAt set to nil before the lookAt object is deleted.]],
	inherits = "niTimeController",
}