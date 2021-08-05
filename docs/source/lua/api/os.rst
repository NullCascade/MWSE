os
====================================================================================================

Provides various OS-specific functions.

Functions
----------------------------------------------------------------------------------------------------

`clock <os/clock.html>`_ (`function`_)
    Return CPU time since Lua started in seconds.

`getClipboardText <os/getClipboardText.html>`_ (`function`_)
    Returns the current clipboard text, if any is available.

`setClipboardText <os/setClipboardText.html>`_ (`function`_)
    Sets the current clipboard text. If nil is passed, the clipboard will be cleared instead. If either operation fails, the function returns false.



.. toctree::
    :hidden:
    :maxdepth: 1

    os/clock
    os/getClipboardText
    os/setClipboardText

.. _`function`: ../../lua/type/function.html
