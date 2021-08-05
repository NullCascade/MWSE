createTextInput
====================================================================================================

Creates a single line text input element. To receive input the keyboard must be captured with ``tes3ui.acquireTextInput(element)``. Read the input with the ``text`` property. Write an initial value to display by setting the ``text`` property; that value will be cleared on the first keypress.

    Custom widget properties:
        | `boolean`_ ``element.widget.eraseOnFirstKey``: Clears the initial value if the first keypress is not an edit action. Default is ``true``.
        | `number`_ (integer) ``element.widget.lengthLimit"``: Maximum input length, or ``nil`` for no limit. If you are setting names, the engine limits most identifiers to 31 characters. Default is ``nil``.

Returns
----------------------------------------------------------------------------------------------------

result (`tes3uiElement`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`number`_)
    Optional. A registered identifier to help find this element later.

.. _``element.widget.eraseOnFirstKey``: Clears the initial value if the first keypress is not an edit action. Default is ``true``.
        | `number`: ../../../lua/type/`element.widget.eraseOnFirstKey``: Clears the initial value if the first keypress is not an edit action. Default is ``true``.
        | `number.html
.. _``tes3ui.acquireTextInput(element)``. Read the input with the ``text`` property. Write an initial value to display by setting the ``text`` property; that value will be cleared on the first keypress.

    Custom widget properties:
        | `boolean`: ../../../lua/type/`tes3ui.acquireTextInput(element)``. Read the input with the ``text`` property. Write an initial value to display by setting the ``text`` property; that value will be cleared on the first keypress.

    Custom widget properties:
        | `boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
