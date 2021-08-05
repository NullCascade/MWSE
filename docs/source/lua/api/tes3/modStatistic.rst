tes3.modStatistic
====================================================================================================

Modifies a statistic on a given actor. This should be used instead of manually setting values on the game structures, to ensure that events and GUI elements are properly handled. Either skill, attribute, or name must be provided.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3mobileActor`_, `tes3reference`_, `string`_)
    No description available.

name (`string`_)
    Optional. A generic name of an attribute to set.

attribute (`number`_)
    Optional. The attribute to set.

skill (`number`_)
    Optional. The skill to set.

base (`number`_)
    Optional. If set, the base value will be modified.

current (`number`_)
    Optional. If set, the current value will be modified.

value (`number`_)
    Optional. If set, both the base and current value will be modified.

limit (`boolean`_)
    Default: ``false``. If set, the attribute won't rise above 100 or fall below 0.

limitToBase (`boolean`_)
    Default: ``false``. If set, the attribute's current value won't rise above its base value. Useful for health, magicka, and fatigue.

Examples
----------------------------------------------------------------------------------------------------

Decrease Health of an Actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this simple example, the current health of an actor, the player, is decreased by 10. To simulate actual combat damage, you should use tes3mobileActor.applyDamage.

.. code-block:: lua

    tes3.modStatistic({
        reference = tes3.mobilePlayer,
        name = "health",
        current = -10
    })


Restore Magicka without Overflowing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Magicka is restored without exceeding the magicka pool using the limitToBase flag.

.. code-block:: lua

    tes3.modStatistic({
        reference = tes3.mobilePlayer,
        name = "magicka",
        current = 20,
        limitToBase = true
    })


.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
