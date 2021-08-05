create
====================================================================================================

Creates a new activator object, which will be stored as part of the current saved game.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`string`_)
    Optional. The new object's ID. Must be unique if provided.

name (`string`_)
    Default: ``Activator``. The new object's name.

script (`tes3script`_)
    Optional. A script to attach to the object.

mesh (`string`_)
    Default: ````. The mesh to use for the object.

objectFlags (`number`_)
    Default: ``0``. The object flags initially set. Force set as modified.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3script`: ../../../lua/type/tes3script.html
