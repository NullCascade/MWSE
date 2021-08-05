tes3mobileSpellProjectile
====================================================================================================

A mobile object for a spell projectile.

This type inherits from the following parent types: `tes3mobileProjectile`_, `tes3mobileObject`_

Properties
----------------------------------------------------------------------------------------------------

`animTime <tes3mobileProjectile/animTime.html>`_ (`number`_)
    The time index for the projectile model's animation controllers.

`attackSwing <tes3mobileProjectile/attackSwing.html>`_ (`number`_)
    The attack swing value of the attacker at the time of releasing the projectile.

`boundSize <tes3mobileObject/boundSize.html>`_ (`tes3vector3`_)
    A vector that shows the size of the bounding box in each direction.

`cellX <tes3mobileObject/cellX.html>`_ (`number`_)
    The X grid coordinate of the cell the mobile is in.

`cellY <tes3mobileObject/cellY.html>`_ (`number`_)
    The Y grid coordinate of the cell the mobile is in.

`damage <tes3mobileProjectile/damage.html>`_ (`number`_)
    Access to the raw damage that this projectile will inflict.

`expire <tes3mobileProjectile/expire.html>`_ (`number`_)
    A flag that causes the projectile to expire when set to 1.

`firingMobile <tes3mobileProjectile/firingMobile.html>`_ (`tes3mobileActor`_)
    The mobile that fired this projectile.

`firingWeapon <tes3mobileProjectile/firingWeapon.html>`_ (`tes3weapon`_)
    The weapon that fired this projectile.

`flags <tes3mobileObject/flags.html>`_ (`number`_)
    Access to the root mobile object flags, represented as an integer. Should not be accessed directly.

`height <tes3mobileObject/height.html>`_ (`number`_)
    The height of the mobile above the ground.

`impulseVelocity <tes3mobileObject/impulseVelocity.html>`_ (`tes3vector3`_)
    A vector that represents the 3D acceleration of the object.

`initialSpeed <tes3mobileProjectile/initialSpeed.html>`_ (`number`_)
    The initial speed that the projectile was launched at.

`movementCollision <tes3mobileObject/movementCollision.html>`_ (`boolean`_)
    Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.

`movementFlags <tes3mobileObject/movementFlags.html>`_ (`number`_)
    Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.

`objectType <tes3mobileObject/objectType.html>`_ (`number`_)
    The type of mobile object. Maps to values in tes3.objectType.

`playerDistance <tes3mobileObject/playerDistance.html>`_ (`number`_)
    The distance to the player. Updated every frame when the mobile is in an active cell.

`position <tes3mobileObject/position.html>`_ (`tes3vector3`_)
    A vector that represents the 3D position of the object.

`prevMovementFlags <tes3mobileObject/prevMovementFlags.html>`_ (`number`_)
    Access to the root mobile object movement flags from the previous frame, represented as an integer. Should not be accessed directly.

`reference <tes3mobileObject/reference.html>`_ (`tes3reference`_)
    Access to the reference object for the mobile, if any.

`rotationSpeed <tes3mobileSpellProjectile/rotationSpeed.html>`_ (`number`_)
    The angular speed that the projectile rotates around its local Y axis.

`velocity <tes3mobileProjectile/velocity.html>`_ (`tes3vector3`_)
    Access to the current velocity of the projectile.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3mobileSpellProjectile/rotationSpeed

Methods
----------------------------------------------------------------------------------------------------

`explode <tes3mobileSpellProjectile/explode.html>`_ (`method`_)
    Detonates the projectile, as if it hit a static object. The effects of detonation are not applied immediately, but later in the frame, at the same time as projectile collisions. This ensures consistent behaviour of the magic system.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3mobileSpellProjectile/explode

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3mobileObject`: ../../lua/type/tes3mobileObject.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
