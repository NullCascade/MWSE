
Container Instance
========================================================

An **Container Instance** is a cloned `Container`_.


Properties
--------------------------------------------------------

**actorFlags** (`number`_, read-only)
    The actor's flags, including information on essential and respawn status.

**barterGold** (`number`_)
    The base gold that the actor has to barter with.

**baseObject** (`tes3container`_)
    The `Container`_ that this instance was cloned from.

**boundingBox** (`tes3boundingBox`_, read-only)
    The object's bounding box.

**capacity** (`number`_)
    The weight that the container can hold.

**cloneCount** (`number`_, read-only)
    The number of clones that this actor has.

**equipment** (`tes3iterator`_ of `tes3equipmentStack`_, read-only)
    Items currently equipped on the actor.

**id** (`string`_, read-only)
    The object's unique id.

**inventory** (`tes3inventory`_, read-only)
    Items the actor currently carries.

**isInstance** (`boolean`_, read-only)
    Always return ``true``. This is useful because **objectType** is the same for both `Container`_ and **Container Instance**, while this field allows differentiation.

**model** (`string`_)
    The mesh used by the container.

**name** (`string`_)
    The user-friendly name shown for the container.

**objectType** (`number`_, read-only)
    The object's `objectType`_.

**organic** (`boolean`_)
    Quick access to the container's flags, allowing read/write access to the container's organic status.

**respawns** (`boolean`_)
    Quick access to the container's flags, allowing read/write access to the container's respawn status.

**script** (`tes3script`_, read-only)
    The script, if any, that is attached to the object.

**sourceMod** (`string`_, read-only)
    The object's originating plugin filename.


.. _`boolean`: ../lua/boolean.html
.. _`number`: ../lua/number.html
.. _`string`: ../lua/string.html
.. _`table`: ../lua/table.html
.. _`userdata`: ../lua/userdata.html

.. _`Actor`: actor.html
.. _`Container`: container.html
.. _`Creature Instance`: creatureInstance.html
.. _`Creature`: creature.html
.. _`Mobile Actor`: mobileActor.html
.. _`NPC Instance`: npcInstance.html
.. _`NPC`: npc.html
.. _`objectType`: baseObject/objectType.html
.. _`tes3boundingBox`: boundingBox.html
.. _`tes3container`: container.html
.. _`tes3equipmentStack`: equipmentStack.html
.. _`tes3inventory`: inventory.html
.. _`tes3iterator`: iterator.html
.. _`tes3script`: script.html
