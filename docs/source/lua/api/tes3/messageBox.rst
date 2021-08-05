tes3.messageBox
====================================================================================================

Displays a message box. This may be a simple toast-style message, or a box with choice buttons.

Returns
----------------------------------------------------------------------------------------------------

element (`tes3uiElement`_, `nil`_)
    The UI menu created for the notification, if any.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

message (`string`_)
    No description available.

buttons (`table`_)
    Optional. An array of strings to use for buttons.

callback (`function`_)
    No description available.

showInDialog (`boolean`_)
    Optional. Specifying showInDialog = false forces the toast-style message, which is not shown in the dialog menu. Defaults to true.

duration (`float`_)
    Optional. Overrides how long the toast-style message remains visible.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`float`: ../../../lua/type/float.html
.. _`function`: ../../../lua/type/function.html
.. _`nil`: ../../../lua/type/nil.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
