tes3actionData
====================================================================================================

A substructure of mobile actors that provides information about the current or previous action.

Properties
----------------------------------------------------------------------------------------------------

`aiBehaviorState <tes3actionData/aiBehaviorState.html>`_ (`number`_)
    The behavior state of the tes3actionData.

`animationAttackState <tes3actionData/animationAttackState.html>`_ (`number`_)
    No description available.

`attackDirection <tes3actionData/attackDirection.html>`_ (`number`_)
    **Deprecated. Use tes3actionData.physicalAttackType instead.** When attacking, the direction swung with the weapon. This shows if the actor was thrusting, swinging, or chopping.

`attackSwing <tes3actionData/attackSwing.html>`_ (`number`_)
    When attacking, this value represents how much the weapon has been pulled back. The value ranges from [0.0 - 1.0].

`currentAnimationGroup <tes3actionData/currentAnimationGroup.html>`_ (`number`_)
    No description available.

`hitTarget <tes3actionData/hitTarget.html>`_ (`tes3mobileActor`_)
    No description available.

`lastBarterHoursPassed <tes3actionData/lastBarterHoursPassed.html>`_ (`number`_)
    No description available.

`nockedProjectile <tes3actionData/nockedProjectile.html>`_ (`tes3weapon`_)
    The currently knocked projectile the associated actor is using.

`physicalAttackType <tes3actionData/physicalAttackType.html>`_ (`number`_)
    A number from the tes3.physicalAttackType enumeration identifying the physical attack type. Can be tes3.physicalAttackType.slash, .chop, .thrust, .projectile, .creature1, .creature2, or .creature3.

`physicalDamage <tes3actionData/physicalDamage.html>`_ (`number`_)
    When attacking, this is the value of the weapon damage that was rolled. This value doesn't take into account the actor's strength, or other additional damage.

`stolenFrom <tes3actionData/stolenFrom.html>`_ (`tes3object`_)
    No description available.

`target <tes3actionData/target.html>`_ (`tes3mobileActor`_)
    No description available.

`walkDestination <tes3actionData/walkDestination.html>`_ (`tes3vector3`_)
    If moving to a location, this is the position to be walked to.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3actionData/aiBehaviorState
    tes3actionData/animationAttackState
    tes3actionData/attackDirection
    tes3actionData/attackSwing
    tes3actionData/currentAnimationGroup
    tes3actionData/hitTarget
    tes3actionData/lastBarterHoursPassed
    tes3actionData/nockedProjectile
    tes3actionData/physicalAttackType
    tes3actionData/physicalDamage
    tes3actionData/stolenFrom
    tes3actionData/target
    tes3actionData/walkDestination

.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
