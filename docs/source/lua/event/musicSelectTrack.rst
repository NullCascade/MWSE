
musicSelectTrack
========================================================

The **musicSelectTrack** event occurs when new music is needed after a playing music track ends, or the combat situation changes. It allows you to select your own music for the current conditions by setting ``eventData.music``. However, it does not control transitions to combat music, which in the future will be available in another event.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

situation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`number`_. Either ``tes3.musicSituation.explore`` that indicates non-combat music, or ``tes3.musicSituation.combat`` that indicates combat music.

music
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`string`_. Set this to the path of your chosen track (relative to Data Files/music/), or leave it as ``nil`` for the standard random selection.



Filter
--------------------------------------------------------
This event does not support filters.


.. _`number`: ../type/lua/number.html
.. _`string`: ../type/lua/string.html

.. _`Event Guide`: ../guide/events.html
