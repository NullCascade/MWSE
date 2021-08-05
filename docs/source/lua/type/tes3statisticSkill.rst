tes3statisticSkill
====================================================================================================

A structure that holds statistical information relating to the current and base values of a skill statistic.

Morrowind statistics do not track buffs and debuffs separately. Instead, total buff or debuff values can be calculated with the tes3.getEffectMagnitude function, using the appropriate buff or debuff magic effect for the statistic.

This type inherits from the following parent types: `tes3statistic`_

Properties
----------------------------------------------------------------------------------------------------

`base <tes3statisticSkill/base.html>`_ (`number`_)
    The base value of the skill statistic. Damage, and fortify spell effects don't change base skill value.

`baseRaw <tes3statisticSkill/baseRaw.html>`_ (`number`_)
    The base value of the skill statistic, as stored in memory. Prefer to use .base unless you know what you're doing.

`current <tes3statisticSkill/current.html>`_ (`number`_)
    The current value of the skill statistic.

`currentRaw <tes3statisticSkill/currentRaw.html>`_ (`number`_)
    The current value of the skill statistic, as stored in memory. Prefer to use .current unless you know what you're doing.

`normalized <tes3statisticSkill/normalized.html>`_ (`number`_)
    The normalized value of the skill statistic.

`type <tes3statisticSkill/type.html>`_ (`number`_)
    No description available.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3statisticSkill/base
    tes3statisticSkill/baseRaw
    tes3statisticSkill/current
    tes3statisticSkill/currentRaw
    tes3statisticSkill/normalized
    tes3statisticSkill/type

.. _`number`: ../../lua/type/number.html
.. _`tes3statistic`: ../../lua/type/tes3statistic.html
