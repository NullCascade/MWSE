tes3actor
====================================================================================================

An Actor (not to be confused with a Mobile Actor) is an object that can be cloned and that has an inventory. Creatures, NPCs, and containers are all considered actors.

It is standard for creatures and NPCs to be composed of an actor and a mobile actor, linked together with a reference.

This type inherits from the following parent types: `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`actorFlags <tes3actor/actorFlags.html>`_ (`number`_)
    A number representing the actor flags. Truly a bit field.

`barterGold <tes3actor/barterGold.html>`_ (`number`_)
    Friendly access to actor's barter gold amount.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`blood <tes3actor/blood.html>`_ (`number`_)
    Friendly access to actor's blood type.

`boundingBox <tes3physicalObject/boundingBox.html>`_ (`tes3boundingBox`_)
    The bounding box for the object.

`cloneCount <tes3actor/cloneCount.html>`_ (`number`_)
    The number of clones that exist of this actor.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`equipment <tes3actor/equipment.html>`_ (`tes3iterator`_)
    The items currently equipped to the actor.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`inventory <tes3actor/inventory.html>`_ (`tes3iterator`_)
    The items currently carried by the actor.

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

    tes3actor/actorFlags
    tes3actor/barterGold
    tes3actor/blood
    tes3actor/cloneCount
    tes3actor/equipment
    tes3actor/inventory

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`offersService <tes3actor/offersService.html>`_ (`method`_)
    Checks if the actor will offer a service in dialogue. This an offer and may still be refused by dialogue checks. To also get the result of dialogue checks, use tes3.checkMerchantOffersService.

`onInventoryClose <tes3actor/onInventoryClose.html>`_ (`method`_)
    A callback function invoked when an inventory is closed. Typically not used outside of specific purposes. You may find tes3.reference's onCloseInventory() to be more convenient to use.

`tradesItemType <tes3actor/tradesItemType.html>`_ (`method`_)
    Checks if the actor will buy and sell items of a given object type. e.g. actor:tradesItemType(tes3.objectType.repairItem)



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3actor/offersService
    tes3actor/onInventoryClose
    tes3actor/tradesItemType

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
