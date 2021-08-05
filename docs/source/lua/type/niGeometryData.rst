niGeometryData
====================================================================================================

        NiGeometryData objects contain the geometry data necessary to render an NiGeometry object. When an NiGeometry-based object is created, the actual geometry data is stored in an attached NiGeometryData object. 
    

This type inherits from the following parent types: `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`bounds <niGeometryData/bounds.html>`_ (`niBound`_)
    The bounds of the object.

`colors <niGeometryData/colors.html>`_ (`niColorA`_)
    The color for the object.

`normals <niGeometryData/normals.html>`_ (`tes3vector3`_)
    The normals list for the object.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.

`texCoords <niGeometryData/texCoords.html>`_ (`tes3vector2`_)
    The array of texture coordinates.

`textures <niGeometryData/textures.html>`_ (`tes3vector2`_)
    The array of texture coordinates.

`textureSets <niGeometryData/textureSets.html>`_ (`number`_)
    The number of texture coordinate sets in the data.

`uniqueID <niGeometryData/uniqueID.html>`_ (`number`_)
    A unique ID for this model, assigned at model creation.

`vertexCount <niGeometryData/vertexCount.html>`_ (`number`_)
    The vertex count for the object.

`vertices <niGeometryData/vertices.html>`_ (`tes3vector3`_)
    The array of vertex position data.



.. toctree::
    :hidden:
    :maxdepth: 1

    niGeometryData/bounds
    niGeometryData/colors
    niGeometryData/normals
    niGeometryData/texCoords
    niGeometryData/textures
    niGeometryData/textureSets
    niGeometryData/uniqueID
    niGeometryData/vertexCount
    niGeometryData/vertices

Methods
----------------------------------------------------------------------------------------------------

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`isInstanceOfType <niObject/isInstanceOfType.html>`_ (`method`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType <niObject/isOfType.html>`_ (`method`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`markAsChanged <niGeometryData/markAsChanged.html>`_ (`method`_)
    Tells the renderer that the object has changed. Should be called after you have finished changing any vertex data.

If you have altered vertex positions, you may need to also call `updateModelBound`_. You should call it if vertices have been moved outside the bounds of the original model.

`updateModelBound <niGeometryData/updateModelBound.html>`_ (`method`_)
    Updates the geometry bounds to match the vertex data. You should call it if vertices have been moved outside the bounds of the original model, or if the effective bounds have become significantly smaller. 

If you already know the effective radius of the vertex data, you could more efficiently set the bounds directly instead of calling this function.



.. toctree::
    :hidden:
    :maxdepth: 1

    niGeometryData/markAsChanged
    niGeometryData/updateModelBound

.. _`method`: ../../lua/type/method.html
.. _`niBound`: ../../lua/type/niBound.html
.. _`niColorA`: ../../lua/type/niColorA.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3vector2`: ../../lua/type/tes3vector2.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`updateModelBound`: ../../lua/type/updateModelBound.html
