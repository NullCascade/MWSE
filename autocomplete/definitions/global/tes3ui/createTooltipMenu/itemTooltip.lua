local button = menu:createButton{text = "- Item -"}
local item = tes3.getObject("iron_shield")

button:register("help", function(e)
	local tooltip = tes3ui.createTooltipMenu{item = item}
	tooltip:createLabel{text = "Additional custom text"}
end)
