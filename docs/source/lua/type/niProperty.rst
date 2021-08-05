niProperty
====================================================================================================

A base class representing all rendering properties.

This type inherits from the following parent types: `niObjectNET`_, `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`name <niObjectNET/name.html>`_ (`string`_)
    The human-facing name of the given object.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.

`type <niProperty/type.html>`_ (`niPropertyType`_)
    The unique class identifier number of the given rendering property.



.. toctree::
    :hidden:
    :maxdepth: 1

    niProperty/type

Methods
----------------------------------------------------------------------------------------------------

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`isInstanceOfType <niObject/isInstanceOfType.html>`_ (`method`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType <niObject/isOfType.html>`_ (`method`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`prependController <niObjectNET/prependController.html>`_ (`method`_)
    Add a controller to the object as the first controller.

`removeAllControllers <niObjectNET/removeAllControllers.html>`_ (`method`_)
    Removes all controllers.

`removeController <niObjectNET/removeController.html>`_ (`method`_)
    Removes a controller from the object.



.. toctree::
    :hidden:
    :maxdepth: 1


.. _`method`: ../../lua/type/method.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niPropertyType`: ../../lua/type/niPropertyType.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`string`: ../../lua/type/string.html
