combatStart
====================================================================================================

The combatStart event occurs when combat is about to begin between two actors. This event allows scripts to prevent combat from starting.

Related events: `combatStarted`_, `combatStop`_, `combatStopped`_

Event Data
----------------------------------------------------------------------------------------------------

actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor who is entering combat.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor who combat is being triggered against.

.. _`combatStarted`: ../../lua/event/combatStarted.html
.. _`combatStop`: ../../lua/event/combatStop.html
.. _`combatStopped`: ../../lua/event/combatStopped.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
