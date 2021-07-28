combatStarted
====================================================================================================

The **combatStarted** event occurs after combat has began between two actors. This event is preceded by the `combatStart`_ event.

Related events: `combatStart`_, `combatStop`_, `combatStopped`_

Event Data
----------------------------------------------------------------------------------------------------

actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor who has entered combat.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor who combat has been triggered against.

.. _`combatStart`: ../../lua/event/combatStart.html
.. _`combatStop`: ../../lua/event/combatStop.html
.. _`combatStopped`: ../../lua/event/combatStopped.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
