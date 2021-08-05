tes3leveledItem
====================================================================================================

A leveled creature game object.

This type inherits from the following parent types: `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`boundingBox <tes3physicalObject/boundingBox.html>`_ (`tes3boundingBox`_)
    The bounding box for the object.

`calculateForEachItem <tes3leveledItem/calculateForEachItem.html>`_ (`boolean`_)
    If true, each entry is rolled in the leveled list when resolving items. This can result in multiple items from one list being added.

`calculateFromAllLevels <tes3leveledItem/calculateFromAllLevels.html>`_ (`boolean`_)
    If true, the leveled list is picked from the entire level range, instead of just the highest.

`chanceForNothing <tes3leveledItem/chanceForNothing.html>`_ (`number`_)
    The percent chance, from 0 to 100, for no object to be chosen.

`count <tes3leveledItem/count.html>`_ (`number`_)
    The number of possible options in the leveled object container.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`flags <tes3leveledItem/flags.html>`_ (`number`_)
    A numerical representation of bit flags for the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`list <tes3leveledItem/list.html>`_ (`tes3iterator`_)
    The collection that itself, containing tes3leveledListNodes.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`nextInCollection <tes3object/nextInCollection.html>`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`owningCollection <tes3object/owningCollection.html>`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

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

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList <tes3physicalObject/stolenList.html>`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3leveledItem/calculateForEachItem
    tes3leveledItem/calculateFromAllLevels
    tes3leveledItem/chanceForNothing
    tes3leveledItem/count
    tes3leveledItem/flags
    tes3leveledItem/list

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`insert <tes3leveledItem/insert.html>`_ (`method`_)
    This function inserts given item in the leveled list. True is returned if the operation was successful.

`pickFrom <tes3leveledItem/pickFrom.html>`_ (`method`_)
    Chooses a random item from the list, based on the player's level.

`remove <tes3leveledItem/remove.html>`_ (`method`_)
    This function removes an item from the leveled list. True is returned if the operation was successful.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3leveledItem/insert
    tes3leveledItem/pickFrom
    tes3leveledItem/remove

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
