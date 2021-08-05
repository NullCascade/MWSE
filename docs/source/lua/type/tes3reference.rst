tes3reference
====================================================================================================

A reference is a sort of container structure for objects. It holds a base object, as well as various variables associated with that object that make it unique.

For example, many doors may share the same base object. However, each door reference might have a different owner, different lock/trap statuses, etc. that make the object unique.

This type inherits from the following parent types: `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`activationReference <tes3reference/activationReference.html>`_ (`tes3reference`_)
    The current reference, if any, that this reference will activate.

`attachments <tes3reference/attachments.html>`_ (`table`_)
    A table with friendly named access to all supported attachments.

`baseObject <tes3reference/baseObject.html>`_ (`tes3physicalObject`_)
    This is similar to the object field, but is guaranteed to provide the base-most object. If object is an actor clone, the base actor will be given instead.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`bodyPartManager <tes3reference/bodyPartManager.html>`_ (`tes3bodyPartManager`_, `nil`_)
    Access to the reference's body part manager, if available. Typically this is only available on NPC references.

`cell <tes3reference/cell.html>`_ (`tes3cell`_)
    The cell that the reference is currently in.

`context <tes3reference/context.html>`_ (`tes3scriptContext`_)
    Access to the script context for this reference and its associated script.

`data <tes3reference/data.html>`_ (`table`_)
    A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For item references, this is the same table as on the tes3itemData structure. To store data that doesn't get serialized to/from the save, use tempData.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`destination <tes3reference/destination.html>`_ (`tes3travelDestinationNode`_, `nil`_)
    Returns the travel destination node for this reference, or nil. This can be used to determine where a given door links to.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`facing <tes3reference/facing.html>`_ (`number`_)
    Convenient access to the z-component of the reference's orientation. Setting the facing sets the reference as modified.

`hasNoCollision <tes3reference/hasNoCollision.html>`_ (`boolean`_)
    Sets the no-collision flag on this reference, and recalculates collision groups. Use the setNoCollisionFlag function to manage collision group recalculation instead.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`isDead <tes3reference/isDead.html>`_ (`boolean`_, `nil`_)
    Returns true if the object is dead, false if they are alive, or nil if that couldn't be determined.

`isEmpty <tes3reference/isEmpty.html>`_ (`boolean`_)
    Friendly access onto the reference's empty inventory flag.

`isLeveledSpawn <tes3reference/isLeveledSpawn.html>`_ (`boolean`_)
    If true, this reference was created as a result of a leveled spawn.

`isRespawn <tes3reference/isRespawn.html>`_ (`boolean`_)
    If true, the references respawn flag is set.

`itemData <tes3reference/itemData.html>`_ (`tes3itemData`_)
    Gets or sets the attached itemData for this reference. If set to nil, the item data will be unhooked but not deleted.

`leveledBaseReference <tes3reference/leveledBaseReference.html>`_ (`tes3reference`_, `nil`_)
    If this reference is a leveled spawn, this is the leveled creature spawn reference. If this reference wasn't the result of a leveled spawn, the value is nil.

`light <tes3reference/light.html>`_ (`niPointLight`_)
    Direct access to the scene graph light, if a dynamic light is set.

`lockNode <tes3reference/lockNode.html>`_ (`tes3lockNode`_)
    Quick access to the reference's lock node, if any.

`mesh <tes3reference/mesh.html>`_ (`string`_)
    The path to the object's mesh.

`mobile <tes3reference/mobile.html>`_ (`tes3mobileCreature`_, `tes3mobileNPC`_, `tes3mobilePlayer`_, `tes3mobileProjectile`_, `tes3mobileSpell`_, `nil`_)
    Access to the attached mobile object, if applicable.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`nextInCollection <tes3object/nextInCollection.html>`_ (`tes3object`_)
    The next object in parent collection's list.

`nextNode <tes3reference/nextNode.html>`_ (`tes3reference`_)
    The next reference in the parent reference list.

`nodeData <tes3reference/nodeData.html>`_ (`tes3reference`_)
    Redundant access to this object, for iterating over a tes3referenceList.

`object <tes3reference/object.html>`_ (`tes3physicalObject`_)
    The object that the reference is for, such as a weapon, armor, or actor.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`orientation <tes3reference/orientation.html>`_ (`tes3vector3`_)
    Access to the reference's orientation. Setting the orientation sets the reference as modified.

`owningCollection <tes3object/owningCollection.html>`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`position <tes3reference/position.html>`_ (`tes3vector3`_)
    Access to the reference's position. Setting the position sets the reference as modified.

`previousInCollection <tes3object/previousInCollection.html>`_ (`tes3object`_)
    The previous object in parent collection's list.

`previousNode <tes3reference/previousNode.html>`_ (`tes3reference`_)
    The previous reference in the parent reference list.

`scale <tes3object/scale.html>`_ (`number`_)
    The object's scale.

`sceneNode <tes3reference/sceneNode.html>`_ (`niNode`_)
    The scene graph node that the reference uses for rendering.

`sceneReference <tes3object/sceneReference.html>`_ (`niNode`_)
    The scene graph reference node for this object.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`stackSize <tes3reference/stackSize.html>`_ (`number`_)
    Access to the size of a stack, if the reference represents one or more items.

`supportsLuaData <tes3reference/supportsLuaData.html>`_ (`boolean`_)
    If true, this reference can store temporary or persistent lua data.

`tempData <tes3reference/tempData.html>`_ (`table`_)
    As with the data field, a generic lua table that data can be written to. No information in this table will persist into saves. For item references, this is the same table as on the tes3itemData structure.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3reference/activationReference
    tes3reference/attachments
    tes3reference/baseObject
    tes3reference/bodyPartManager
    tes3reference/cell
    tes3reference/context
    tes3reference/data
    tes3reference/destination
    tes3reference/facing
    tes3reference/hasNoCollision
    tes3reference/isDead
    tes3reference/isEmpty
    tes3reference/isLeveledSpawn
    tes3reference/isRespawn
    tes3reference/itemData
    tes3reference/leveledBaseReference
    tes3reference/light
    tes3reference/lockNode
    tes3reference/mesh
    tes3reference/mobile
    tes3reference/nextNode
    tes3reference/nodeData
    tes3reference/object
    tes3reference/orientation
    tes3reference/position
    tes3reference/previousNode
    tes3reference/sceneNode
    tes3reference/stackSize
    tes3reference/supportsLuaData
    tes3reference/tempData

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`activate <tes3reference/activate.html>`_ (`method`_)
    Causes this reference to activate another. This will lead them to go through doors, pick up items, etc.

`clearActionFlag <tes3reference/clearActionFlag.html>`_ (`method`_)
    Unsets a bit in the reference's action data attachment

`clone <tes3reference/clone.html>`_ (`method`_)
    Clones a reference for a base actor into a reference to an instance of that actor. For example, this will force a container to resolve its leveled items and have its own unique inventory.

`delete <tes3reference/delete.html>`_ (`method`_)
    Disables the reference, removes all its attachments, resets its scale, and sets the reference to be deleted.

`deleteDynamicLightAttachment <tes3reference/deleteDynamicLightAttachment.html>`_ (`method`_)
    Deletes the dynamic light attachment, if it exists. This will automatically detach the dynamic light from affected nodes.

`detachDynamicLightFromAffectedNodes <tes3reference/detachDynamicLightFromAffectedNodes.html>`_ (`method`_)
    Removes the dynamic light from any affected scene graph nodes, but will not delete the associated attachment.

`disable <tes3reference/disable.html>`_ (`method`_)
    Hides the reference, stops all scripts for it, detaches all dynamic lights and sounds, and closes any mobile simulation. Sets the reference as modified implicitly.

`enable <tes3reference/enable.html>`_ (`method`_)
    Shows the reference, restarts all scripts for it, reattaches all dynamic lights and sounds, and restarts any mobile simulation if the player is close enough. Sets the reference as modified implicitly.

`getAngleTo <tes3reference/getAngleTo.html>`_ (`method`_)
    Calculates the angle from this reference's current facing to the target reference.

`getAttachedDynamicLight <tes3reference/getAttachedDynamicLight.html>`_ (`method`_)
    Fetches the dynamic light attachment.

`getOrCreateAttachedDynamicLight <tes3reference/getOrCreateAttachedDynamicLight.html>`_ (`method`_)
    Fetches the dynamic light attachment. If there isn't one, a new one will be created with the given light and value.

    If no light is supplied as an argument, a point light of radius 512 will be automatically created.
    
    If the light is not attached to any part of the scene graph yet, the point light will be placed as a child of the "attachLight" subnode of the model, or a child of the model if "attachLight" is not found.

`onCloseInventory <tes3reference/onCloseInventory.html>`_ (`method`_)
    A function that tells the game that an inventory has been closed, if you are simulating inventory operations. It has the capability of un-cloning an inventory if it has not been modified. Typically not used outside of specific purposes.

`setActionFlag <tes3reference/setActionFlag.html>`_ (`method`_)
    Sets a bit in the reference's action data attachment

`setDynamicLighting <tes3reference/setDynamicLighting.html>`_ (`method`_)
    Sets the dynamic lighting state of the reference using the global data handler.

`setNoCollisionFlag <tes3reference/setNoCollisionFlag.html>`_ (`method`_)
    Sets if this reference has active collision. This is preferable to the hasNoCollision property, if are manipulating multiple objects at once and do not want to constantly recalculate collision groups.

`testActionFlag <tes3reference/testActionFlag.html>`_ (`method`_)
    Returns the flag's value in the reference's action data attachment

`updateEquipment <tes3reference/updateEquipment.html>`_ (`method`_)
    Causes the reference, if of an actor, to reevaluate its equipment choices and equip items it should.

`updateLighting <tes3reference/updateLighting.html>`_ (`method`_)
    Updates the lighting of the reference using the global data handler.

`updateSceneGraph <tes3reference/updateSceneGraph.html>`_ (`method`_)
    Updates the reference's local rotation matrix, propagates position changes to the scene graph, and sets the reference's modified flag.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3reference/activate
    tes3reference/clearActionFlag
    tes3reference/clone
    tes3reference/delete
    tes3reference/deleteDynamicLightAttachment
    tes3reference/detachDynamicLightFromAffectedNodes
    tes3reference/disable
    tes3reference/enable
    tes3reference/getAngleTo
    tes3reference/getAttachedDynamicLight
    tes3reference/getOrCreateAttachedDynamicLight
    tes3reference/onCloseInventory
    tes3reference/setActionFlag
    tes3reference/setDynamicLighting
    tes3reference/setNoCollisionFlag
    tes3reference/testActionFlag
    tes3reference/updateEquipment
    tes3reference/updateLighting
    tes3reference/updateSceneGraph

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`niPointLight`: ../../lua/type/niPointLight.html
.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3bodyPartManager`: ../../lua/type/tes3bodyPartManager.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3lockNode`: ../../lua/type/tes3lockNode.html
.. _`tes3mobileCreature`: ../../lua/type/tes3mobileCreature.html
.. _`tes3mobileNPC`: ../../lua/type/tes3mobileNPC.html
.. _`tes3mobilePlayer`: ../../lua/type/tes3mobilePlayer.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3mobileSpell`: ../../lua/type/tes3mobileSpell.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3scriptContext`: ../../lua/type/tes3scriptContext.html
.. _`tes3travelDestinationNode`: ../../lua/type/tes3travelDestinationNode.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
