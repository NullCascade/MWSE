
weatherTransitionFinished
========================================================

The **weatherTransitionFinished** event occurs when the currently simulated weather finished transitioning to a new weather. 

Weather will continue transitioning in interiors, and may finish while in an interior. The transition may be interrupted at any time by the weatherChangedImmediate event, and this finish event will not be triggered.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

to
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Weather`_. The weather object that will be transitioned to.



Filter
--------------------------------------------------------
This event does not support filters.


.. _`Event Guide`: ../guide/events.html
.. _`Weather`: ../type/tes3/weather.html
