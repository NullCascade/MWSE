tes3dialogueInfo
====================================================================================================

A child for a given dialogue. Whereas a dialogue may be a conversation topic, a tes3dialogueInfo would be an individual response.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`actor <tes3dialogueInfo/actor.html>`_ (`tes3actor`_)
    The speaker's actor that the info is filtered for.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`cell <tes3dialogueInfo/cell.html>`_ (`tes3cell`_)
    The speaker's current cell that the info is filtered for.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`disposition <tes3dialogueInfo/disposition.html>`_ (`number`_)
    The minimum disposition that the info is filtered for.

`firstHeardFrom <tes3dialogueInfo/firstHeardFrom.html>`_ (`tes3actor`_)
    The actor that the player first heard the info from.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`npcClass <tes3dialogueInfo/npcClass.html>`_ (`tes3class`_)
    The speaker's class that the info is filtered for.

`npcFaction <tes3dialogueInfo/npcFaction.html>`_ (`tes3faction`_)
    The speaker's faction that the info is filtered for.

`npcRace <tes3dialogueInfo/npcRace.html>`_ (`tes3actor`_)
    The speaker's race that the info is filtered for.

`npcRank <tes3dialogueInfo/npcRank.html>`_ (`number`_)
    The speaker's faction rank that the info is filtered for.

`npcSex <tes3dialogueInfo/npcSex.html>`_ (`number`_)
    The speaker's sex that the info is filtered for.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`pcFaction <tes3dialogueInfo/pcFaction.html>`_ (`number`_)
    The player's joined faction that the info is filtered for.

`pcRank <tes3dialogueInfo/pcRank.html>`_ (`number`_)
    The player's rank required rank in the speaker's faction.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`text <tes3dialogueInfo/text.html>`_ (`string`_)
    String contents for the info. This is not kept in memory, and must be loaded from files for each call.

`type <tes3dialogueInfo/type.html>`_ (`number`_)
    The type of the info.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3dialogueInfo/actor
    tes3dialogueInfo/cell
    tes3dialogueInfo/disposition
    tes3dialogueInfo/firstHeardFrom
    tes3dialogueInfo/npcClass
    tes3dialogueInfo/npcFaction
    tes3dialogueInfo/npcRace
    tes3dialogueInfo/npcRank
    tes3dialogueInfo/npcSex
    tes3dialogueInfo/pcFaction
    tes3dialogueInfo/pcRank
    tes3dialogueInfo/text
    tes3dialogueInfo/type

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`filter <tes3dialogueInfo/filter.html>`_ (`method`_)
    This method filters the associated dialogue info for given arguments and returns true if the dialogue info filtering passes. This method rises infoFilter event.

`runScript <tes3dialogueInfo/runScript.html>`_ (`method`_)
    This method runs the dialogue info's associated script on a given reference.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3dialogueInfo/filter
    tes3dialogueInfo/runScript

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3actor`: ../../lua/type/tes3actor.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3class`: ../../lua/type/tes3class.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
