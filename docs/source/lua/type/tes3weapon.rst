tes3weapon
====================================================================================================

A weapon game object.

This type inherits from the following parent types: `tes3item`_, `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`boundingBox <tes3physicalObject/boundingBox.html>`_ (`tes3boundingBox`_)
    The bounding box for the object.

`chopMax <tes3weapon/chopMax.html>`_ (`number`_)
    Chop damage inflicted at maximum weapon swing.

`chopMin <tes3weapon/chopMin.html>`_ (`number`_)
    Chop damage inflicted at minimum weapon swing.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`enchantCapacity <tes3weapon/enchantCapacity.html>`_ (`number`_)
    The object's enchantment capacity.

`enchantment <tes3weapon/enchantment.html>`_ (`tes3enchantment`_)
    The enchantment used by the object.

`flags <tes3weapon/flags.html>`_ (`number`_)
    Access to the weapon flag bitfield. Known flags can be accessed via other weapon properties.

`hasDurability <tes3weapon/hasDurability.html>`_ (`boolean`_)
    Whether the weapon type has durability. Thrown weapons and ammunition are used once fired, and therefore do not use durability.

`icon <tes3weapon/icon.html>`_ (`string`_)
    The path to the object's icon.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`ignoresNormalWeaponResistance <tes3weapon/ignoresNormalWeaponResistance.html>`_ (`boolean`_)
    Access to the flag that controls if this weapon bypasses the "Resist normal weapons" magic effect.

`isAmmo <tes3weapon/isAmmo.html>`_ (`boolean`_)
    If true, the weapon is a type of ammunition.

`isMelee <tes3weapon/isMelee.html>`_ (`boolean`_)
    If true, the weapon has a melee weapon type.

`isOneHanded <tes3weapon/isOneHanded.html>`_ (`boolean`_)
    If true, the weapon is a one-handed weapon.

`isProjectile <tes3weapon/isProjectile.html>`_ (`boolean`_)
    If true, the weapon is a projectile.

`isRanged <tes3weapon/isRanged.html>`_ (`boolean`_)
    If true, the weapon is a ranged weapon.

`isSilver <tes3weapon/isSilver.html>`_ (`boolean`_)
    If true, the weapon has a silver material flag.

`isTwoHanded <tes3weapon/isTwoHanded.html>`_ (`boolean`_)
    If true, the weapon is a two-handed melee weapon.

`maxCondition <tes3weapon/maxCondition.html>`_ (`number`_)
    The object's maximum condition.

`mesh <tes3weapon/mesh.html>`_ (`string`_)
    The path to the object's mesh.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3weapon/name.html>`_ (`string`_)
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

`reach <tes3weapon/reach.html>`_ (`number`_)
    The weapon's relative reach. A value of 1.0 is the standard reach. It is multiplied by the GMST fCombatDistance to find the attack reach in world units.

`scale <tes3object/scale.html>`_ (`number`_)
    The object's scale.

`sceneNode <tes3object/sceneNode.html>`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference <tes3object/sceneReference.html>`_ (`niNode`_)
    The scene graph reference node for this object.

`script <tes3weapon/script.html>`_ (`tes3script`_)
    The script that runs on the object.

`skill <tes3weapon/skill.html>`_ (`tes3skill`_)
    Gets the skill data for the attack skill used by the weapon.

`skillId <tes3weapon/skillId.html>`_ (`number`_)
    Gets the skill ID for the attack skill used by the weapon.

`slashMax <tes3weapon/slashMax.html>`_ (`number`_)
    Slash damage inflicted at maximum weapon swing.

`slashMin <tes3weapon/slashMin.html>`_ (`number`_)
    Slash damage inflicted at minimum weapon swing.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`speed <tes3weapon/speed.html>`_ (`number`_)
    The relative attack speed of the weapon. Controls attack timing and animation speed. Standard speed is 1.0.

`stolenList <tes3physicalObject/stolenList.html>`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`thrustMax <tes3weapon/thrustMax.html>`_ (`number`_)
    Thrust damage inflicted at maximum weapon swing.

`thrustMin <tes3weapon/thrustMin.html>`_ (`number`_)
    Thrust damage inflicted at minimum weapon swing.

`type <tes3weapon/type.html>`_ (`number`_)
    A number representing the weapon type. Matches values from the tes3.weaponType table.

`typeName <tes3weapon/typeName.html>`_ (`string`_)
    The player-facing name for the weapon type.

`value <tes3weapon/value.html>`_ (`number`_)
    The value of the object.

`weight <tes3weapon/weight.html>`_ (`number`_)
    The weight of the object.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3weapon/chopMax
    tes3weapon/chopMin
    tes3weapon/enchantCapacity
    tes3weapon/enchantment
    tes3weapon/flags
    tes3weapon/hasDurability
    tes3weapon/icon
    tes3weapon/ignoresNormalWeaponResistance
    tes3weapon/isAmmo
    tes3weapon/isMelee
    tes3weapon/isOneHanded
    tes3weapon/isProjectile
    tes3weapon/isRanged
    tes3weapon/isSilver
    tes3weapon/isTwoHanded
    tes3weapon/maxCondition
    tes3weapon/mesh
    tes3weapon/name
    tes3weapon/reach
    tes3weapon/script
    tes3weapon/skill
    tes3weapon/skillId
    tes3weapon/slashMax
    tes3weapon/slashMin
    tes3weapon/speed
    tes3weapon/thrustMax
    tes3weapon/thrustMin
    tes3weapon/type
    tes3weapon/typeName
    tes3weapon/value
    tes3weapon/weight

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`getMaterialFlag <tes3weapon/getMaterialFlag.html>`_ (`method`_)
    Access to the material flag bitfield. Known material flags can be accessed via other weapon properties.

`setMaterialFlag <tes3weapon/setMaterialFlag.html>`_ (`method`_)
    Access to the material flag bitfield. Known material flags can be accessed via other weapon properties.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3weapon/getMaterialFlag
    tes3weapon/setMaterialFlag

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3script`: ../../lua/type/tes3script.html
.. _`tes3skill`: ../../lua/type/tes3skill.html
