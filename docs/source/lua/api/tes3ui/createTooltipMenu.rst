tes3ui.createTooltipMenu
====================================================================================================

Creates a tooltip menu, which can be an empty menu or an item tooltip. This should be called from within a tooltip event callback. These automatically follow the mouse cursor, and are also destroyed automatically when the mouse leaves the originating element. Creating an item tooltip will invoke the uiObjectTooltip event.

Returns
----------------------------------------------------------------------------------------------------

result (`tes3uiElement`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

item (`tes3item`_, `string`_)
    Optional. The item to create a tooltip for. If not specified, the tooltip will be empty.

itemData (`tes3itemData`_)
    Optional. The item data for the item.

.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
