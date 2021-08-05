tes3race
====================================================================================================

A core object representing a character race.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`abilities <tes3race/abilities.html>`_ (`tes3spellList`_)
    A list of abilities that NPCs of that race have access to. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, ability in pairs(race.abilities.iterator) do print(ability.name) end

`baseAttributes <tes3race/baseAttributes.html>`_ (`table`_)
    Array-style table access to base 8 attributes for the race. Each element in the array is a tes3raceBaseAttribute.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`description <tes3race/description.html>`_ (`string`_)
    Loads from disk and returns the description of the race.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`femaleBody <tes3race/femaleBody.html>`_ (`tes3raceBodyParts`_)
    Access to all the body parts that will be used for female actors of this race.

`flags <tes3race/flags.html>`_ (`number`_)
    Raw bit-based flags.

`height <tes3race/height.html>`_ (`tes3raceHeightWeight`_)
    Access to the the height pair for males/females of the race.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`isBeast <tes3race/isBeast.html>`_ (`boolean`_)
    Access to the beast race flag.

`isPlayable <tes3race/isPlayable.html>`_ (`boolean`_)
    Access to the playable race flag.

`maleBody <tes3race/maleBody.html>`_ (`tes3raceBodyParts`_)
    Access to all the body parts that will be used for male actors of this race.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3race/name.html>`_ (`string`_)
    The player-facing name for the object.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`skillBonuses <tes3race/skillBonuses.html>`_ (`table`_)
    Array-style table access for 7 skill bonuses for the race. Each element in the array is a tes3raceSkillBonus.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`weight <tes3race/weight.html>`_ (`tes3raceHeightWeight`_)
    Access to the the height pair for males/females of the race.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3race/abilities
    tes3race/baseAttributes
    tes3race/description
    tes3race/femaleBody
    tes3race/flags
    tes3race/height
    tes3race/isBeast
    tes3race/isPlayable
    tes3race/maleBody
    tes3race/name
    tes3race/skillBonuses
    tes3race/weight

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.



.. toctree::
    :hidden:
    :maxdepth: 1


.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3raceBodyParts`: ../../lua/type/tes3raceBodyParts.html
.. _`tes3raceHeightWeight`: ../../lua/type/tes3raceHeightWeight.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
