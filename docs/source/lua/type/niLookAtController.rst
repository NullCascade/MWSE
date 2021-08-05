niLookAtController
====================================================================================================

Orients an axis of the controlled object towards a lookAt target. The axis is selectable.

Warning: The lookAt property is not reference counted, so this controller must be removed, or lookAt set to nil before the lookAt object is deleted.

Properties
----------------------------------------------------------------------------------------------------

`active`_ (`boolean`_)
    If the controller is active. When inactive, the target does not get updated and animation timing does not advance.

`animTimingType`_ (`number`_)
    How the time parameter passed to the update function is treated. Type 0 is the default, which treats the update time as a delta time. Type 1 treats the update time an offset from the start of the animation.

`axis`_ (`number`_)
    A number representing the axis that points at the lookAt object. ``flip`` determines which end of this axis points at the target.
``0`` X axis
``1`` Y axis
``2`` Z axis

`cycleType`_ (`number`_)
    The animation cycle type. Type 0 is looping, type 2 is reverse, type 4 is clamped.

`flip`_ (`boolean`_)
    Determines which end of the chosen axis points towards the lookAt target.

`frequency`_ (`number`_)
    The animation controller's frequency value.

`highKeyFrame`_ (`number`_)
    No description.

`lastScaledTime`_ (`number`_)
    No description.

`lastTime`_ (`number`_)
    No description.

`lookAt`_ (`niAVObject`_)
    The object that this controller will point towards. This is not a reference counted pointer, so this controller must be removed or lookAt set to nil before the lookAt object is deleted. This was probably designed to avoid reference cycles where controllers keep an ancestor node alive.

`lowKeyFrame`_ (`number`_)
    No description.

`nextController`_ (`niTimeController`_)
    The next NiTimeController in the list of controllers containing the calling controller.

`phase`_ (`number`_)
    The phase value.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`startTime`_ (`number`_)
    No description.

`target`_ (`niObjectNET`_)
    The scene graph object that this animation controller is to animate.

.. toctree::
    :hidden:

    niLookAtController/active
    niLookAtController/animTimingType
    niLookAtController/axis
    niLookAtController/cycleType
    niLookAtController/flip
    niLookAtController/frequency
    niLookAtController/highKeyFrame
    niLookAtController/lastScaledTime
    niLookAtController/lastTime
    niLookAtController/lookAt
    niLookAtController/lowKeyFrame
    niLookAtController/nextController
    niLookAtController/phase
    niLookAtController/references
    niLookAtController/runTimeTypeInformation
    niLookAtController/startTime
    niLookAtController/target

.. _`active`: niLookAtController/active.html
.. _`animTimingType`: niLookAtController/animTimingType.html
.. _`axis`: niLookAtController/axis.html
.. _`cycleType`: niLookAtController/cycleType.html
.. _`flip`: niLookAtController/flip.html
.. _`frequency`: niLookAtController/frequency.html
.. _`highKeyFrame`: niLookAtController/highKeyFrame.html
.. _`lastScaledTime`: niLookAtController/lastScaledTime.html
.. _`lastTime`: niLookAtController/lastTime.html
.. _`lookAt`: niLookAtController/lookAt.html
.. _`lowKeyFrame`: niLookAtController/lowKeyFrame.html
.. _`nextController`: niLookAtController/nextController.html
.. _`phase`: niLookAtController/phase.html
.. _`references`: niLookAtController/references.html
.. _`runTimeTypeInformation`: niLookAtController/runTimeTypeInformation.html
.. _`startTime`: niLookAtController/startTime.html
.. _`target`: niLookAtController/target.html

Methods
----------------------------------------------------------------------------------------------------

`clone`_ (`niObject`_)
    Creates a copy of this object.

`isInstanceOfType`_ (`boolean`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType`_ (`boolean`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`start`_
    Starts the controller at the specified time.

`stop`_
    Stops the controller. If you only want to pause the controller, use the ``update`` property.

.. toctree::
    :hidden:

    niLookAtController/clone
    niLookAtController/isInstanceOfType
    niLookAtController/isOfType
    niLookAtController/start
    niLookAtController/stop

.. _`clone`: niLookAtController/clone.html
.. _`isInstanceOfType`: niLookAtController/isInstanceOfType.html
.. _`isOfType`: niLookAtController/isOfType.html
.. _`start`: niLookAtController/start.html
.. _`stop`: niLookAtController/stop.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niTimeController`: ../../lua/type/niTimeController.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
