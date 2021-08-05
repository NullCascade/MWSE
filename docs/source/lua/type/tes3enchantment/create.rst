create
====================================================================================================

Creates a new enchantment object, which will be stored as part of the current saved game.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`string`_)
    Optional. The new object's ID. Must be unique if provided.

castType (`number`_)
    The enchantment castType. See tes3.enchantmentType.

chargeCost (`number`_)
    The new enchantment charge cost. Must be greater than 0.

maxCharge (`number`_)
    The new enchantment maximum charge. Must be greater than 0

flags (`number`_)
    Optional. The new enchantment flags.

objectFlags (`number`_)
    Default: ``0``. The object flags initially set. Force set as modified.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
