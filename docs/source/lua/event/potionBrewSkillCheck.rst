potionBrewSkillCheck
====================================================================================================

This event is triggered when the player attempts to brew a potion. It controls both the strength of the potion's effects, and if the brew is successful. These both use the player's stats, so the game combines them into one function. Because the potion effects are not determined yet, the potion data is not available. When using this event, use the example as a template for your own code.

Event Data
----------------------------------------------------------------------------------------------------

alembic
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The alembic apparatus used to create the potion.

calcinator
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The calcinator apparatus used to create the potion.

mortar
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The mortar apparatus used to create the potion.

potionStrength
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Can be modified. The strength multiplier used to scale the potion effects. For each different effect, it can modify magnitude, duration, or both. This is initially equal to -1 if the potion failed to brew.

retort
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The retort apparatus used to create the potion.

success
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Can be modified. Whether the brewing is successful.

Examples
----------------------------------------------------------------------------------------------------

Vanilla potion skill check function
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua

    local function brewSkillCheck(e)
        local player = tes3.mobilePlayer
        local x = player.alchemy.current + 0.1 * player.intelligence.current + 0.1 * player.luck.current

        local roll = math.floor(100 * math.random())
        if (roll <= x) then
            local fPotionStrengthMult = tes3.findGMST(tes3.gmst.fPotionStrengthMult).value
            e.potionStrength = fPotionStrengthMult * e.mortar.quality * x
            e.success = true
        else
            e.potionStrength = -1
            e.success = false
        end
    end

    event.register("potionBrewSkillCheck", brewSkillCheck)


.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3apparatus`: ../../lua/type/tes3apparatus.html
