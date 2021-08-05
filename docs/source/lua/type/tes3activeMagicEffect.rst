tes3activeMagicEffect
====================================================================================================

An active magic effect.

Properties
----------------------------------------------------------------------------------------------------

`attributeId <tes3activeMagicEffect/attributeId.html>`_ (`number`_)
    The attribute ID (note that this may be the skill ID if the effect affects skills).

`duration <tes3activeMagicEffect/duration.html>`_ (`number`_)
    The total duration of this specific effect.

`effectId <tes3activeMagicEffect/effectId.html>`_ (`number`_)
    The magic effect ID.

`effectIndex <tes3activeMagicEffect/effectIndex.html>`_ (`number`_)
    The index of the effect inside the magic source instance.

`effectInstance <tes3activeMagicEffect/effectInstance.html>`_ (`tes3magicEffectInstance`_)
    The magic effect instance for this effect.

`harmful <tes3activeMagicEffect/harmful.html>`_ (`boolean`_)
    This flag that determines if this effect is counted as a hostile action.

`instance <tes3activeMagicEffect/instance.html>`_ (`tes3magicSourceInstance`_)
    The magic source instance for this effect.

`isSummon <tes3activeMagicEffect/isSummon.html>`_ (`boolean`_)
    No description available.

`magnitude <tes3activeMagicEffect/magnitude.html>`_ (`number`_)
    The unresisted magnitude of the magic effect, that is the magnitude before resistance attributes are applied. This is always an integer number. To find the actual applied magnitude, you must check the magic effect instance.

`magnitudeMin <tes3activeMagicEffect/magnitudeMin.html>`_ (`number`_)
    Deprecated.

`mobile <tes3activeMagicEffect/mobile.html>`_ (`tes3magicSourceInstance`_)
    The mobile affected by the magic effect.

`next <tes3activeMagicEffect/next.html>`_ (`tes3activeMagicEffect`_)
    Deprecated.

`previous <tes3activeMagicEffect/previous.html>`_ (`tes3activeMagicEffect`_)
    Deprecated.

`serial <tes3activeMagicEffect/serial.html>`_ (`number`_)
    The magic source instance serial number. See the ``instance`` and ``effectInstance`` properties to easily access the magic and effect instances.

`skillId <tes3activeMagicEffect/skillId.html>`_ (`number`_)
    The skill ID (note that this may be the attribute ID if the effect affects attributes).



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3activeMagicEffect/attributeId
    tes3activeMagicEffect/duration
    tes3activeMagicEffect/effectId
    tes3activeMagicEffect/effectIndex
    tes3activeMagicEffect/effectInstance
    tes3activeMagicEffect/harmful
    tes3activeMagicEffect/instance
    tes3activeMagicEffect/isSummon
    tes3activeMagicEffect/magnitude
    tes3activeMagicEffect/magnitudeMin
    tes3activeMagicEffect/mobile
    tes3activeMagicEffect/next
    tes3activeMagicEffect/previous
    tes3activeMagicEffect/serial
    tes3activeMagicEffect/skillId

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3activeMagicEffect`: ../../lua/type/tes3activeMagicEffect.html
.. _`tes3magicEffectInstance`: ../../lua/type/tes3magicEffectInstance.html
.. _`tes3magicSourceInstance`: ../../lua/type/tes3magicSourceInstance.html
