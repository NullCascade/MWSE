niPickRecord
====================================================================================================

A nested class of NiPick that stores the results of previous picking operations for use by the game engine.

Properties
----------------------------------------------------------------------------------------------------

`color <niPickRecord/color.html>`_ (`number`_)
    The vertex color at the point of intersection. The color is valid only when `returnColor = true`.

`distance <niPickRecord/distance.html>`_ (`number`_)
    The distance of the triangle intersection from the ray origin.

`intersection <niPickRecord/intersection.html>`_ (`tes3vector3`_)
    The intersection point on the triangle.

`normal <niPickRecord/normal.html>`_ (`tes3vector3`_)
    The normal vector at the point of intersection. The normal is valid only when `returnNormal = true`.

`object <niPickRecord/object.html>`_ (`niAVObject`_)
    The object of the record.

`parent <niPickRecord/parent.html>`_ (`niAVObject`_)
    The parent of the record.

`reference <niPickRecord/reference.html>`_ (`tes3reference`_)
    Quick reference to the associated object's reference.

`texture <niPickRecord/texture.html>`_ (`tes3vector2`_)
    The texture coordinates at the point of intersection. The coordinates are valid only when `returnTexture = true`.

`triangleIndex <niPickRecord/triangleIndex.html>`_ (`number`_)
    The index of the intersected triangle.

`vertexIndex <niPickRecord/vertexIndex.html>`_ (`number`_)
    The index of the vertex of the intersected triangle.



.. toctree::
    :hidden:
    :maxdepth: 1

    niPickRecord/color
    niPickRecord/distance
    niPickRecord/intersection
    niPickRecord/normal
    niPickRecord/object
    niPickRecord/parent
    niPickRecord/reference
    niPickRecord/texture
    niPickRecord/triangleIndex
    niPickRecord/vertexIndex

.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`number`: ../../lua/type/number.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector2`: ../../lua/type/tes3vector2.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
