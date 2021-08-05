tes3dialogue
====================================================================================================

A parent-level dialogue, such as a topic, voice, greeting, persuasion response, or journal.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`info <tes3dialogue/info.html>`_ (`tes3iterator`_)
    A collection of individual entries in the dialogue.

`journalIndex <tes3dialogue/journalIndex.html>`_ (`number`_)
    For journal dialogues, the currently active journal index.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`type <tes3dialogue/type.html>`_ (`number`_)
    The type of the dialogue.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3dialogue/info
    tes3dialogue/journalIndex
    tes3dialogue/type

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`addToJournal <tes3dialogue/addToJournal.html>`_ (`method`_)
    Adds the dialogue to the player's journal, if applicable, at a given index.

`getInfo <tes3dialogue/getInfo.html>`_ (`method`_)
    Fetches the info that a given actor would produce for the dialogue.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3dialogue/addToJournal
    tes3dialogue/getInfo

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
