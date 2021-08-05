timer
====================================================================================================

The timer library provides helper functions for creating delayed executors.

Values
----------------------------------------------------------------------------------------------------

`active <timer/active.html>`_ (`any`_)
    Constant to represent a timer that is actively running.

`expired <timer/expired.html>`_ (`any`_)
    Constant to represent a timer that has completed.

`game <timer/game.html>`_ (`any`_)
    Constant to represent timers that run based on in-world time. Duration measured in game-scale hours.

`paused <timer/paused.html>`_ (`any`_)
    Constant to represent a timer that is paused.

`real <timer/real.html>`_ (`any`_)
    Constant to represent timers that run in real-time.

`simulate <timer/simulate.html>`_ (`any`_)
    Constant to represent timers that run when the game isn't paused. It matches the simulate event's timing. If the game is simulating, simulate events and simulate timers progress. Duration is measured in seconds.



.. toctree::
    :hidden:
    :maxdepth: 1

    timer/active
    timer/expired
    timer/game
    timer/paused
    timer/real
    timer/simulate

Functions
----------------------------------------------------------------------------------------------------

`delayOneFrame <timer/delayOneFrame.html>`_ (`function`_)
    Creates a timer that will finish the next frame. It defaults to the next simulation frame.

`start <timer/start.html>`_ (`function`_)
    Creates a timer.



.. toctree::
    :hidden:
    :maxdepth: 1

    timer/delayOneFrame
    timer/start

.. _`any`: ../../lua/type/any.html
.. _`function`: ../../lua/type/function.html
