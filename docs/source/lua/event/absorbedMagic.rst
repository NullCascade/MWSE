absorbedMagic
====================================================================================================

This event is triggered after magic absorption absorbs and cancels a magic effect, and just before magic absorption gives magicka to the target. It can control the amount of magicka restored.

This event occurs once per effect restored, so a multi-effect spell may trigger this multiple times. The default amount restored is equal to the casting cost of the entire spell, which is the vanilla behaviour.

Event Data
----------------------------------------------------------------------------------------------------

absorb
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The amount of magicka to restore to the actor.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor that absorbed the spell.

source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3alchemy|tes3enchantment|tes3spell. Read-only. The magic source.

sourceInstance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3magicSourceInstance`_. Read-only. The unique instance of the magic source.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The actor that absorbed the spell.

.. _`number`: ../../lua/type/number.html
.. _`tes3magicSourceInstance`: ../../lua/type/tes3magicSourceInstance.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
