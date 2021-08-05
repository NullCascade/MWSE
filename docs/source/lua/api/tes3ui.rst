tes3ui
====================================================================================================

The tes3ui library provides access to manipulating the game's GUI.

Functions
----------------------------------------------------------------------------------------------------

`acquireTextInput <tes3ui/acquireTextInput.html>`_ (`function`_)
    Sends all text input to the specified element.  Calling this function with a nil argument will release text input and allow keybinds to work. Suppresses most keybinds while active, except the Journal open/close keybind (it's up to the individual menu implementation).

Only one element can have control of input, and there is no automatic restoration of control if one element takes control from another. Be careful of conflicts with other users of this function.

`captureMouseDrag <tes3ui/captureMouseDrag.html>`_ (`function`_)
    When used in a mouse event, causes the element to capture further mouse events even when the cursor goes outside the element.

`createHelpLayerMenu <tes3ui/createHelpLayerMenu.html>`_ (`function`_)
    Creates a help layer menu. Help layer menus include notifications and tooltips that are always above the rest of the interface. The game realizes this using a separate menu root and set of functions.

Note, to create tooltips with the correct behaviour, use tes3ui.createTooltipMenu.

Unlike standard menus, help layer menus are always created with a fixed frame.

`createMenu <tes3ui/createMenu.html>`_ (`function`_)
    Creates a top-level menu.

`createTooltipMenu <tes3ui/createTooltipMenu.html>`_ (`function`_)
    Creates a tooltip menu, which can be an empty menu or an item tooltip. This should be called from within a tooltip event callback. These automatically follow the mouse cursor, and are also destroyed automatically when the mouse leaves the originating element. Creating an item tooltip will invoke the uiObjectTooltip event.

`enterMenuMode <tes3ui/enterMenuMode.html>`_ (`function`_)
    Requests menu mode be activated on a menu with a given id.

`findHelpLayerMenu <tes3ui/findHelpLayerMenu.html>`_ (`function`_)
    Locates a help layer menu through its id. Help layer menus include notifications and tooltips that are always above the rest of the interface. The game realizes this using a separate menu root and set of functions.

`findMenu <tes3ui/findMenu.html>`_ (`function`_)
    Locates a top-level menu through its id.

`forcePlayerInventoryUpdate <tes3ui/forcePlayerInventoryUpdate.html>`_ (`function`_)
    Forces the game to update the inventory tile GUI elements. Unlike tes3ui.updateInventoryTiles, this will force-resync the player's inventory to the GUI, rather than updating what is already in the GUI system.

`getInventorySelectType <tes3ui/getInventorySelectType.html>`_ (`function`_)
    Determines what sort of search is being done when performing an inventory selection, e.g. "alembic" or "ingredient" or "soulGemFilled".

`getMenuOnTop <tes3ui/getMenuOnTop.html>`_ (`function`_)
    Returns the top-most, active menu.

`getPalette <tes3ui/getPalette.html>`_ (`function`_)
    Gets a standard palette color. Returns an array containing the RGB color values, in the range [0.0, 1.0].

`getServiceActor <tes3ui/getServiceActor.html>`_ (`function`_)
    Returns the mobile actor currently providing services to the player.

`getViewportScale <tes3ui/getViewportScale.html>`_ (`function`_)
    Returns the UI scale, set in MGE.

`getViewportSize <tes3ui/getViewportSize.html>`_ (`function`_)
    Returns both the viewport width and the viewport height. Note that this value is not necessarily the real resolution of the screen. For that value, see the same-named function in the tes3 namespace. To get the scale used, check getViewportScale.

`leaveMenuMode <tes3ui/leaveMenuMode.html>`_ (`function`_)
    Requests menu mode be deactivated on a menu with a given id.

`logToConsole <tes3ui/logToConsole.html>`_ (`function`_)
    Logs a message to the console.

`menuMode <tes3ui/menuMode.html>`_ (`function`_)
    Checks if the game is in menu mode.

`registerID <tes3ui/registerID.html>`_ (`function`_)
    Registers a UI element name, returning a UI_ID. Once a property is registered, this function always returns the same UI_ID. These UI_IDs are used by the API to locate elements that may not exist (a weak reference), instead of by element name.

The registry namespace is shared between Property and UI_ID. It is advisable to use a namespace prefix to avoid collisions with other mods.

`registerProperty <tes3ui/registerProperty.html>`_ (`function`_)
    Registers a property name, returning a Property. Once a property is registered, this function always returns the same Property.

The registry namespace is shared between Property and UI_ID. It is advisable to use a namespace prefix to avoid collisions with other mods.

`showBookMenu <tes3ui/showBookMenu.html>`_ (`function`_)
    Displays the book menu with arbitrary text. Paging is automatically handled.

`showScrollMenu <tes3ui/showScrollMenu.html>`_ (`function`_)
    Displays the scroll menu with arbitrary text.

`suppressTooltip <tes3ui/suppressTooltip.html>`_ (`function`_)
    Controls hiding of world object tooltips.

`updateBarterMenuTiles <tes3ui/updateBarterMenuTiles.html>`_ (`function`_)
    Forces the game to update the barter tile GUI elements.

`updateInventoryCharacterImage <tes3ui/updateInventoryCharacterImage.html>`_ (`function`_)
    Tells the game to update the character image in the player's inventory. It will update immediately if the inventory is open, or the next time the inventory is opened otherwise.

`updateInventorySelectTiles <tes3ui/updateInventorySelectTiles.html>`_ (`function`_)
    Forces the game to update the inventory selection GUI elements.

`updateInventoryTiles <tes3ui/updateInventoryTiles.html>`_ (`function`_)
    Forces the game to update the inventory tile GUI elements.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3ui/acquireTextInput
    tes3ui/captureMouseDrag
    tes3ui/createHelpLayerMenu
    tes3ui/createMenu
    tes3ui/createTooltipMenu
    tes3ui/enterMenuMode
    tes3ui/findHelpLayerMenu
    tes3ui/findMenu
    tes3ui/forcePlayerInventoryUpdate
    tes3ui/getInventorySelectType
    tes3ui/getMenuOnTop
    tes3ui/getPalette
    tes3ui/getServiceActor
    tes3ui/getViewportScale
    tes3ui/getViewportSize
    tes3ui/leaveMenuMode
    tes3ui/logToConsole
    tes3ui/menuMode
    tes3ui/registerID
    tes3ui/registerProperty
    tes3ui/showBookMenu
    tes3ui/showScrollMenu
    tes3ui/suppressTooltip
    tes3ui/updateBarterMenuTiles
    tes3ui/updateInventoryCharacterImage
    tes3ui/updateInventorySelectTiles
    tes3ui/updateInventoryTiles

.. _`function`: ../../lua/type/function.html
