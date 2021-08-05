createFillBar
====================================================================================================

Creates a horizontal quantity indicator bar.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.current``: Current (filled) value.
        | `number`_ (integer) ``element.widget.max``: Maximum value.
        | `boolean`_ ``element.widget.showText``: If text of the format "current/max" is shown. Default is ``true``.
        | `table`_ (float[3]) ``element.widget.fillColor``: Colour of filled area.
        | `number`_ (float) ``element.widget.fillAlpha``: Alpha transparency of filled area.

Returns
----------------------------------------------------------------------------------------------------

result (`tes3uiElement`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`number`_)
    Optional. A registered identifier to help find this element later.

current (`number`_)
    Optional. The initial current value.

max (`number`_)
    Optional. The initial maximum value.

.. _``element.widget.current``: Current (filled) value.
        | `number`: ../../../lua/type/`element.widget.current``: Current (filled) value.
        | `number.html
.. _``element.widget.fillColor``: Colour of filled area.
        | `number`: ../../../lua/type/`element.widget.fillColor``: Colour of filled area.
        | `number.html
.. _``element.widget.max``: Maximum value.
        | `boolean`: ../../../lua/type/`element.widget.max``: Maximum value.
        | `boolean.html
.. _``element.widget.showText``: If text of the format "current/max" is shown. Default is ``true``.
        | `table`: ../../../lua/type/`element.widget.showText``: If text of the format "current/max" is shown. Default is ``true``.
        | `table.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
