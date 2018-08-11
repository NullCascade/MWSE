
weatherTransitionStarted
========================================================

The **weatherTransitionStarted** event occurs when the currently simulated weather has started to gradually transition to a new weather. This can occur when moving between regions, or when the weather cycles.

Weather will not start transitioning in interiors, but will continue transitioning, and may finish while in an interior. The transition may be interrupted at any time by the weatherChangedImmediate event, and the weatherTransitionFinished event will not occur.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

from
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Weather`_. The weather object that is the current weather.

to
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Weather`_. The weather object that will be transitioned to.



Filter
--------------------------------------------------------
This event does not support filters.


.. _`Event Guide`: ../guide/events.html
.. _`Weather`: ../type/tes3/weather.html
