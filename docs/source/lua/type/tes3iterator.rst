tes3iterator
====================================================================================================

A collection that can be iterated over Contains items in a simple linked list, and stores its head/tail.

Properties
----------------------------------------------------------------------------------------------------

`current <tes3iterator/current.html>`_ (`tes3iteratorNode`_)
    A reference for the currently iterated node. This is used by the core game engine, but should not be accessed from lua.

`head <tes3iterator/head.html>`_ (`tes3iteratorNode`_)
    The first node in the collection.

`size <tes3iterator/size.html>`_ (`number`_)
    The amount of items in the iterator.

`tail <tes3iterator/tail.html>`_ (`tes3iteratorNode`_)
    The last node in the collection.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3iterator/current
    tes3iterator/head
    tes3iterator/size
    tes3iterator/tail

.. _`number`: ../../lua/type/number.html
.. _`tes3iteratorNode`: ../../lua/type/tes3iteratorNode.html
