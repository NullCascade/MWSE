tes3ui.createTooltipMenu
====================================================================================================

Creates a tooltip menu, which can be an empty menu or an item tooltip. This should be called from within a tooltip event callback. These automatically follow the mouse cursor, and are also destroyed automatically when the mouse leaves the originating element. Creating an item tooltip will invoke the uiObjectTooltip event.

Returns
----------------------------------------------------------------------------------------------------

`tes3uiElement`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

item (`tes3item`_, `string`_)
    Optional. The item to create a tooltip for. If not specified, the tooltip will be empty.

itemData (`tes3itemData`_)
    Optional. The item data for the item.

Examples
----------------------------------------------------------------------------------------------------

Add an item tooltip to a new element
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This demonstrates adding an item tooltip to a button using the help event.

.. code-block:: lua

    local button = menu:createButton{text = "- Item -"}
    local item = tes3.getObject("iron_shield")

    button:register("help", function(e)
        local tooltip = tes3ui.createTooltipMenu{item = item}
        tooltip:createLabel{text = "Additional custom text"}
    end)


.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
