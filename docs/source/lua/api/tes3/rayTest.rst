tes3.rayTest
====================================================================================================

Performs a ray test and returns various information related to the result(s). If findAll is set, the result will be a table of results, otherwise only the first result is returned.

Returns
----------------------------------------------------------------------------------------------------

result (`niPickRecord`_, `table`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

position (`tes3vector3`_, `table`_)
    Position of the ray origin.

direction (`tes3vector3`_, `table`_)
    Direction of the ray. Does not have to be unit length.

findAll (`boolean`_)
    If true, the ray test won't stop after the first result.

maxDistance (`number`_)
    Optional. The maximum distance that the test will run.

sort (`boolean`_)
    Default: ``true``. If true, the results will be sorted by distance from the origin position.

useModelBounds (`boolean`_)
    If true, model bounds will be tested for intersection. Otherwise triangles will be used.

useModelCoordinates (`boolean`_)
    If true, model coordinates will be used instead of world coordinates.

useBackTriangles (`boolean`_)
    Include intersections with back-facing triangles.

observeAppCullFlag (`boolean`_)
    Default: ``true``. Ignore intersections with culled (hidden) models.

root (`node*`_)
    Default: ``tes3.game.worldSceneGraphRoot``. Node pointer to node scene.

ignoreSkinned (`boolean`_)
    Ignore results from skinned objects.

returnColor (`boolean`_)
    Calculate and return the vertex color at intersections.

returnNormal (`boolean`_)
    Default: ``true``. Calculate and return the vertex normal at intersections.

returnSmoothNormal (`boolean`_)
    Use normal interpolation for calculating vertex normals.

returnTexture (`boolean`_)
    Calculate and return the texture coordinate at intersections.

ignore (`table`_)
    Optional. An array of references and/or scene graph nodes to cull from the result(s).

.. _`boolean`: ../../../lua/type/boolean.html
.. _`niPickRecord`: ../../../lua/type/niPickRecord.html
.. _`node*`: ../../../lua/type/node*.html
.. _`number`: ../../../lua/type/number.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
