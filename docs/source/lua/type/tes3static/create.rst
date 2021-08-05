create
====================================================================================================

Creates a new static object, which will be stored as part of the current saved game.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`string`_)
    Optional. The new object's ID. Must be unique if provided.

mesh (`string`_)
    Default: ````. The mesh to use for the object.

objectFlags (`number`_)
    Default: ``0``. The object flags initially set. Force set as modified.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
