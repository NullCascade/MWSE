createButton
====================================================================================================

Creates a clickable button. Register the "mouseClick" event to capture a button press.
    
    Custom widget properties:
        | `number`_ ``element.widget.state``: Interaction state. 1 = normal, 2 = disabled, 4 = active. Controls which colour set to use for text.
        | `table`_ (float[3]) ``element.widget.idle``: Text colour for normal state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.over``: Text colour for normal state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressed``: Text colour for normal state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleDisabled``: Text colour for disabled state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overDisabled``: Text colour for disabled state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedDisabled``: Text colour for disabled state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleActive``: Text colour for active state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overActive``: Text colour for active state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedActive``: Text colour for active state, on mouseDown.

Returns
----------------------------------------------------------------------------------------------------

result (`tes3uiElement`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`number`_)
    Optional. A registered identifier to help find this element later.

.. _``element.widget.idleActive``: Text colour for active state, no mouse interaction.
        | `table`: ../../../lua/type/`element.widget.idleActive``: Text colour for active state, no mouse interaction.
        | `table.html
.. _``element.widget.idleDisabled``: Text colour for disabled state, no mouse interaction.
        | `table`: ../../../lua/type/`element.widget.idleDisabled``: Text colour for disabled state, no mouse interaction.
        | `table.html
.. _``element.widget.idle``: Text colour for normal state, no mouse interaction.
        | `table`: ../../../lua/type/`element.widget.idle``: Text colour for normal state, no mouse interaction.
        | `table.html
.. _``element.widget.overActive``: Text colour for active state, on mouseOver.
        | `table`: ../../../lua/type/`element.widget.overActive``: Text colour for active state, on mouseOver.
        | `table.html
.. _``element.widget.overDisabled``: Text colour for disabled state, on mouseOver.
        | `table`: ../../../lua/type/`element.widget.overDisabled``: Text colour for disabled state, on mouseOver.
        | `table.html
.. _``element.widget.over``: Text colour for normal state, on mouseOver.
        | `table`: ../../../lua/type/`element.widget.over``: Text colour for normal state, on mouseOver.
        | `table.html
.. _``element.widget.pressedDisabled``: Text colour for disabled state, on mouseDown.
        | `table`: ../../../lua/type/`element.widget.pressedDisabled``: Text colour for disabled state, on mouseDown.
        | `table.html
.. _``element.widget.pressed``: Text colour for normal state, on mouseDown.
        | `table`: ../../../lua/type/`element.widget.pressed``: Text colour for normal state, on mouseDown.
        | `table.html
.. _``element.widget.state``: Interaction state. 1 = normal, 2 = disabled, 4 = active. Controls which colour set to use for text.
        | `table`: ../../../lua/type/`element.widget.state``: Interaction state. 1 = normal, 2 = disabled, 4 = active. Controls which colour set to use for text.
        | `table.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
