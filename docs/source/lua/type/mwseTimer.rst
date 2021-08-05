mwseTimer
====================================================================================================

A Timer is a class used to keep track of callback that should be invoked at a later time.

Properties
----------------------------------------------------------------------------------------------------

`callback <mwseTimer/callback.html>`_ (`function`_)
    The callback that will be invoked when the timer elapses.

`duration <mwseTimer/duration.html>`_ (`number`_)
    The amount of time left on the timer.

`iterations <mwseTimer/iterations.html>`_ (`number`_)
    The amount of iterations left for the timer.

`state <mwseTimer/state.html>`_ (`number`_)
    The state of the timer, matching timer.active, timer.paused, or timer.expired.

`timeLeft <mwseTimer/timeLeft.html>`_ (`number`_)
    The amount of time left before this timer will complete.

`timing <mwseTimer/timing.html>`_ (`number`_)
    When this timer ends, or the time remaining if the timer is paused.



.. toctree::
    :hidden:
    :maxdepth: 1

    mwseTimer/callback
    mwseTimer/duration
    mwseTimer/iterations
    mwseTimer/state
    mwseTimer/timeLeft
    mwseTimer/timing

Methods
----------------------------------------------------------------------------------------------------

`cancel <mwseTimer/cancel.html>`_ (`method`_)
    Cancels the timer.

`pause <mwseTimer/pause.html>`_ (`method`_)
    Pauses the timer.

`reset <mwseTimer/reset.html>`_ (`method`_)
    Resets the timer completion time, as if it elapsed. Only works if the timer is active.

`resume <mwseTimer/resume.html>`_ (`method`_)
    Resumes a paused timer.



.. toctree::
    :hidden:
    :maxdepth: 1

    mwseTimer/cancel
    mwseTimer/pause
    mwseTimer/reset
    mwseTimer/resume

.. _`function`: ../../lua/type/function.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
