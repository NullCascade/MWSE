absorbMagic
====================================================================================================

This event is triggered just before magic absorption gives magicka to the target, and after the absorb chance test. It can control the amount of magicka restored. It can also be blocked, though the absorb visual effect will remain.

This event occurs once per effect restored, so a multi-effect spell may trigger this multiple times. The default amount restored is equal to the casting cost of the entire spell, which is the vanilla behaviour.

Event Data
----------------------------------------------------------------------------------------------------

absorb
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The amount of magicka to restore to the actor.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor absorbing the spell.

source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3alchemy|tes3enchantment|tes3spell. Read-only. The magic source.

sourceInstance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3magicSourceInstance`_. Read-only. The unique instance of the magic source.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The actor absorbing the spell.

.. _`number`: ../../lua/type/number.html
.. _`tes3magicSourceInstance`: ../../lua/type/tes3magicSourceInstance.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
