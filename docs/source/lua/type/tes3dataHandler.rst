tes3dataHandler
====================================================================================================

A core game object used for storing both active and non-dynamic gameplay data.

Properties
----------------------------------------------------------------------------------------------------

`backgroundThread <tes3dataHandler/backgroundThread.html>`_ (`number`_)
    A Windows handle to the background processing thread.

`backgroundThreadId <tes3dataHandler/backgroundThreadId.html>`_ (`number`_)
    The thread ID for the background processing thread.

`backgroundThreadRunning <tes3dataHandler/backgroundThreadRunning.html>`_ (`boolean`_)
    Access to the running state for the background processing thread.

`cellChanged <tes3dataHandler/cellChanged.html>`_ (`boolean`_)
    A flag set for the frame that the player has changed cells.

`centralGridX <tes3dataHandler/centralGridX.html>`_ (`boolean`_)
    The position of the origin horizontal grid coordinate.

`centralGridY <tes3dataHandler/centralGridY.html>`_ (`boolean`_)
    The position of the origin longitudinal grid coordinate.

`currentAmbientWaterSound <tes3dataHandler/currentAmbientWaterSound.html>`_ (`tes3sound`_)
    Access to the current ambient water sound.

`currentCell <tes3dataHandler/currentCell.html>`_ (`tes3cell`_)
    Access to the cell that the player is currently in.

`currentInteriorCell <tes3dataHandler/currentInteriorCell.html>`_ (`tes3cell`_)
    Access to the current interior cell, if the player is in an interior.

`exteriorCells <tes3dataHandler/exteriorCells.html>`_ (`table`_)
    A table of nine tes3cellExteriorData objects for any loaded exterior cells.

`lastExteriorCell <tes3dataHandler/lastExteriorCell.html>`_ (`tes3cell`_)
    Access to the last visited exterior cell.

`mainThread <tes3dataHandler/mainThread.html>`_ (`number`_)
    A Windows handle to the main execution thread.

`mainThreadId <tes3dataHandler/mainThreadId.html>`_ (`number`_)
    The thread ID for the main execution thread.

`nonDynamicData <tes3dataHandler/nonDynamicData.html>`_ (`tes3nonDynamicData`_)
    A child structure where core game objects are held.

`threadSleepTime <tes3dataHandler/threadSleepTime.html>`_ (`number`_)
    No description available.

`useCellTransitionFader <tes3dataHandler/useCellTransitionFader.html>`_ (`boolean`_)
    An engine flag that controls if there is a fade in/out between cells.

`worldLandscapeRoot <tes3dataHandler/worldLandscapeRoot.html>`_ (`niNode`_)
    One of the core parent scene graph nodes.

`worldObjectRoot <tes3dataHandler/worldObjectRoot.html>`_ (`niNode`_)
    One of the core parent scene graph nodes.

`worldPickObjectRoot <tes3dataHandler/worldPickObjectRoot.html>`_ (`niNode`_)
    One of the core parent scene graph nodes.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3dataHandler/backgroundThread
    tes3dataHandler/backgroundThreadId
    tes3dataHandler/backgroundThreadRunning
    tes3dataHandler/cellChanged
    tes3dataHandler/centralGridX
    tes3dataHandler/centralGridY
    tes3dataHandler/currentAmbientWaterSound
    tes3dataHandler/currentCell
    tes3dataHandler/currentInteriorCell
    tes3dataHandler/exteriorCells
    tes3dataHandler/lastExteriorCell
    tes3dataHandler/mainThread
    tes3dataHandler/mainThreadId
    tes3dataHandler/nonDynamicData
    tes3dataHandler/threadSleepTime
    tes3dataHandler/useCellTransitionFader
    tes3dataHandler/worldLandscapeRoot
    tes3dataHandler/worldObjectRoot
    tes3dataHandler/worldPickObjectRoot

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3nonDynamicData`: ../../lua/type/tes3nonDynamicData.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
