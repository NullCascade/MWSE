tes3faction
====================================================================================================

A faction game object.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`attributes <tes3faction/attributes.html>`_ (`table`_)
    An array-style table holding the two attributes that govern advancement.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3faction/name.html>`_ (`string`_)
    The faction's player-facing name.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`playerExpelled <tes3faction/playerExpelled.html>`_ (`boolean`_)
    The player's expelled state in the faction.

`playerJoined <tes3faction/playerJoined.html>`_ (`boolean`_)
    The player's join state for the faction.

`playerRank <tes3faction/playerRank.html>`_ (`number`_)
    The player's current rank in the faction.

`playerReputation <tes3faction/playerReputation.html>`_ (`number`_)
    The player's current reputation in the faction.

`ranks <tes3faction/ranks.html>`_ (`table`_)
    An array-style table holding the ten related tes3factionRanks.

`reactions <tes3faction/reactions.html>`_ (`tes3iterator`_)
    A collection of tes3factionReactions.

`skills <tes3faction/skills.html>`_ (`table`_)
    An array-style table holding the seven skills that govern advancement.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3faction/attributes
    tes3faction/name
    tes3faction/playerExpelled
    tes3faction/playerJoined
    tes3faction/playerRank
    tes3faction/playerReputation
    tes3faction/ranks
    tes3faction/reactions
    tes3faction/skills

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`getRankName <tes3faction/getRankName.html>`_ (`method`_)
    This function fetches the player-facing name of a rank in the faction.

`setRankName <tes3faction/setRankName.html>`_ (`method`_)
    This function changes the player-facing name of a given rank in the faction.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3faction/getRankName
    tes3faction/setRankName

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
