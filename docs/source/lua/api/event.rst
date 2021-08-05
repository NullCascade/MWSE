event
====================================================================================================

The event library helps to instruct mwse to call a given function when a specific action is taken in the game.

Functions
----------------------------------------------------------------------------------------------------

`clear <event/clear.html>`_ (`function`_)
    Removes all callbacks registered for a given event.

`register <event/register.html>`_ (`function`_)
    Registers a function to be called when an event is raised.

`trigger <event/trigger.html>`_ (`function`_)
    Triggers an event. This can be used to trigger custom events with specific data.

`unregister <event/unregister.html>`_ (`function`_)
    Unregisters a function  event is raised.



.. toctree::
    :hidden:
    :maxdepth: 1

    event/clear
    event/register
    event/trigger
    event/unregister

.. _`function`: ../../lua/type/function.html
