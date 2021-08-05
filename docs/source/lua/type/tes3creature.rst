tes3creature
====================================================================================================

A creature object that has not been cloned. Typically represents the raw information edited in the construction set.

This type inherits from the following parent types: `tes3actor`_, `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`actorFlags <tes3actor/actorFlags.html>`_ (`number`_)
    A number representing the actor flags. Truly a bit field.

`aiConfig <tes3creature/aiConfig.html>`_ (`tes3aiConfig`_)
    A substructure off of actors that contains information on the current AI configuration.

`attacks <tes3creature/attacks.html>`_ (`table`_)
    A table of three attacks, represented by a trio of tes3rangeInt.

`attributes <tes3creature/attributes.html>`_ (`table`_)
    A table of eight numbers, representing the base values for the actor's attributes.

`barterGold <tes3actor/barterGold.html>`_ (`number`_)
    Friendly access to actor's barter gold amount.

`biped <tes3creature/biped.html>`_ (`boolean`_)
    Access to the creature's biped flag.

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

`fatigue <tes3creature/fatigue.html>`_ (`number`_)
    The actor's max fatigue.

`flies <tes3creature/flies.html>`_ (`boolean`_)
    Access to the creature's flies flag.

`health <tes3creature/health.html>`_ (`number`_)
    The actor's max health.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`inventory <tes3actor/inventory.html>`_ (`tes3iterator`_)
    The items currently carried by the actor.

`isAttacked <tes3creature/isAttacked.html>`_ (`boolean`_)
    If true, the actor's attacked flag is set.

`isEssential <tes3creature/isEssential.html>`_ (`boolean`_)
    If true, the actor's essential flag is set.

`isInstance <tes3creature/isInstance.html>`_ (`boolean`_)
    Always returns false.

`isRespawn <tes3creature/isRespawn.html>`_ (`boolean`_)
    If true, the actor's respawn flag is set.

`level <tes3creature/level.html>`_ (`number`_)
    The base level of the creature.

`magicka <tes3creature/magicka.html>`_ (`number`_)
    The actor's max magicka.

`mesh <tes3creature/mesh.html>`_ (`string`_)
    The path to the object's mesh.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3creature/name.html>`_ (`string`_)
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

`respawns <tes3creature/respawns.html>`_ (`boolean`_)
    Access to the creature's respawns flag.

`scale <tes3object/scale.html>`_ (`number`_)
    The object's scale.

`sceneNode <tes3object/sceneNode.html>`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference <tes3object/sceneReference.html>`_ (`niNode`_)
    The scene graph reference node for this object.

`script <tes3creature/script.html>`_ (`tes3script`_)
    The script that runs on the object.

`skills <tes3creature/skills.html>`_ (`table`_)
    A table of three numbers, representing the base values for the creature's combat, magic, and stealth skills.

`soul <tes3creature/soul.html>`_ (`number`_)
    The amount of soul value that the creature provides.

`soundCreature <tes3creature/soundCreature.html>`_ (`tes3creature`_)
    A creature to use instead of this one for sound generation.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`spells <tes3creature/spells.html>`_ (`tes3spellList`_)
    A list of spells that the actor has access to. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(creature.spells.iterator) do print(spell.name) end

`stolenList <tes3physicalObject/stolenList.html>`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`swims <tes3creature/swims.html>`_ (`boolean`_)
    Access to the creature's swims flag.

`type <tes3creature/type.html>`_ (`number`_)
    The type of the creature, represented by a number for normal, daedra, undead, or humanoid.

`usesEquipment <tes3creature/usesEquipment.html>`_ (`boolean`_)
    Access to the creature's usesEquipment flag.

`walks <tes3creature/walks.html>`_ (`boolean`_)
    Access to the creature's walks flag.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3creature/aiConfig
    tes3creature/attacks
    tes3creature/attributes
    tes3creature/biped
    tes3creature/fatigue
    tes3creature/flies
    tes3creature/health
    tes3creature/isAttacked
    tes3creature/isEssential
    tes3creature/isInstance
    tes3creature/isRespawn
    tes3creature/level
    tes3creature/magicka
    tes3creature/mesh
    tes3creature/name
    tes3creature/respawns
    tes3creature/script
    tes3creature/skills
    tes3creature/soul
    tes3creature/soundCreature
    tes3creature/spells
    tes3creature/swims
    tes3creature/type
    tes3creature/usesEquipment
    tes3creature/walks

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
.. _`tes3creature`: ../../lua/type/tes3creature.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3script`: ../../lua/type/tes3script.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
