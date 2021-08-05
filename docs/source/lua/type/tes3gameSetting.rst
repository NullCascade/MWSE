tes3gameSetting
====================================================================================================

A GMST game object.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`defaultValue <tes3gameSetting/defaultValue.html>`_ (`number`_, `string`_)
    The default value of the GMST, if no master defines the value.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`index <tes3gameSetting/index.html>`_ (`number`_)
    The array index for the GMST.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`rawValue <tes3gameSetting/rawValue.html>`_ (`number`_)
    The base value of the game setting, as stored in memory. Prefer to use .value unless you know what you're doing.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`type <tes3gameSetting/type.html>`_ (`string`_)
    The type of the variable, either 'i', 'f', or 's'.

`value <tes3gameSetting/value.html>`_ (`number`_, `string`_)
    The value of the GMST.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3gameSetting/defaultValue
    tes3gameSetting/index
    tes3gameSetting/rawValue
    tes3gameSetting/type
    tes3gameSetting/value

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
