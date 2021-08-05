return {
	type = "event",
	description = "This event is triggered when the player attempts to brew a potion. It controls both the strength of the potion's effects, and if the brew is successful. These both use the player's stats, so the game combines them into one function. Because the potion effects are not determined yet, the potion data is not available. When using this event, use the example as a template for your own code.",
	eventData = {
		["potionStrength"] = {
			type = "number",
			description = "Can be modified. The strength multiplier used to scale the potion effects. For each different effect, it can modify magnitude, duration, or both. This is initially equal to -1 if the potion failed to brew.",
		},
		["success"] = {
			type = "boolean",
			description = "Can be modified. Whether the brewing is successful.",
		},
		["alembic"] = {
			type = "tes3apparatus",
			readOnly = true,
			description = "The alembic apparatus used to create the potion.",
		},
		["calcinator"] = {
			type = "tes3apparatus",
			readOnly = true,
			description = "The calcinator apparatus used to create the potion.",
		},
		["mortar"] = {
			type = "tes3apparatus",
			readOnly = true,
			description = "The mortar apparatus used to create the potion.",
		},
		["retort"] = {
			type = "tes3apparatus",
			readOnly = true,
			description = "The retort apparatus used to create the potion.",
		},
	},
	examples = {
		["Template"] = {
			title = "Vanilla potion skill check function",
		},
	},
}