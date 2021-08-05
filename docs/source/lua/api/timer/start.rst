timer.start
====================================================================================================

Creates a timer.

Returns
----------------------------------------------------------------------------------------------------

timer (`mwseTimer`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

type (`number`_)
    Default: ``timer.simulate``. Type of the timer. This value can be timer.simulate, timer.game or timer.real.

duration (`number`_)
    Duration of the timer. The method of time passing depends on the timer type.

callback (`function`_)
    The callback function that will execute when the timer expires.

iterations (`number`_)
    Default: ``1``. The number of iterations to run. Use ``-1`` for infinite looping.

.. _`function`: ../../../lua/type/function.html
.. _`mwseTimer`: ../../../lua/type/mwseTimer.html
.. _`number`: ../../../lua/type/number.html
