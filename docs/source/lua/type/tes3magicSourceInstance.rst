tes3magicSourceInstance
====================================================================================================

A game structure that keeps track of a magic source on an object.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`castChanceOverride <tes3magicSourceInstance/castChanceOverride.html>`_ (`number`_)
    No description available.

`caster <tes3magicSourceInstance/caster.html>`_ (`tes3reference`_)
    No description available.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`item <tes3magicSourceInstance/item.html>`_ (`tes3item`_)
    No description available.

`itemData <tes3magicSourceInstance/itemData.html>`_ (`tes3itemData`_)
    No description available.

`itemID <tes3magicSourceInstance/itemID.html>`_ (`string`_)
    No description available.

`magicID <tes3magicSourceInstance/magicID.html>`_ (`string`_)
    No description available.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`projectile <tes3magicSourceInstance/projectile.html>`_ (`tes3mobileProjectile`_)
    No description available.

`serialNumber <tes3magicSourceInstance/serialNumber.html>`_ (`number`_)
    Unique indentifier for just this magic source.

`source <tes3magicSourceInstance/source.html>`_ (`tes3alchemy`_, `tes3enchantment`_, `tes3spell`_)
    No description available.

`sourceEffects <tes3magicSourceInstance/sourceEffects.html>`_ (`table`_)
    An array-style table holding spell effects this magic source has.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`sourceType <tes3magicSourceInstance/sourceType.html>`_ (`number`_)
    The type of this magic source. Maps to tes3.magicSourceType.* constants.

`state <tes3magicSourceInstance/state.html>`_ (`number`_)
    Shows if the state is pre-cast, cast, beginning, working, ending, retired, etc.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`target <tes3magicSourceInstance/target.html>`_ (`tes3reference`_)
    No description available.

`timestampCastBegin <tes3magicSourceInstance/timestampCastBegin.html>`_ (`number`_)
    No description available.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3magicSourceInstance/castChanceOverride
    tes3magicSourceInstance/caster
    tes3magicSourceInstance/item
    tes3magicSourceInstance/itemData
    tes3magicSourceInstance/itemID
    tes3magicSourceInstance/magicID
    tes3magicSourceInstance/projectile
    tes3magicSourceInstance/serialNumber
    tes3magicSourceInstance/source
    tes3magicSourceInstance/sourceEffects
    tes3magicSourceInstance/sourceType
    tes3magicSourceInstance/state
    tes3magicSourceInstance/target
    tes3magicSourceInstance/timestampCastBegin

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`getEffectInstance <tes3magicSourceInstance/getEffectInstance.html>`_ (`method`_)
    Gets the magic effect instance for a given effect index and target reference. Effect instances may not all be active on a target, due to reflect, absorption, dispels, different durations and other factors.

`getMagnitudeForIndex <tes3magicSourceInstance/getMagnitudeForIndex.html>`_ (`method`_)
    Gets the magnitude from the casting source for a given effect index.

`playVisualEffect <tes3magicSourceInstance/playVisualEffect.html>`_ (`method`_)
    This function plays an effect from tes3magicSourceInstance of a given index.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3magicSourceInstance/getEffectInstance
    tes3magicSourceInstance/getMagnitudeForIndex
    tes3magicSourceInstance/playVisualEffect

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3alchemy`: ../../lua/type/tes3alchemy.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
