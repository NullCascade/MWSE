tes3effect
====================================================================================================

A structure that defines information for an effect and its associated variables, typically found on spells, alchemy, and enchantments.

Properties
----------------------------------------------------------------------------------------------------

`attribute <tes3effect/attribute.html>`_ (`number`_)
    The attribute associated with this effect, or -1 if no attribute is used.

`cost <tes3effect/cost.html>`_ (`number`_)
    The base magicka cost of this effect.

`duration <tes3effect/duration.html>`_ (`number`_)
    How long the effect should last.

`id <tes3effect/id.html>`_ (`number`_)
    The base tes3magicEffect ID that the effect uses.

`max <tes3effect/max.html>`_ (`number`_)
    The maximum magnitude of the effect.

`min <tes3effect/min.html>`_ (`number`_)
    The minimum magnitude of the effect.

`object <tes3effect/object.html>`_ (`tes3magicEffect`_)
    Fetches the tes3magicEffect for the given id used.

`radius <tes3effect/radius.html>`_ (`number`_)
    The radius of the effect.

`rangeType <tes3effect/rangeType.html>`_ (`number`_)
    Determines if the effect is self, touch, or target ranged.

`skill <tes3effect/skill.html>`_ (`number`_)
    The skill associated with this effect, or -1 if no skill is used.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3effect/attribute
    tes3effect/cost
    tes3effect/duration
    tes3effect/id
    tes3effect/max
    tes3effect/min
    tes3effect/object
    tes3effect/radius
    tes3effect/rangeType
    tes3effect/skill

.. _`number`: ../../lua/type/number.html
.. _`tes3magicEffect`: ../../lua/type/tes3magicEffect.html
