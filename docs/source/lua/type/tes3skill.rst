tes3skill
====================================================================================================

A skill object.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`actions <tes3skill/actions.html>`_ (`table`_)
    A table with 4 values. The amount of experience an action (use of skill) awards.

`attribute <tes3skill/attribute.html>`_ (`number`_)
    Skill's governing attribute. Maps to tes3.attribute.* constants.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`iconPath <tes3skill/iconPath.html>`_ (`string`_)
    The path to the icon for the skill.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3skill/name.html>`_ (`string`_)
    The player-facing name of the skill.

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

`specialization <tes3skill/specialization.html>`_ (`number`_)
    The specialization in which the skill belongs. Maps to tes3.specialization.* constants.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3skill/actions
    tes3skill/attribute
    tes3skill/iconPath
    tes3skill/name
    tes3skill/specialization

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
