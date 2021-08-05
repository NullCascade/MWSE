niTimeController
====================================================================================================

Controls the timing and update functions for animation controllers.

This type inherits from the following parent types: `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`active <niTimeController/active.html>`_ (`boolean`_)
    If the controller is active. When inactive, the target does not get updated and animation timing does not advance.

`animTimingType <niTimeController/animTimingType.html>`_ (`number`_)
    How the time parameter passed to the update function is treated. Type 0 is the default, which treats the update time as a delta time. Type 1 treats the update time an offset from the start of the animation.

`cycleType <niTimeController/cycleType.html>`_ (`number`_)
    The animation cycle type. Type 0 is looping, type 2 is reverse, type 4 is clamped.

`frequency <niTimeController/frequency.html>`_ (`number`_)
    The animation controller's frequency value.

`highKeyFrame <niTimeController/highKeyFrame.html>`_ (`number`_)
    No description.

`lastScaledTime <niTimeController/lastScaledTime.html>`_ (`number`_)
    No description.

`lastTime <niTimeController/lastTime.html>`_ (`number`_)
    No description.

`lowKeyFrame <niTimeController/lowKeyFrame.html>`_ (`number`_)
    No description.

`nextController <niTimeController/nextController.html>`_ (`niTimeController`_)
    The next NiTimeController in the list of controllers containing the calling controller.

`phase <niTimeController/phase.html>`_ (`number`_)
    The phase value.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.

`startTime <niTimeController/startTime.html>`_ (`number`_)
    No description.

`target <niTimeController/target.html>`_ (`niObjectNET`_)
    The scene graph object that this animation controller is to animate.



.. toctree::
    :hidden:
    :maxdepth: 1

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
    niTimeController/startTime
    niTimeController/target

Methods
----------------------------------------------------------------------------------------------------

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`isInstanceOfType <niObject/isInstanceOfType.html>`_ (`method`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType <niObject/isOfType.html>`_ (`method`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`start <niTimeController/start.html>`_ (`method`_)
    Starts the controller at the specified time.

`stop <niTimeController/stop.html>`_ (`method`_)
    Stops the controller. If you only want to pause the controller, use the ``update`` property.



.. toctree::
    :hidden:
    :maxdepth: 1

    niTimeController/start
    niTimeController/stop

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niTimeController`: ../../lua/type/niTimeController.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
