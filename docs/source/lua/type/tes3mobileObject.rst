tes3mobileObject
====================================================================================================

The base object from which all other mobiles (AI/movement using) structures derive.

Properties
----------------------------------------------------------------------------------------------------

`boundSize <tes3mobileObject/boundSize.html>`_ (`tes3vector3`_)
    A vector that shows the size of the bounding box in each direction.

`cellX <tes3mobileObject/cellX.html>`_ (`number`_)
    The X grid coordinate of the cell the mobile is in.

`cellY <tes3mobileObject/cellY.html>`_ (`number`_)
    The Y grid coordinate of the cell the mobile is in.

`flags <tes3mobileObject/flags.html>`_ (`number`_)
    Access to the root mobile object flags, represented as an integer. Should not be accessed directly.

`height <tes3mobileObject/height.html>`_ (`number`_)
    The height of the mobile above the ground.

`impulseVelocity <tes3mobileObject/impulseVelocity.html>`_ (`tes3vector3`_)
    A vector that represents the 3D acceleration of the object.

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

`velocity <tes3mobileObject/velocity.html>`_ (`tes3vector3`_)
    A vector that represents the 3D velocity of the object.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3mobileObject/boundSize
    tes3mobileObject/cellX
    tes3mobileObject/cellY
    tes3mobileObject/flags
    tes3mobileObject/height
    tes3mobileObject/impulseVelocity
    tes3mobileObject/movementCollision
    tes3mobileObject/movementFlags
    tes3mobileObject/objectType
    tes3mobileObject/playerDistance
    tes3mobileObject/position
    tes3mobileObject/prevMovementFlags
    tes3mobileObject/reference
    tes3mobileObject/velocity

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
