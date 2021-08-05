tes3uiMenuInputController
====================================================================================================

A core came structure responsible for managing the input around menu elements.

Properties
----------------------------------------------------------------------------------------------------

`lastInputTime <tes3uiMenuInputController/lastInputTime.html>`_ (`number`_)
    The amount of seconds that have passed since the last time the input was updated.

`menuController <tes3uiMenuInputController/menuController.html>`_ (`tes3uiMenuController`_)
    A handle back to the main menu controller.

`textInputFocus <tes3uiMenuInputController/textInputFocus.html>`_ (`tes3uiElement`_)
    The element that is currently focused for text input.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3uiMenuInputController/lastInputTime
    tes3uiMenuInputController/menuController
    tes3uiMenuInputController/textInputFocus

Methods
----------------------------------------------------------------------------------------------------

`flushBufferedTextEvents <tes3uiMenuInputController/flushBufferedTextEvents.html>`_ (`method`_)
    Clears the current text input. If no text has been passed to the current text focus, this will prevent it from doing so.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3uiMenuInputController/flushBufferedTextEvents

.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`tes3uiElement`: ../../lua/type/tes3uiElement.html
.. _`tes3uiMenuController`: ../../lua/type/tes3uiMenuController.html
