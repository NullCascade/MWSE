niTexturingProperty
====================================================================================================

A rendering property that controls the methods used to filter texture pixels, and blend texture colors and vertex colors.

This type inherits from the following parent types: `niProperty`_, `niObjectNET`_, `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`applyMode <niTexturingProperty/applyMode.html>`_ (`niTexturingPropertyApplyMode`_)
    The texture apply mode. The texture apply mode controls how the vertex colors are blended with the texture color.

`baseMap <niTexturingProperty/baseMap.html>`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the base map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`bumpMap <niTexturingProperty/bumpMap.html>`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the bump map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`canAddDecal <niTexturingProperty/canAddDecal.html>`_ (`boolean`_)
    If true, the texturing property supports additional decal maps.

`darkMap <niTexturingProperty/darkMap.html>`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the dark map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`decalCount <niTexturingProperty/decalCount.html>`_ (`number`_)
    The number of decals currently on the texturing property.

`detailMap <niTexturingProperty/detailMap.html>`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the detail map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`glossMap <niTexturingProperty/glossMap.html>`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the gloss map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`glowMap <niTexturingProperty/glowMap.html>`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the glow map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`maps <niTexturingProperty/maps.html>`_ (`niTexturingPropertyMapTArray`_)
    The maps of texture property. Accessible as an array.

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

    niTexturingProperty/applyMode
    niTexturingProperty/baseMap
    niTexturingProperty/bumpMap
    niTexturingProperty/canAddDecal
    niTexturingProperty/darkMap
    niTexturingProperty/decalCount
    niTexturingProperty/detailMap
    niTexturingProperty/glossMap
    niTexturingProperty/glowMap
    niTexturingProperty/maps

Methods
----------------------------------------------------------------------------------------------------

`addDecalMap <niTexturingProperty/addDecalMap.html>`_ (`method`_)
    Attempts to create a new decal map. If successful, it returns both the new map and the index it was created in.

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

`removeDecalMap <niTexturingProperty/removeDecalMap.html>`_ (`method`_)
    Attempts to remove a decal at a given index.



.. toctree::
    :hidden:
    :maxdepth: 1

    niTexturingProperty/addDecalMap
    niTexturingProperty/removeDecalMap

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niProperty`: ../../lua/type/niProperty.html
.. _`niPropertyType`: ../../lua/type/niPropertyType.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niTexturingPropertyApplyMode`: ../../lua/type/niTexturingPropertyApplyMode.html
.. _`niTexturingPropertyMap`: ../../lua/type/niTexturingPropertyMap.html
.. _`niTexturingPropertyMapTArray`: ../../lua/type/niTexturingPropertyMapTArray.html
.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
