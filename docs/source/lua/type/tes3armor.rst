tes3armor
====================================================================================================

An armor game object.

This type inherits from the following parent types: `tes3item`_, `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`armorRating <tes3armor/armorRating.html>`_ (`number`_)
    The armor's defensive rating.

`armorScalar <tes3armor/armorScalar.html>`_ (`number`_)
    No description available.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`boundingBox <tes3physicalObject/boundingBox.html>`_ (`tes3boundingBox`_)
    The bounding box for the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`enchantCapacity <tes3armor/enchantCapacity.html>`_ (`number`_)
    The object's enchantment capacity.

`enchantment <tes3armor/enchantment.html>`_ (`tes3enchantment`_)
    The enchantment used by the object.

`icon <tes3armor/icon.html>`_ (`string`_)
    The path to the object's icon.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`isLeftPart <tes3armor/isLeftPart.html>`_ (`boolean`_)
    Determines if the armor is the left part of a pair.

`maxCondition <tes3armor/maxCondition.html>`_ (`number`_)
    The object's maximum condition.

`mesh <tes3armor/mesh.html>`_ (`string`_)
    The path to the object's mesh.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3armor/name.html>`_ (`string`_)
    The player-facing name for the object.

`nextInCollection <tes3object/nextInCollection.html>`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`owningCollection <tes3object/owningCollection.html>`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`parts <tes3armor/parts.html>`_ (`table`_)
    An array-style table of the tes3wearablePart data on the object.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`previousInCollection <tes3object/previousInCollection.html>`_ (`tes3object`_)
    The previous object in parent collection's list.

`scale <tes3object/scale.html>`_ (`number`_)
    The object's scale.

`sceneNode <tes3object/sceneNode.html>`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference <tes3object/sceneReference.html>`_ (`niNode`_)
    The scene graph reference node for this object.

`script <tes3armor/script.html>`_ (`tes3script`_)
    The script that runs on the object.

`slot <tes3armor/slot.html>`_ (`number`_)
    The slot used by the armor.

`slotName <tes3armor/slotName.html>`_ (`string`_)
    The name of the slot used by the armor.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList <tes3physicalObject/stolenList.html>`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`value <tes3armor/value.html>`_ (`number`_)
    The value of the object.

`weight <tes3armor/weight.html>`_ (`number`_)
    The weight of the object.

`weightClass <tes3armor/weightClass.html>`_ (`number`_)
    The weight class of the armor. This value maps to tes3.armorWeightClass.* constants.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3armor/armorRating
    tes3armor/armorScalar
    tes3armor/enchantCapacity
    tes3armor/enchantment
    tes3armor/icon
    tes3armor/isLeftPart
    tes3armor/maxCondition
    tes3armor/mesh
    tes3armor/name
    tes3armor/parts
    tes3armor/script
    tes3armor/slot
    tes3armor/slotName
    tes3armor/value
    tes3armor/weight
    tes3armor/weightClass

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`calculateArmorRating <tes3armor/calculateArmorRating.html>`_ (`method`_)
    Calculates what armor rating is provided for a given mobile actor.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3armor/calculateArmorRating

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3script`: ../../lua/type/tes3script.html
