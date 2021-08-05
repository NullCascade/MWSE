tes3npcInstance
====================================================================================================

An NPC object that has been cloned. Typically represents an NPC that has been instanced in the world.

This type inherits from the following parent types: `tes3actor`_, `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`actorFlags <tes3actor/actorFlags.html>`_ (`number`_)
    A number representing the actor flags. Truly a bit field.

`aiConfig <tes3npcInstance/aiConfig.html>`_ (`tes3aiConfig`_)
    A substructure off of actors that contains information on the current AI configuration.

`attributes <tes3npcInstance/attributes.html>`_ (`table`_)
    Quick access to the base NPC's attributes.

`barterGold <tes3npcInstance/barterGold.html>`_ (`number`_)
    Quick access to the base NPC's base amount of barter gold.

`baseObject <tes3npcInstance/baseObject.html>`_ (`tes3npc`_)
    Access to the base NPC object.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`blood <tes3actor/blood.html>`_ (`number`_)
    Friendly access to actor's blood type.

`boundingBox <tes3physicalObject/boundingBox.html>`_ (`tes3boundingBox`_)
    The bounding box for the object.

`class <tes3npcInstance/class.html>`_ (`tes3class`_)
    Quick access to the base NPC's class.

`cloneCount <tes3actor/cloneCount.html>`_ (`number`_)
    The number of clones that exist of this actor.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`disposition <tes3npcInstance/disposition.html>`_ (`number`_)
    The actor's effective disposition, with all modifiers applied.

`equipment <tes3actor/equipment.html>`_ (`tes3iterator`_)
    The items currently equipped to the actor.

`faction <tes3npcInstance/faction.html>`_ (`tes3faction`_)
    Quick access to the base NPC's faction.

`factionIndex <tes3npcInstance/factionIndex.html>`_ (`number`_)
    No description available.

`fatigue <tes3npcInstance/fatigue.html>`_ (`number`_)
    Quick access to the base NPC's fatigue.

`health <tes3npcInstance/health.html>`_ (`number`_)
    Quick access to the base NPC's health.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`inventory <tes3actor/inventory.html>`_ (`tes3iterator`_)
    The items currently carried by the actor.

`isAttacked <tes3npcInstance/isAttacked.html>`_ (`boolean`_)
    If true, the actor's attacked flag is set.

`isEssential <tes3npcInstance/isEssential.html>`_ (`boolean`_)
    If true, the actor's essential flag is set.

`isInstance <tes3npcInstance/isInstance.html>`_ (`boolean`_)
    Always returns true.

`isRespawn <tes3npcInstance/isRespawn.html>`_ (`boolean`_)
    If true, the actor's respawn flag is set.

`level <tes3npcInstance/level.html>`_ (`number`_)
    Quick access to the base NPC's level.

`magicka <tes3npcInstance/magicka.html>`_ (`number`_)
    Quick access to the base NPC's magicka.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3npcInstance/name.html>`_ (`number`_)
    Quick access to the base NPC's name.

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

`race <tes3npcInstance/race.html>`_ (`tes3race`_)
    Quick access to the base NPC's race.

`scale <tes3object/scale.html>`_ (`number`_)
    The object's scale.

`sceneNode <tes3object/sceneNode.html>`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference <tes3object/sceneReference.html>`_ (`niNode`_)
    The scene graph reference node for this object.

`script <tes3npcInstance/script.html>`_ (`tes3script`_)
    Quick access to the base NPC's script.

`skills <tes3npcInstance/skills.html>`_ (`table`_)
    Quick access to the base NPC's skills.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`spells <tes3npcInstance/spells.html>`_ (`tes3spellList`_)
    Quick access to the base NPC's spell list. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(npc.spells.iterator) do print(spell.name) end

`stolenList <tes3physicalObject/stolenList.html>`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3npcInstance/aiConfig
    tes3npcInstance/attributes
    tes3npcInstance/barterGold
    tes3npcInstance/baseObject
    tes3npcInstance/class
    tes3npcInstance/disposition
    tes3npcInstance/faction
    tes3npcInstance/factionIndex
    tes3npcInstance/fatigue
    tes3npcInstance/health
    tes3npcInstance/isAttacked
    tes3npcInstance/isEssential
    tes3npcInstance/isInstance
    tes3npcInstance/isRespawn
    tes3npcInstance/level
    tes3npcInstance/magicka
    tes3npcInstance/name
    tes3npcInstance/race
    tes3npcInstance/script
    tes3npcInstance/skills
    tes3npcInstance/spells

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


.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3actor`: ../../lua/type/tes3actor.html
.. _`tes3aiConfig`: ../../lua/type/tes3aiConfig.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3class`: ../../lua/type/tes3class.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3race`: ../../lua/type/tes3race.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3script`: ../../lua/type/tes3script.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
