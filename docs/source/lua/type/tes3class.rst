tes3class
====================================================================================================

A core object representing a character class.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`attributes <tes3class/attributes.html>`_ (`table`_)
    An array-style table of the two attribute IDs associated with the class.

`bartersAlchemy <tes3class/bartersAlchemy.html>`_ (`boolean`_)
    If true, the class will barter alchemy items.

`bartersApparatus <tes3class/bartersApparatus.html>`_ (`boolean`_)
    If true, the class will barter apparatus items.

`bartersArmor <tes3class/bartersArmor.html>`_ (`boolean`_)
    If true, the class will barter armor items.

`bartersBooks <tes3class/bartersBooks.html>`_ (`boolean`_)
    If true, the class will barter book items.

`bartersClothing <tes3class/bartersClothing.html>`_ (`boolean`_)
    If true, the class will barter clothing items.

`bartersEnchantedItems <tes3class/bartersEnchantedItems.html>`_ (`boolean`_)
    If true, the class will barter enchanted items.

`bartersIngredients <tes3class/bartersIngredients.html>`_ (`boolean`_)
    If true, the class will barter ingredient items.

`bartersLights <tes3class/bartersLights.html>`_ (`boolean`_)
    If true, the class will barter light items.

`bartersLockpicks <tes3class/bartersLockpicks.html>`_ (`boolean`_)
    If true, the class will barter lockpick items.

`bartersMiscItems <tes3class/bartersMiscItems.html>`_ (`boolean`_)
    If true, the class will barter misc items.

`bartersProbes <tes3class/bartersProbes.html>`_ (`boolean`_)
    If true, the class will barter probe items.

`bartersRepairTools <tes3class/bartersRepairTools.html>`_ (`boolean`_)
    If true, the class will barter repair items.

`bartersWeapons <tes3class/bartersWeapons.html>`_ (`boolean`_)
    If true, the class will barter weapon items.

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`description <tes3class/description.html>`_ (`string`_)
    Loads from disk and returns the description of the class.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`majorSkills <tes3class/majorSkills.html>`_ (`table`_)
    An array-style table of the 5 skills IDs associated with the class' major skills.

`minorSkills <tes3class/minorSkills.html>`_ (`table`_)
    An array-style table of the 5 skills IDs associated with the class' major skills.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3class/name.html>`_ (`string`_)
    The player-facing name for the object.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`offersEnchanting <tes3class/offersEnchanting.html>`_ (`boolean`_)
    If true, the class will offer repair services.

`offersRepairs <tes3class/offersRepairs.html>`_ (`boolean`_)
    If true, the class will offer enchanting services.

`offersSpellmaking <tes3class/offersSpellmaking.html>`_ (`boolean`_)
    If true, the class will offer spellmaking services.

`offersSpells <tes3class/offersSpells.html>`_ (`boolean`_)
    If true, the class will offer spell selling services.

`offersTraining <tes3class/offersTraining.html>`_ (`boolean`_)
    If true, the class will offer spell training services.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`playable <tes3class/playable.html>`_ (`boolean`_)
    If true, the class is selectable at character generation.

`services <tes3class/services.html>`_ (`number`_)
    The services offered by the class. This is a bit field, and its values should typically be accessed through values such as bartersAlchemy.

`skills <tes3class/skills.html>`_ (`table`_)
    An array-style table of the 10 skills IDs associated with the class. For major or minor skills specifically, use the majorSkills and MinorSkills properties.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`specialization <tes3class/specialization.html>`_ (`number`_)
    The specialization for the class. Maps to the tes3.specialization table.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3class/attributes
    tes3class/bartersAlchemy
    tes3class/bartersApparatus
    tes3class/bartersArmor
    tes3class/bartersBooks
    tes3class/bartersClothing
    tes3class/bartersEnchantedItems
    tes3class/bartersIngredients
    tes3class/bartersLights
    tes3class/bartersLockpicks
    tes3class/bartersMiscItems
    tes3class/bartersProbes
    tes3class/bartersRepairTools
    tes3class/bartersWeapons
    tes3class/description
    tes3class/majorSkills
    tes3class/minorSkills
    tes3class/name
    tes3class/offersEnchanting
    tes3class/offersRepairs
    tes3class/offersSpellmaking
    tes3class/offersSpells
    tes3class/offersTraining
    tes3class/playable
    tes3class/services
    tes3class/skills
    tes3class/specialization

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.



.. toctree::
    :hidden:
    :maxdepth: 1


.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
