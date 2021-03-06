
cellChanged
========================================================

The **cellChanged** event is occurs when the player changes cells. This might occur from going through a door, using intervention or recall spells, or from scripted repositioning.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

cell
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`tes3cell`_. Read-only. The new cell that the player has entered.

previousCell
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`tes3cell`_. Read-only. The previous cell that the player came from. This will be ``nil`` when loading a game.


Filter
--------------------------------------------------------
This event is filtered on **cell**. If provided, the event callback will only be invoked when that cell is entered.


.. _`Event Guide`: ../guide/events.html

.. _`tes3cell`: ../type/tes3/cell.html
