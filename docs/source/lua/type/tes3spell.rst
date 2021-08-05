tes3spell
====================================================================================================

A spell game object.

This type inherits from the following parent types: `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`alwaysSucceeds <tes3spell/alwaysSucceeds.html>`_ (`boolean`_)
    A flag that determines if casting the spell will always succeed.

`autoCalc <tes3spell/autoCalc.html>`_ (`boolean`_)
    Determines if the magicka cost for the spell is autocalculated, and if the spell may be automatically assigned to NPCs if they are skillful enough to cast it.

`basePurchaseCost <tes3spell/basePurchaseCost.html>`_ (`number`_)
    Calculates the base cost to purchase the spell. This value is not adjusted by mercantile skills.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`castType <tes3spell/castType.html>`_ (`number`_)
    The spell's cast type.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`effects <tes3spell/effects.html>`_ (`table`_)
    An array-style table of the tes3effect data on the object.

`flags <tes3spell/flags.html>`_ (`number`_)
    A bit field for the spell's flags.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`magickaCost <tes3spell/magickaCost.html>`_ (`number`_)
    No description available.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3spell/name.html>`_ (`string`_)
    The name of the spell.

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

`playerStart <tes3spell/playerStart.html>`_ (`boolean`_)
    A flag that determines if the spell may be assigned to the player at character generation if the player has enough skill to cast it.

`previousInCollection <tes3object/previousInCollection.html>`_ (`tes3object`_)
    The previous object in parent collection's list.

`scale <tes3object/scale.html>`_ (`number`_)
    The object's scale.

`sceneNode <tes3object/sceneNode.html>`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference <tes3object/sceneReference.html>`_ (`niNode`_)
    The scene graph reference node for this object.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3spell/alwaysSucceeds
    tes3spell/autoCalc
    tes3spell/basePurchaseCost
    tes3spell/castType
    tes3spell/effects
    tes3spell/flags
    tes3spell/magickaCost
    tes3spell/name
    tes3spell/playerStart

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.



.. toctree::
    :hidden:
    :maxdepth: 1


Functions
----------------------------------------------------------------------------------------------------

`calculateCastChance <tes3spell/calculateCastChance.html>`_ (`function`_)
    Calculates the chance that a caster can cast a given spell.

`create <tes3spell/create.html>`_ (`function`_)
    Creates a new spell object, which will be stored as part of the current saved game.

`getActiveEffectCount <tes3spell/getActiveEffectCount.html>`_ (`function`_)
    Gets the number of active effects in the spell effect table.

`getFirstIndexOfEffect <tes3spell/getFirstIndexOfEffect.html>`_ (`function`_)
    Gets the first index of an effect ID in the spell effect table.

`getLeastProficientEffect <tes3spell/getLeastProficientEffect.html>`_ (`function`_)
    Returns the effect of the spell that a given actor is least proficient with.

`getLeastProficientSchool <tes3spell/getLeastProficientSchool.html>`_ (`function`_)
    Returns the school of the least proficient effect on the spell, for a given actor.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3spell/calculateCastChance
    tes3spell/create
    tes3spell/getActiveEffectCount
    tes3spell/getFirstIndexOfEffect
    tes3spell/getLeastProficientEffect
    tes3spell/getLeastProficientSchool

.. _`boolean`: ../../lua/type/boolean.html
.. _`function`: ../../lua/type/function.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
