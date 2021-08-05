niSwitchNode
====================================================================================================

Represents groups of multiple scene graph subtrees, only one of which is visible at any given time. They are useful for showing different states of a model depending on engine / lua logic. If you detach the active subtree, the switch node will set the active subtree to none, or to an index of -1.

This type inherits from the following parent types: `niNode`_, `niAVObject`_, `niObjectNET`_, `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`alphaProperty <niAVObject/alphaProperty.html>`_ (`niAlphaProperty`_, `nil`_)
    Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

`appCulled <niAVObject/appCulled.html>`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`children <niNode/children.html>`_ (`niAVObjectTArray`_)
    The children of the node.

`effectList <niNode/effectList.html>`_ (`niDynamicEffectLinkedList`_)
    The effect list of the node.

`flags <niAVObject/flags.html>`_ (`number`_)
    Flags, dependent on the specific object type.

`fogProperty <niAVObject/fogProperty.html>`_ (`niFogProperty`_, `nil`_)
    Convenient access to this object's fog property. Setting this value to be nil will erase the property, while setting it to a valid fog property will set (or replace) it.

`materialProperty <niAVObject/materialProperty.html>`_ (`niMaterialProperty`_, `nil`_)
    Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.

`name <niObjectNET/name.html>`_ (`string`_)
    The human-facing name of the given object.

`parent <niAVObject/parent.html>`_ (`niNode`_)
    The object's parent. It may not have one if it is not attached to the scene.

`properties <niAVObject/properties.html>`_ (`niPropertyLinkedList`_)
    The list of properties attached to this niAVObject.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`rotation <niAVObject/rotation.html>`_ (`tes3matrix33`_)
    The object's local rotation matrix.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.

`scale <niAVObject/scale.html>`_ (`number`_)
    The object's local uniform scaling factor.

`stencilProperty <niAVObject/stencilProperty.html>`_ (`niStencilProperty`_, `nil`_)
    Convenient access to this object's stencil property. Setting this value to be nil will erase the property, while setting it to a valid stencil property will set (or replace) it.

`switchIndex <niSwitchNode/switchIndex.html>`_ (`number`_)
    The value of the active child index. This index represents the index of the active child in the array of the NiSwitchNode. If this index is –1, then no child is active.

`texturingProperty <niAVObject/texturingProperty.html>`_ (`niTexturingProperty`_, `nil`_)
    Convenient access to this object's texturing property. Setting this value to be nil will erase the property, while setting it to a valid texturing property will set (or replace) it.

`translation <niAVObject/translation.html>`_ (`tes3vector3`_)
    The object's local translation vector.

`vertexColorProperty <niAVObject/vertexColorProperty.html>`_ (`niVertexColorProperty`_, `nil`_)
    Convenient access to this object's vertex coloring property. Setting this value to be nil will erase the property, while setting it to a valid vertex coloring property will set (or replace) it.

`zBufferProperty <niAVObject/zBufferProperty.html>`_ (`niZBufferProperty`_, `nil`_)
    Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.



.. toctree::
    :hidden:
    :maxdepth: 1

    niSwitchNode/switchIndex

Methods
----------------------------------------------------------------------------------------------------

`attachChild <niNode/attachChild.html>`_ (`method`_)
    Attachs the child to the children list of the node.

`attachProperty <niAVObject/attachProperty.html>`_ (`method`_)
    Attach a property to this object.

`clearTransforms <niAVObject/clearTransforms.html>`_ (`method`_)
    Resets the object's local transform.

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`detachChild <niNode/detachChild.html>`_ (`method`_)
    Detaches the child from the children list of the node. Returns the detached child.

`detachChildAt <niNode/detachChildAt.html>`_ (`method`_)
    Detaches the child at the specified index from the children list of the node. Returns the detached child.

`detachProperty <niAVObject/detachProperty.html>`_ (`method`_)
    Detaches and returns a property from the object which matches the given property type.

`getActiveChild <niSwitchNode/getActiveChild.html>`_ (`method`_)
    Gets the active child node, or returns nil if the switch index is -1.

`getEffect <niNode/getEffect.html>`_ (`method`_)
    Gets the effect for the given type.

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



.. toctree::
    :hidden:
    :maxdepth: 1

    niSwitchNode/getActiveChild

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`niAVObjectTArray`: ../../lua/type/niAVObjectTArray.html
.. _`niAlphaProperty`: ../../lua/type/niAlphaProperty.html
.. _`niDynamicEffectLinkedList`: ../../lua/type/niDynamicEffectLinkedList.html
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
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
