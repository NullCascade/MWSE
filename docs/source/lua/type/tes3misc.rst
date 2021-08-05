tes3misc
====================================================================================================

A core miscellaneous object.

This type inherits from the following parent types: `tes3item`_, `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`boundingBox <tes3physicalObject/boundingBox.html>`_ (`tes3boundingBox`_)
    The bounding box for the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`icon <tes3misc/icon.html>`_ (`string`_)
    The path to the object's icon.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`isKey <tes3misc/isKey.html>`_ (`boolean`_)
    Access to the flag determining if this item is recognized as a key.

`isSoulGem <tes3misc/isSoulGem.html>`_ (`boolean`_)
    Determines if this item is a soul gem.

`mesh <tes3misc/mesh.html>`_ (`string`_)
    The path to the object's mesh.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3misc/name.html>`_ (`string`_)
    The player-facing name for the object.

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

`script <tes3misc/script.html>`_ (`tes3script`_)
    The script that runs on the object.

`soulGemCapacity <tes3misc/soulGemCapacity.html>`_ (`number`_)
    Fetches capacity of the soul gem, if this item is a soul gem.

`soulGemData <tes3misc/soulGemData.html>`_ (`tes3soulGemData`_)
    Fetches related soul gem data, if this item is a soul gem.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList <tes3physicalObject/stolenList.html>`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`value <tes3misc/value.html>`_ (`number`_)
    The value of the object.

`weight <tes3misc/weight.html>`_ (`number`_)
    The weight of the object.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3misc/icon
    tes3misc/isKey
    tes3misc/isSoulGem
    tes3misc/mesh
    tes3misc/name
    tes3misc/script
    tes3misc/soulGemCapacity
    tes3misc/soulGemData
    tes3misc/value
    tes3misc/weight

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`isGold <tes3misc/isGold.html>`_ (`method`_)
    Returns true if the misc item is gold.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3misc/isGold

Functions
----------------------------------------------------------------------------------------------------

`create <tes3misc/create.html>`_ (`function`_)
    Creates a new misc item object, which will be stored as part of the current saved game.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3misc/create

.. _`boolean`: ../../lua/type/boolean.html
.. _`function`: ../../lua/type/function.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3script`: ../../lua/type/tes3script.html
.. _`tes3soulGemData`: ../../lua/type/tes3soulGemData.html
