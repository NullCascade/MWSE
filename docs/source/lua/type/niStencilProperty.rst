niStencilProperty
====================================================================================================

A rendering property that controls the use of a stencil buffer when rendering. It also includes a draw-mode setting to allows the game engine to control the culling mode of a set of geometry.

This type inherits from the following parent types: `niProperty`_, `niObjectNET`_, `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`drawMode <niStencilProperty/drawMode.html>`_ (`number`_)
    The drawing mode used to draw the object.

`enabled <niStencilProperty/enabled.html>`_ (`boolean`_)
    The value of the stencil buffer enable flag.

`failAction <niStencilProperty/failAction.html>`_ (`number`_)
    The action that is taken in the stencil buffer when the stencil test fails.

`mask <niStencilProperty/mask.html>`_ (`number`_)
    The mask value of the stencil buffer.

`name <niObjectNET/name.html>`_ (`string`_)
    The human-facing name of the given object.

`passAction <niStencilProperty/passAction.html>`_ (`number`_)
    The action that is taken in the stencil buffer when the stencil test passes and the pixel passes the Z-buffer test.

`reference <niStencilProperty/reference.html>`_ (`number`_)
    The stencil reference.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.

`testFunc <niStencilProperty/testFunc.html>`_ (`number`_)
    The stencil buffer test function used to test the reference value against the buffer value.

`type <niProperty/type.html>`_ (`niPropertyType`_)
    The unique class identifier number of the given rendering property.

`zFailAction <niStencilProperty/zFailAction.html>`_ (`number`_)
    The action that is taken in the stencil buffer when the stencil test passes but the pixel fails the Z-buffer test.



.. toctree::
    :hidden:
    :maxdepth: 1

    niStencilProperty/drawMode
    niStencilProperty/enabled
    niStencilProperty/failAction
    niStencilProperty/mask
    niStencilProperty/passAction
    niStencilProperty/reference
    niStencilProperty/testFunc
    niStencilProperty/zFailAction

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


.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niProperty`: ../../lua/type/niProperty.html
.. _`niPropertyType`: ../../lua/type/niPropertyType.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
