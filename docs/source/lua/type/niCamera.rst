niCamera
====================================================================================================

Object that represent a camera. Position and orientation is determined by parent properties.

This type inherits from the following parent types: `niAVObject`_, `niObjectNET`_, `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`alphaProperty <niAVObject/alphaProperty.html>`_ (`niAlphaProperty`_, `nil`_)
    Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

`appCulled <niAVObject/appCulled.html>`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`flags <niAVObject/flags.html>`_ (`number`_)
    Flags, dependent on the specific object type.

`fogProperty <niAVObject/fogProperty.html>`_ (`niFogProperty`_, `nil`_)
    Convenient access to this object's fog property. Setting this value to be nil will erase the property, while setting it to a valid fog property will set (or replace) it.

`lodAdjust <niCamera/lodAdjust.html>`_ (`number`_)
    The level-of-detail adjustment value for the camera. A higher value will lower the LOD of the objects drawn by the camera.

`materialProperty <niAVObject/materialProperty.html>`_ (`niMaterialProperty`_, `nil`_)
    Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.

`name <niObjectNET/name.html>`_ (`string`_)
    The human-facing name of the given object.

`parent <niAVObject/parent.html>`_ (`niNode`_)
    The object's parent. It may not have one if it is not attached to the scene.

`port <niCamera/port.html>`_ (`tes3vector4`_)
    The port on the backbuffer of the camera.

`properties <niAVObject/properties.html>`_ (`niPropertyLinkedList`_)
    The list of properties attached to this niAVObject.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`renderer <niCamera/renderer.html>`_ (`niObject`_)
    The renderer of the camera.

`rotation <niAVObject/rotation.html>`_ (`tes3matrix33`_)
    The object's local rotation matrix.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.

`scale <niAVObject/scale.html>`_ (`number`_)
    The object's local uniform scaling factor.

`scene <niCamera/scene.html>`_ (`niObject`_)
    The scene of the camera.

`stencilProperty <niAVObject/stencilProperty.html>`_ (`niStencilProperty`_, `nil`_)
    Convenient access to this object's stencil property. Setting this value to be nil will erase the property, while setting it to a valid stencil property will set (or replace) it.

`texturingProperty <niAVObject/texturingProperty.html>`_ (`niTexturingProperty`_, `nil`_)
    Convenient access to this object's texturing property. Setting this value to be nil will erase the property, while setting it to a valid texturing property will set (or replace) it.

`translation <niAVObject/translation.html>`_ (`tes3vector3`_)
    The object's local translation vector.

`vertexColorProperty <niAVObject/vertexColorProperty.html>`_ (`niVertexColorProperty`_, `nil`_)
    Convenient access to this object's vertex coloring property. Setting this value to be nil will erase the property, while setting it to a valid vertex coloring property will set (or replace) it.

`viewDistance <niCamera/viewDistance.html>`_ (`number`_)
    The view distance of the camera.

`worldDirection <niCamera/worldDirection.html>`_ (`tes3vector3`_)
    Defines the X-axis of the camera.

`worldRight <niCamera/worldRight.html>`_ (`tes3vector3`_)
    Defines the Z-axis of the camera.

`worldToCamera <niCamera/worldToCamera.html>`_ (`tes3matrix44`_)
    The world-to-camera transform matrix in row-major order.

`worldUp <niCamera/worldUp.html>`_ (`tes3vector3`_)
    Defines the Y-axis of the camera.

`zBufferProperty <niAVObject/zBufferProperty.html>`_ (`niZBufferProperty`_, `nil`_)
    Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.



.. toctree::
    :hidden:
    :maxdepth: 1

    niCamera/lodAdjust
    niCamera/port
    niCamera/renderer
    niCamera/scene
    niCamera/viewDistance
    niCamera/worldDirection
    niCamera/worldRight
    niCamera/worldToCamera
    niCamera/worldUp

Methods
----------------------------------------------------------------------------------------------------

`attachProperty <niAVObject/attachProperty.html>`_ (`method`_)
    Attach a property to this object.

`clearTransforms <niAVObject/clearTransforms.html>`_ (`method`_)
    Resets the object's local transform.

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`detachProperty <niAVObject/detachProperty.html>`_ (`method`_)
    Detaches and returns a property from the object which matches the given property type.

`getObjectByName <niAVObject/getObjectByName.html>`_ (`method`_)
    Searches this node and all child nodes recursively for a node with a name that matches the argument.

`getProperty <niAVObject/getProperty.html>`_ (`method`_)
    Gets an attached property by property type.

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

`update <niAVObject/update.html>`_ (`method`_)
    Updates the world transforms of this node and its children, which makes changes visible for rendering. Use after changing any local rotation, translation, scale, or bounds.

`updateEffects <niAVObject/updateEffects.html>`_ (`method`_)
    Update all attached effects.

`updateProperties <niAVObject/updateProperties.html>`_ (`method`_)
    Update all attached properties.

`windowPointToRay <niCamera/windowPointToRay.html>`_ (`method`_)
    Given a screen space position, calculates the world position and outlook direction. This can be useful when trying to find a reference under a UI element, such as the cusor.

`worldPointToScreenPoint <niCamera/worldPointToScreenPoint.html>`_ (`method`_)
    Given a world coordinate position, calculates where that point relates to the screen space.



.. toctree::
    :hidden:
    :maxdepth: 1

    niCamera/windowPointToRay
    niCamera/worldPointToScreenPoint

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`niAlphaProperty`: ../../lua/type/niAlphaProperty.html
.. _`niFogProperty`: ../../lua/type/niFogProperty.html
.. _`niMaterialProperty`: ../../lua/type/niMaterialProperty.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niPropertyLinkedList`: ../../lua/type/niPropertyLinkedList.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niStencilProperty`: ../../lua/type/niStencilProperty.html
.. _`niTexturingProperty`: ../../lua/type/niTexturingProperty.html
.. _`niVertexColorProperty`: ../../lua/type/niVertexColorProperty.html
.. _`niZBufferProperty`: ../../lua/type/niZBufferProperty.html
.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3matrix33`: ../../lua/type/tes3matrix33.html
.. _`tes3matrix44`: ../../lua/type/tes3matrix44.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3vector4`: ../../lua/type/tes3vector4.html
