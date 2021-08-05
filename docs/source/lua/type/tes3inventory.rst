tes3inventory
====================================================================================================

An inventory composes of an iterator, and flags caching the state of the inventory.

Properties
----------------------------------------------------------------------------------------------------

`flags <tes3inventory/flags.html>`_ (`number`_)
    Raw bit-based flags.

`items <tes3inventory/items.html>`_ (`tes3iterator`_)
    Direct acces to the container that holds the inventory's items.

`iterator <tes3inventory/iterator.html>`_ (`tes3iterator`_)
    Deprecated. Use ``for index, stack in pairs(inventory)`` to iterate over inventory stacks.
	
Direct access to the container that holds the inventory's items.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3inventory/flags
    tes3inventory/items
    tes3inventory/iterator

Methods
----------------------------------------------------------------------------------------------------

`addItem <tes3inventory/addItem.html>`_ (`method`_)
    Adds an item into the inventory directly. This should not be used, in favor of the tes3.addItem function.

`calculateWeight <tes3inventory/calculateWeight.html>`_ (`method`_)
    Calculates the weight of all items in the container.

`contains <tes3inventory/contains.html>`_ (`method`_)
    Checks to see if the inventory contains an item.

`dropItem <tes3inventory/dropItem.html>`_ (`method`_)
    Checks to see if the inventory contains an item. This should not be used, instead use the tes3.dropItem function.

`findItemStack <tes3inventory/findItemStack.html>`_ (`method`_)
    Searches for an item stack in the inventory.

`removeItem <tes3inventory/removeItem.html>`_ (`method`_)
    Removes an item from the inventory directly. This should not be used, in favor of the tes3.removeItem function.

`resolveLeveledItems <tes3inventory/resolveLeveledItems.html>`_ (`method`_)
    Resolves all contained leveled lists and adds the randomized items to the inventory. This should generally not be called directly.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3inventory/addItem
    tes3inventory/calculateWeight
    tes3inventory/contains
    tes3inventory/dropItem
    tes3inventory/findItemStack
    tes3inventory/removeItem
    tes3inventory/resolveLeveledItems

.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
