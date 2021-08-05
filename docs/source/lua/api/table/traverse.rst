table.traverse
====================================================================================================

This function is used to iterate over values and its subtables in the table.

Each "node" is an object with a children table of other "nodes", each of which might have their own children. For example, a sceneNode is made up of niNodes, and each niNodes can have a list of niNode children. This is best used for recursive data structures like UI elements and sceneNodes etc.

Returns
----------------------------------------------------------------------------------------------------

result (`iterator`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

t (`table`_)
    A table to transverse.

k (`unknown`_)
    Default: ``children``. The key of a table inside t object.

.. _`iterator`: ../../../lua/type/iterator.html
.. _`table`: ../../../lua/type/table.html
.. _`unknown`: ../../../lua/type/unknown.html
