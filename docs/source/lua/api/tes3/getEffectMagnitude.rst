tes3.getEffectMagnitude
====================================================================================================

This function returns the total magnitude and total unresisted magnitude of a certain spell effect affecting a reference. It returns a pair of numbers, the first being the post-resistance magnitude (see examples). The unresisted magnitude is the magnitude before the actor's resistances are applied; it is always an integer, so it is used in some UI elements.

Returns
----------------------------------------------------------------------------------------------------

magnitude, unresistedMagnitude (`number, number`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    An associated mobile should exist for this function to be able to work.

effect (`tes3.effect.* constants`_)
    Effect ID. Can be any of the predefined spell effects, or one added by tes3.claimSpellEffectId.

skill (`tes3.skill.* constants`_)
    Default: ``-1``. If effect parameter specified is: Absorb, Damage, Drain, Fortify or Restore Skill, a skill should be provided. This also applies to any custom spell effect which operates on a certain skill.

attribute (`tes3.attribute.* constants`_)
    Default: ``-1``. If effect parameter specified is: Absorb, Damage, Drain, Fortify or Restore Attribute, an attribute should be provided. This also applies to any custom spell effect which operates on a certain attribute.

.. _`number, number`: ../../../lua/type/number, number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3.attribute.* constants`: ../../../lua/type/tes3.attribute.* constants.html
.. _`tes3.effect.* constants`: ../../../lua/type/tes3.effect.* constants.html
.. _`tes3.skill.* constants`: ../../../lua/type/tes3.skill.* constants.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
