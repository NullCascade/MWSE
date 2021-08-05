tes3nonDynamicData
====================================================================================================

A child container from tes3dataHandler, where game data is stored.

Properties
----------------------------------------------------------------------------------------------------

`birthsigns <tes3nonDynamicData/birthsigns.html>`_ (`tes3iterator`_)
    A collection of all birthsign objects.

`cells <tes3nonDynamicData/cells.html>`_ (`tes3stlList`_)
    A collection of all cell objects.

`classes <tes3nonDynamicData/classes.html>`_ (`tes3iterator`_)
    A collection of all class objects.

`dialogues <tes3nonDynamicData/dialogues.html>`_ (`tes3iterator`_)
    A collection of all dialogue objects.

`factions <tes3nonDynamicData/factions.html>`_ (`tes3iterator`_)
    A collection of all faction objects.

`globals <tes3nonDynamicData/globals.html>`_ (`tes3iterator`_)
    A collection of all global variable objects.

`magicEffects <tes3nonDynamicData/magicEffects.html>`_ (`table`_)
    A table of references to all 143 magic effects.

`objects <tes3nonDynamicData/objects.html>`_ (`tes3linkedList`_)
    A collection of all other game objects.

`races <tes3nonDynamicData/races.html>`_ (`tes3iterator`_)
    A collection of all race objects.

`regions <tes3nonDynamicData/regions.html>`_ (`tes3iterator`_)
    A collection of all region objects.

`scripts <tes3nonDynamicData/scripts.html>`_ (`tes3iterator`_)
    A collection of all script objects.

`skills <tes3nonDynamicData/skills.html>`_ (`table`_)
    A table of references to all 27 skills.

`soundGenerators <tes3nonDynamicData/soundGenerators.html>`_ (`tes3iterator`_)
    A collection of all sound generator objects.

`sounds <tes3nonDynamicData/sounds.html>`_ (`tes3iterator`_)
    A collection of all sound objects.

`spells <tes3nonDynamicData/spells.html>`_ (`tes3linkedList`_)
    A collection of all spell objects.

`startScripts <tes3nonDynamicData/startScripts.html>`_ (`tes3iterator`_)
    A collection of all tes3startScript objects.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3nonDynamicData/birthsigns
    tes3nonDynamicData/cells
    tes3nonDynamicData/classes
    tes3nonDynamicData/dialogues
    tes3nonDynamicData/factions
    tes3nonDynamicData/globals
    tes3nonDynamicData/magicEffects
    tes3nonDynamicData/objects
    tes3nonDynamicData/races
    tes3nonDynamicData/regions
    tes3nonDynamicData/scripts
    tes3nonDynamicData/skills
    tes3nonDynamicData/soundGenerators
    tes3nonDynamicData/sounds
    tes3nonDynamicData/spells
    tes3nonDynamicData/startScripts

Methods
----------------------------------------------------------------------------------------------------

`addNewObject <tes3nonDynamicData/addNewObject.html>`_ (`method`_)
    Inserts a newly created object into the proper collections.

`deleteObject <tes3nonDynamicData/deleteObject.html>`_ (`method`_)
    Removes an object from the proper collections.

`drawCellMapMarker <tes3nonDynamicData/drawCellMapMarker.html>`_ (`method`_)
    Draws a map marker box for an exterior cell onto the world map.

`findDialogue <tes3nonDynamicData/findDialogue.html>`_ (`method`_)
    Locates a dialogue for a given ID.

`findFirstCloneOfActor <tes3nonDynamicData/findFirstCloneOfActor.html>`_ (`method`_)
    Locates the first reference for a given object ID.

`findGlobalVariable <tes3nonDynamicData/findGlobalVariable.html>`_ (`method`_)
    Locates a global variable for a given ID.

`findScript <tes3nonDynamicData/findScript.html>`_ (`method`_)
    Locates a script for a given ID.

`findSound <tes3nonDynamicData/findSound.html>`_ (`method`_)
    Locates a sound for a given ID.

`resolveObject <tes3nonDynamicData/resolveObject.html>`_ (`method`_)
    Locates a general object for a given ID.

`showLocationOnMap <tes3nonDynamicData/showLocationOnMap.html>`_ (`method`_)
    Draws map marker boxes onto the world map for all cells with names starting with the prefix. e.g. showLocationOnMap("Vivec") draws boxes for all cells in Vivec, because the cell names "Vivec, Arena", "Vivec, Temple", etc. matches.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3nonDynamicData/addNewObject
    tes3nonDynamicData/deleteObject
    tes3nonDynamicData/drawCellMapMarker
    tes3nonDynamicData/findDialogue
    tes3nonDynamicData/findFirstCloneOfActor
    tes3nonDynamicData/findGlobalVariable
    tes3nonDynamicData/findScript
    tes3nonDynamicData/findSound
    tes3nonDynamicData/resolveObject
    tes3nonDynamicData/showLocationOnMap

.. _`method`: ../../lua/type/method.html
.. _`table`: ../../lua/type/table.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3linkedList`: ../../lua/type/tes3linkedList.html
.. _`tes3stlList`: ../../lua/type/tes3stlList.html
