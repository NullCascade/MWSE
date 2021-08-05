tes3itemData
====================================================================================================

A set of variables that differentiates one item from another.

Properties
----------------------------------------------------------------------------------------------------

`charge <tes3itemData/charge.html>`_ (`number`_)
    The charge of the item. Provides incorrect values on misc items, which instead have a soul actor.

`condition <tes3itemData/condition.html>`_ (`number`_)
    The condition/health of the item. Provides incorrect values on light items, which instead have a timeLeft property.

`context <tes3itemData/context.html>`_ (`tes3scriptContext`_)
    Returns an ease of use script context for variable access.

`count <tes3itemData/count.html>`_ (`number`_)
    The total number of identical items represented by this data. Reserved for use by in-world references. It should always be 1 for items in inventories. Inventories utilize the stack count in tes3itemStack.

`data <tes3itemData/data.html>`_ (`table`_)
    A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For data that doesn't persist, use tempData.

`owner <tes3itemData/owner.html>`_ (`tes3npc`_, `tes3faction`_, `nil`_)
    The script associated with the scriptVariables.

`requirement <tes3itemData/requirement.html>`_ (`tes3globalVariable`_, `number`_, `nil`_)
    A requirement, typically associated with ownership and when the player may freely interact with an object. The type depends on the owner. Faction owners provide a required rank as a number, while NPCs provide a global variable to check.

`script <tes3itemData/script.html>`_ (`tes3script`_)
    The script associated with the scriptVariables.

`scriptVariables <tes3itemData/scriptVariables.html>`_ (`tes3scriptVariables`_)
    Access to the structure where individual mwscript data is stored.

`soul <tes3itemData/soul.html>`_ (`tes3actor`_)
    Only available on misc items. The actor that is stored inside the soul gem.

`tempData <tes3itemData/tempData.html>`_ (`table`_)
    A generic lua table that data can be written to, but is not synced to/from a save.

`timeLeft <tes3itemData/timeLeft.html>`_ (`number`_)
    The time remaining on a light. Provides incorrect values on non-light items, which instead have a condition property.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3itemData/charge
    tes3itemData/condition
    tes3itemData/context
    tes3itemData/count
    tes3itemData/data
    tes3itemData/owner
    tes3itemData/requirement
    tes3itemData/script
    tes3itemData/scriptVariables
    tes3itemData/soul
    tes3itemData/tempData
    tes3itemData/timeLeft

.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3actor`: ../../lua/type/tes3actor.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
.. _`tes3globalVariable`: ../../lua/type/tes3globalVariable.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
.. _`tes3script`: ../../lua/type/tes3script.html
.. _`tes3scriptContext`: ../../lua/type/tes3scriptContext.html
.. _`tes3scriptVariables`: ../../lua/type/tes3scriptVariables.html
