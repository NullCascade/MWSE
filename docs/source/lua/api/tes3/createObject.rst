tes3.createObject
====================================================================================================

Create an object and returns it. The created object will be part of the saved game. Supported object types are those that have their own create function, such as tes3activator for example.

Returns
----------------------------------------------------------------------------------------------------

table (`tes3BaseObject`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

objectType (`number`_)
    Maps to tes3.objectType constants. Used to filter object type to create.

.. _`number`: ../../../lua/type/number.html
.. _`tes3BaseObject`: ../../../lua/type/tes3BaseObject.html
