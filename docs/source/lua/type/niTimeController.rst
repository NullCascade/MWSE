niTimeController
====================================================================================================

Controls the timing and update functions for animation controllers.

Properties
----------------------------------------------------------------------------------------------------

`active`_ (`boolean`_)
    If the controller is active. When inactive, the target does not get updated and animation timing does not advance.

`animTimingType`_ (`number`_)
    How the time parameter passed to the update function is treated. Type 0 is the default, which treats the update time as a delta time. Type 1 treats the update time an offset from the start of the animation.

`cycleType`_ (`number`_)
    The animation cycle type. Type 0 is looping, type 2 is reverse, type 4 is clamped.

`frequency`_ (`number`_)
    The animation controller's frequency value.

`highKeyFrame`_ (`number`_)
    No description.

`lastScaledTime`_ (`number`_)
    No description.

`lastTime`_ (`number`_)
    No description.

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

    niTimeController/active
    niTimeController/animTimingType
    niTimeController/cycleType
    niTimeController/frequency
    niTimeController/highKeyFrame
    niTimeController/lastScaledTime
    niTimeController/lastTime
    niTimeController/lowKeyFrame
    niTimeController/nextController
    niTimeController/phase
    niTimeController/references
    niTimeController/runTimeTypeInformation
    niTimeController/startTime
    niTimeController/target

.. _`active`: niTimeController/active.html
.. _`animTimingType`: niTimeController/animTimingType.html
.. _`cycleType`: niTimeController/cycleType.html
.. _`frequency`: niTimeController/frequency.html
.. _`highKeyFrame`: niTimeController/highKeyFrame.html
.. _`lastScaledTime`: niTimeController/lastScaledTime.html
.. _`lastTime`: niTimeController/lastTime.html
.. _`lowKeyFrame`: niTimeController/lowKeyFrame.html
.. _`nextController`: niTimeController/nextController.html
.. _`phase`: niTimeController/phase.html
.. _`references`: niTimeController/references.html
.. _`runTimeTypeInformation`: niTimeController/runTimeTypeInformation.html
.. _`startTime`: niTimeController/startTime.html
.. _`target`: niTimeController/target.html

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

    niTimeController/clone
    niTimeController/isInstanceOfType
    niTimeController/isOfType
    niTimeController/start
    niTimeController/stop

.. _`clone`: niTimeController/clone.html
.. _`isInstanceOfType`: niTimeController/isInstanceOfType.html
.. _`isOfType`: niTimeController/isOfType.html
.. _`start`: niTimeController/start.html
.. _`stop`: niTimeController/stop.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niTimeController`: ../../lua/type/niTimeController.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
