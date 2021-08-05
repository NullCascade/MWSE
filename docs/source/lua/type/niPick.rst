niPick
====================================================================================================

Class used in the picking intersection system within the game engine.  Using a ray, the engine performs picking operations on a scene graph or any subtree. Given a ray and a subtree, the subtree is traversed and matching nodes are added to an array.

Properties
----------------------------------------------------------------------------------------------------

`coordinateType <niPick/coordinateType.html>`_ (`niPickCoordinateType`_)
    The coordinate type for the object.

`frontOnly <niPick/frontOnly.html>`_ (`boolean`_)
    If true, back face triangles (those with clockwise winding order) are culled prior to intersection calculations. If false, intersections of ray with triangles are computed regardless of triangle orientation.
    

`intersectType <niPick/intersectType.html>`_ (`niPickIntersectType`_)
    The intersect type for the object.

`lastAddedRecord <niPick/lastAddedRecord.html>`_ (`niPickRecord`_)
    The pick record that was last added to the object.

`observeAppCullFlag <niPick/observeAppCullFlag.html>`_ (`boolean`_)
    If true, the pick stops if it encounters an object with the 'app culled' flag set.  If false, the pick will continue over 'app culled' objects.
    

`pickType <niPick/pickType.html>`_ (`niPickType`_)
    The pick type for the object.

`results <niPick/results.html>`_ (`niPickRecordTArray`_)
    The array of pick records obtained by the last call to PickObjects.

`returnColor <niPick/returnColor.html>`_ (`boolean`_)
    If true, a vertex color value is returned for the point of intersection of triangles during picking.

`returnNormal <niPick/returnNormal.html>`_ (`boolean`_)
    If true, a unit-length normal is returned for the point of intersection of triangles intersected during the picking.

`returnSmoothNormal <niPick/returnSmoothNormal.html>`_ (`boolean`_)
    If true and if vertex normals exist, then a unit-length normal vector is interpolated from the vertex normals and returned during the picking operation.

`returnTexture <niPick/returnTexture.html>`_ (`boolean`_)
    If true, texture coordinates are returned for the point of intersection of triangles during the picking operation.

`root <niPick/root.html>`_ (`niPickType`_)
    The root pick type for the object.

`sortType <niPick/sortType.html>`_ (`niPickSortType`_)
    The sort type for the object.



.. toctree::
    :hidden:
    :maxdepth: 1

    niPick/coordinateType
    niPick/frontOnly
    niPick/intersectType
    niPick/lastAddedRecord
    niPick/observeAppCullFlag
    niPick/pickType
    niPick/results
    niPick/returnColor
    niPick/returnNormal
    niPick/returnSmoothNormal
    niPick/returnTexture
    niPick/root
    niPick/sortType

Methods
----------------------------------------------------------------------------------------------------

`clearResults <niPick/clearResults.html>`_ (`method`_)
    Clears the results of any previous picking operation.

`pickObjects <niPick/pickObjects.html>`_ (`method`_)
    Performs the picking operation.



.. toctree::
    :hidden:
    :maxdepth: 1

    niPick/clearResults
    niPick/pickObjects

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niPickCoordinateType`: ../../lua/type/niPickCoordinateType.html
.. _`niPickIntersectType`: ../../lua/type/niPickIntersectType.html
.. _`niPickRecord`: ../../lua/type/niPickRecord.html
.. _`niPickRecordTArray`: ../../lua/type/niPickRecordTArray.html
.. _`niPickSortType`: ../../lua/type/niPickSortType.html
.. _`niPickType`: ../../lua/type/niPickType.html
