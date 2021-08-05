tes3inputController
====================================================================================================

A data structure, off of the world controller, that handles input.

Properties
----------------------------------------------------------------------------------------------------

`creationFlags <tes3inputController/creationFlags.html>`_ (`number`_)
    A bit field representing device capabilities and settings.

`inputMaps <tes3inputController/inputMaps.html>`_ (`table`_)
    The array-style access to input bindings.

`keyboardState <tes3inputController/keyboardState.html>`_ (`table`_)
    The array-style access to raw key states.

`mouseState <tes3inputController/mouseState.html>`_ (`tes3directInputMouseState`_)
    The raw DirectInput mouse state.

`previousKeyboardState <tes3inputController/previousKeyboardState.html>`_ (`table`_)
    The array-style access to the previous frame's raw key states.

`previousMouseState <tes3inputController/previousMouseState.html>`_ (`tes3directInputMouseState`_)
    The raw DirectInput mouse state for the previous state.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3inputController/creationFlags
    tes3inputController/inputMaps
    tes3inputController/keyboardState
    tes3inputController/mouseState
    tes3inputController/previousKeyboardState
    tes3inputController/previousMouseState

Methods
----------------------------------------------------------------------------------------------------

`isAltDown <tes3inputController/isAltDown.html>`_ (`method`_)
    Returns true if either alt modifier key is pressed.

`isCapsLockActive <tes3inputController/isCapsLockActive.html>`_ (`method`_)
    Returns true if the caps lock mode is active.

`isControlDown <tes3inputController/isControlDown.html>`_ (`method`_)
    Returns true if either control modifier key is pressed.

`isKeyDown <tes3inputController/isKeyDown.html>`_ (`method`_)
    Performs a key down test for a given scan key code.

`isKeyPressedThisFrame <tes3inputController/isKeyPressedThisFrame.html>`_ (`method`_)
    Checks to see if a given scan code is pressed, and wasn't pressed last frame.

`isKeyReleasedThisFrame <tes3inputController/isKeyReleasedThisFrame.html>`_ (`method`_)
    Checks to see if a given scan code is released, and was pressed last frame.

`isMouseButtonDown <tes3inputController/isMouseButtonDown.html>`_ (`method`_)
    Returns the mouse down state for a given mouse button.

`isMouseButtonPressedThisFrame <tes3inputController/isMouseButtonPressedThisFrame.html>`_ (`method`_)
    Returns true if a mouse button was up last frame, and down this frame.

`isMouseButtonReleasedThisFrame <tes3inputController/isMouseButtonReleasedThisFrame.html>`_ (`method`_)
    Returns true if a mouse button was down last frame, and up this frame.

`isShiftDown <tes3inputController/isShiftDown.html>`_ (`method`_)
    Returns true if either shift modifier key is pressed. It does not take into account caps lock.

`isSuperDown <tes3inputController/isSuperDown.html>`_ (`method`_)
    Returns true if either super/windows modifier key is pressed.

`keybindTest <tes3inputController/keybindTest.html>`_ (`method`_)
    Performs a test for a given keybind, and optionally a transition state.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3inputController/isAltDown
    tes3inputController/isCapsLockActive
    tes3inputController/isControlDown
    tes3inputController/isKeyDown
    tes3inputController/isKeyPressedThisFrame
    tes3inputController/isKeyReleasedThisFrame
    tes3inputController/isMouseButtonDown
    tes3inputController/isMouseButtonPressedThisFrame
    tes3inputController/isMouseButtonReleasedThisFrame
    tes3inputController/isShiftDown
    tes3inputController/isSuperDown
    tes3inputController/keybindTest

.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3directInputMouseState`: ../../lua/type/tes3directInputMouseState.html
