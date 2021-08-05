tes3.getEquippedItem
====================================================================================================

Returns an actor's equipped item stack, provided a given filter

Returns
----------------------------------------------------------------------------------------------------

stack (`tes3equipmentStack`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

actor (`tes3reference`_, `tes3mobileActor`_, `tes3actor`_)
    No description available.

enchanted (`boolean`_)
    Optional. If true, filters to enchanted items.

objectType (`number`_)
    Optional. Maps to tes3.objectType constants. Used to filter equipment by type.

slot (`number`_)
    Optional. Maps to tes3.armorSlot or tes3.clothingSlot. Used to filter equipment by slot.

type (`number`_)
    Optional. Maps to tes3.weaponType. Used to filter equipment by type.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3actor`: ../../../lua/type/tes3actor.html
.. _`tes3equipmentStack`: ../../../lua/type/tes3equipmentStack.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
