niObject
====================================================================================================

The base-most object from which almost all NetImmerse structures are derived from.

Properties
----------------------------------------------------------------------------------------------------

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.



.. toctree::
    :hidden:
    :maxdepth: 1

    niObject/references
    niObject/runTimeTypeInformation

Methods
----------------------------------------------------------------------------------------------------

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`isInstanceOfType <niObject/isInstanceOfType.html>`_ (`method`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType <niObject/isOfType.html>`_ (`method`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.



.. toctree::
    :hidden:
    :maxdepth: 1

    niObject/clone
    niObject/isInstanceOfType
    niObject/isOfType

.. _`method`: ../../lua/type/method.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`string`: ../../lua/type/string.html
