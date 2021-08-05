tes3cell
====================================================================================================

An exterior or interior game area.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`activators <tes3cell/activators.html>`_ (`tes3referenceList`_)
    One of the three reference collections for a cell.

`actors <tes3cell/actors.html>`_ (`tes3referenceList`_)
    One of the three reference collections for a cell.

`ambientColor <tes3cell/ambientColor.html>`_ (`tes3packedColor`_)
    The cell's ambient color. Only available on interior cells.

`behavesAsExterior <tes3cell/behavesAsExterior.html>`_ (`boolean`_)
    If true, the cell behaves as an exterior instead of an interior for certain properties. Only available on interior cells.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`cellFlags <tes3cell/cellFlags.html>`_ (`number`_)
    A numeric representation of the packed bit flags for the cell, typically accessed from other properties.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`displayName <tes3cell/displayName.html>`_ (`string`_)
    The name of the cell as displayed on the in-game UI. For exterior cells without an assigned name, this is the region name.

`editorName <tes3cell/editorName.html>`_ (`string`_)
    The name of the cell as displayed in the Construction Set. For exterior cells, this includes the cell coordinates.

`fogColor <tes3cell/fogColor.html>`_ (`tes3packedColor`_)
    The cell's fog color. Only available on interior cells.

`fogDensity <tes3cell/fogDensity.html>`_ (`number`_)
    The cell's fog density. Only available on interior cells.

`gridX <tes3cell/gridX.html>`_ (`number`_)
    The cell's X grid coordinate. Only available on exterior cells.

`gridY <tes3cell/gridY.html>`_ (`number`_)
    The cell's Y grid coordinate. Only available on exterior cells.

`hasWater <tes3cell/hasWater.html>`_ (`boolean`_)
    If true, the cell has water. Only applies to interior cells.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`isInterior <tes3cell/isInterior.html>`_ (`boolean`_)
    If true, the cell is an interior.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3cell/name.html>`_ (`string`_)
    The name and id of the cell. See also displayName and editorName.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`pickObjectsRoot <tes3cell/pickObjectsRoot.html>`_ (`niNode`_)
    The scenegraph node containing player-interactable objects from this cell.

`region <tes3cell/region.html>`_ (`tes3region`_)
    The region associated with the cell. Only available on exterior cells, or interior cells that behave as exterior cells.

`restingIsIllegal <tes3cell/restingIsIllegal.html>`_ (`boolean`_)
    If true, the player may not rest in the cell.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`staticObjectsRoot <tes3cell/staticObjectsRoot.html>`_ (`tes3region`_)
    The scenegraph node containing static non-player-interactable objects from this cell.

`statics <tes3cell/statics.html>`_ (`tes3referenceList`_)
    One of the three reference collections for a cell.

`sunColor <tes3cell/sunColor.html>`_ (`tes3packedColor`_)
    The cell's sun color. Only available on interior cells.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`waterLevel <tes3cell/waterLevel.html>`_ (`number`_)
    The water level in the cell. Only available on interior cells.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3cell/activators
    tes3cell/actors
    tes3cell/ambientColor
    tes3cell/behavesAsExterior
    tes3cell/cellFlags
    tes3cell/displayName
    tes3cell/editorName
    tes3cell/fogColor
    tes3cell/fogDensity
    tes3cell/gridX
    tes3cell/gridY
    tes3cell/hasWater
    tes3cell/isInterior
    tes3cell/name
    tes3cell/pickObjectsRoot
    tes3cell/region
    tes3cell/restingIsIllegal
    tes3cell/staticObjectsRoot
    tes3cell/statics
    tes3cell/sunColor
    tes3cell/waterLevel

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`iterateReferences <tes3cell/iterateReferences.html>`_ (`method`_)
    Used in a for loop, iterates over objects in the cell.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3cell/iterateReferences

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3packedColor`: ../../lua/type/tes3packedColor.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3region`: ../../lua/type/tes3region.html
