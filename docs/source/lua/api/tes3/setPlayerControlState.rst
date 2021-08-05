tes3.setPlayerControlState
====================================================================================================

Enables or disables player's controls state.

Returns
----------------------------------------------------------------------------------------------------

changedControlState (`boolean`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

enabled (`boolean`_)
    Optional. Setting this to false will disable any kind of control.

attack (`boolean`_)
    Optional. If this is false, it will block player from attacking.

jumping (`boolean`_)
    Optional. If this is false, it will block player from jumping.

magic (`boolean`_)
    Optional. If this is false, it will block player from using magic.

vanity (`boolean`_)
    Optional. If this is false, it will block player from going to vanity mode.

viewSwitch (`boolean`_)
    Optional. If this is false, it will block player changing view mod from 1st to 3rd person camera and vice versa.

.. _`boolean`: ../../../lua/type/boolean.html
