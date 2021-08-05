addDecalMap
====================================================================================================

Attempts to create a new decal map. If successful, it returns both the new map and the index it was created in.

Returns
----------------------------------------------------------------------------------------------------

The function has more than one return value.

map (`niTexturingPropertyMap`_, `nil`_)
    A newly created decal map.

index (`number`_, `nil`_)
    The index of the newly added decal map.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

texture (`niTexture`_)
    Optional. The texture to assign to the new decal.

.. _`niTexture`: ../../../lua/type/niTexture.html
.. _`niTexturingPropertyMap`: ../../../lua/type/niTexturingPropertyMap.html
.. _`nil`: ../../../lua/type/nil.html
.. _`number`: ../../../lua/type/number.html
