tes3quest
====================================================================================================

A representation of a quest, with associated dialogue and info.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`dialogue <tes3quest/dialogue.html>`_ (`tes3iterator`_)
    A collection of dialogues associated with the quest.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`info <tes3quest/info.html>`_ (`tes3iterator`_)
    A collection of dialogue info associated with the quest.

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



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3quest/dialogue
    tes3quest/info

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
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
