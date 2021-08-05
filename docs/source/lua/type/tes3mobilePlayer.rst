tes3mobilePlayer
====================================================================================================

A mobile object for a the player.

This type inherits from the following parent types: `tes3mobileNPC`_, `tes3mobileActor`_, `tes3mobileObject`_

Properties
----------------------------------------------------------------------------------------------------

`acrobatics <tes3mobileNPC/acrobatics.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's acrobatics skill statistic.

`actionBeforeCombat <tes3mobileActor/actionBeforeCombat.html>`_ (`tes3actionData`_)
    Action data stored before the actor entered combat.

`actionData <tes3mobileActor/actionData.html>`_ (`tes3actionData`_)
    Current action data. Pre-combat action data is stored in the actionBeforeCombat property.

`activeAI <tes3mobileActor/activeAI.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if AI is active.

`activeMagicEffectList <tes3mobileActor/activeMagicEffectList.html>`_ (`table`_)
    The active magic effects on the actor, from which all others can be accessed. A table with tes3activeMagicEffect items.

`actorType <tes3mobileActor/actorType.html>`_ (`number`_)
    The type of the mobile actor. 0 is a creature, 1 is an NPC, 2 is the player.

`agility <tes3mobileActor/agility.html>`_ (`tes3statistic`_)
    Direct access to the actor's agility attribute statistic.

`aiPlanner <tes3mobileActor/aiPlanner.html>`_ (`tes3aiPlanner`_)
    Access to the mobile's AI planner and AI package information.

`alarm <tes3mobileActor/alarm.html>`_ (`number`_)
    The actor's alarm AI value.

`alchemy <tes3mobileNPC/alchemy.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's alchemy skill statistic.

`alteration <tes3mobileNPC/alteration.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's alteration skill statistic.

`alwaysRun <tes3mobilePlayer/alwaysRun.html>`_ (`boolean`_)
    Toggle flag for if the player should always run.

`animationController <tes3mobileActor/animationController.html>`_ (`tes3actorAnimationController`_)
    No description available.

`armorer <tes3mobileNPC/armorer.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's armorer skill statistic.

`armorRating <tes3mobileActor/armorRating.html>`_ (`number`_)
    The actor's current armour rating, taking equipment condition into account. Armour mitigation can be automatically applied to damage by using the applyDamage function.

Armour mitigation calculation:
x = damage / (damage + target.armorRating)
damage *= max(fCombatArmorMinMult, x)
if damage < 1 then damage = 1 end

`athletics <tes3mobileNPC/athletics.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's athletics skill statistic.

`attackBonus <tes3mobileActor/attackBonus.html>`_ (`number`_)
    Direct access to the actor's attack bonus effect attribute.

`attackDisabled <tes3mobilePlayer/attackDisabled.html>`_ (`boolean`_)
    Toggle flag for if the player can attack.

`attacked <tes3mobileActor/attacked.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if the actor has been attacked.

`attributes <tes3mobileActor/attributes.html>`_ (`table`_)
    Access to a table of 8 tes3statistic objects for the actor's attributes.

`autoRun <tes3mobilePlayer/autoRun.html>`_ (`boolean`_)
    Toggle flag for if the player should constantly run forward.

`axe <tes3mobileNPC/axe.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's axe skill statistic.

`barterGold <tes3mobileActor/barterGold.html>`_ (`number`_)
    The current amount of gold that the actor has access to for bartering.

`birthsign <tes3mobilePlayer/birthsign.html>`_ (`tes3birthsign`_)
    The player's birthsign.

`blind <tes3mobileActor/blind.html>`_ (`number`_)
    Direct access to the actor's blind effect attribute.

`block <tes3mobileNPC/block.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's block skill statistic.

`bluntWeapon <tes3mobileNPC/bluntWeapon.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's blunt weapon skill statistic.

`boundSize <tes3mobileObject/boundSize.html>`_ (`tes3vector3`_)
    A vector that shows the size of the bounding box in each direction.

`bounty <tes3mobilePlayer/bounty.html>`_ (`number`_)
    The player's current bounty.

`cameraHeight <tes3mobilePlayer/cameraHeight.html>`_ (`number`_)
    Access to the player camera height, used for both the first person and third person camera (specifically the orbit centre). It is normally placed at head height by the game, on loading, or when a race is selected. The camera height can be reset to its regular value by setting this property to nil.

`castReady <tes3mobilePlayer/castReady.html>`_ (`boolean`_)
    Toggle flag for if the player has casting ready.

`cell <tes3mobileActor/cell.html>`_ (`tes3cell`_)
    Fetches the cell that the actor is in.

`cellX <tes3mobileObject/cellX.html>`_ (`number`_)
    The X grid coordinate of the cell the mobile is in.

`cellY <tes3mobileObject/cellY.html>`_ (`number`_)
    The Y grid coordinate of the cell the mobile is in.

`chameleon <tes3mobileActor/chameleon.html>`_ (`number`_)
    Direct access to the actor's chameleon effect attribute.

`clawMultiplier <tes3mobilePlayer/clawMultiplier.html>`_ (`tes3globalVariable`_)
    Quick access to the ClawMultiplier global variable.

`collidingReference <tes3mobileActor/collidingReference.html>`_ (`tes3reference`_)
    The reference that the mobile has collided with this frame.

`combatSession <tes3mobileActor/combatSession.html>`_ (`tes3combatSession`_)
    Combat session data. This exists while the actor is in combat to provide memory for AI combat decisions.

`conjuration <tes3mobileNPC/conjuration.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's conjuration skill statistic.

`controlsDisabled <tes3mobilePlayer/controlsDisabled.html>`_ (`boolean`_)
    Toggle flag for if the player's controls are disabled.

`corpseHourstamp <tes3mobileActor/corpseHourstamp.html>`_ (`number`_)
    No description available.

`currentEnchantedItem <tes3mobileActor/currentEnchantedItem.html>`_ (`tes3equipmentStack`_)
    The currently equipped enchanted item that the actor will use.

`currentSpell <tes3mobileActor/currentSpell.html>`_ (`tes3spell`_)
    The currently equipped spell that the actor will use.

`destruction <tes3mobileNPC/destruction.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's destruction skill statistic.

`dialogueList <tes3mobilePlayer/dialogueList.html>`_ (`tes3iterator`_)
    Access to the collection that holds what dialogue topics the player has access to.

`effectAttributes <tes3mobileActor/effectAttributes.html>`_ (`table`_)
    Access to a table of 24 numbers for the actor's effect attributes.

`enchant <tes3mobileNPC/enchant.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's enchant skill statistic.

`encumbrance <tes3mobileActor/encumbrance.html>`_ (`tes3statistic`_)
    Access to the actor's encumbrance statistic.

`endurance <tes3mobileActor/endurance.html>`_ (`tes3statistic`_)
    Direct access to the actor's endurance attribute statistic.

`facing <tes3mobileActor/facing.html>`_ (`number`_)
    The facing of the actor, in radians.

`fatigue <tes3mobileActor/fatigue.html>`_ (`tes3statistic`_)
    Access to the actor's fatigue statistic.

`fight <tes3mobileActor/fight.html>`_ (`number`_)
    The actor's fight AI value.

`firstPerson <tes3mobilePlayer/firstPerson.html>`_ (`tes3npc`_)
    Quick access to the first person NPC.

`firstPersonReference <tes3mobilePlayer/firstPersonReference.html>`_ (`tes3reference`_)
    Quick access to the first person NPC's reference.

`flags <tes3mobileObject/flags.html>`_ (`number`_)
    Access to the root mobile object flags, represented as an integer. Should not be accessed directly.

`flee <tes3mobileActor/flee.html>`_ (`number`_)
    The actor's flee AI value.

`flySpeed <tes3mobileNPC/flySpeed.html>`_ (`number`_)
    The calculated fly movement speed.

`forceJump <tes3mobileNPC/forceJump.html>`_ (`boolean`_)
    Toggle flag for if the NPC jumps.

`forceMoveJump <tes3mobileNPC/forceMoveJump.html>`_ (`boolean`_)
    Toggle flag for if the NPC moves and jumps.

`forceRun <tes3mobileNPC/forceRun.html>`_ (`boolean`_)
    Toggle flag for if the NPC runs.

`forceSneak <tes3mobileNPC/forceSneak.html>`_ (`boolean`_)
    Toggle flag for if the NPC sneaks.

`friendlyActors <tes3mobileActor/friendlyActors.html>`_ (`tes3iterator`_)
    A collection of other tes3mobileActors that this actor considers friendly.

`greetDuration <tes3mobileActor/greetDuration.html>`_ (`number`_)
    No description available.

`greetTimer <tes3mobileActor/greetTimer.html>`_ (`number`_)
    No description available.

`handToHand <tes3mobileNPC/handToHand.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's hand to hand skill statistic.

`health <tes3mobileActor/health.html>`_ (`tes3statistic`_)
    Access to the actor's health statistic.

`heavyArmor <tes3mobileNPC/heavyArmor.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's heavy armor skill statistic.

`height <tes3mobileObject/height.html>`_ (`number`_)
    The height of the mobile above the ground.

`hello <tes3mobileActor/hello.html>`_ (`number`_)
    The actor's hello AI value.

`holdBreathTime <tes3mobileActor/holdBreathTime.html>`_ (`number`_)
    No description available.

`hostileActors <tes3mobileActor/hostileActors.html>`_ (`tes3iterator`_)
    A collection of other tes3mobileActors that this actor considers hostile.

`idleAnim <tes3mobileActor/idleAnim.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if the actor is using their idle animation.

`illusion <tes3mobileNPC/illusion.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's illusion skill statistic.

`impulseVelocity <tes3mobileObject/impulseVelocity.html>`_ (`tes3vector3`_)
    A vector that represents the 3D acceleration of the object.

`inactivityTime <tes3mobilePlayer/inactivityTime.html>`_ (`number`_)
    The time that the player has spent inactive.

`inCombat <tes3mobileActor/inCombat.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if the actor is in combat.

`inJail <tes3mobilePlayer/inJail.html>`_ (`boolean`_)
    Toggle flag for if the player is currently in jail.

`intelligence <tes3mobileActor/intelligence.html>`_ (`tes3statistic`_)
    Direct access to the actor's intelligence attribute statistic.

`invisibility <tes3mobileActor/invisibility.html>`_ (`number`_)
    Direct access to the actor's invisibility effect attribute.

`is3rdPerson <tes3mobilePlayer/is3rdPerson.html>`_ (`boolean`_)
    Shows if the player's camera is currently in 3rd person view.

`isCrittable <tes3mobileActor/isCrittable.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if the actor can be critically hit.

`isDead <tes3mobileActor/isDead.html>`_ (`boolean`_)
    True if the actor is dead.

`isFalling <tes3mobileActor/isFalling.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is falling. This is when the actor is falling without having jumped, e.g. if they walked off a ledge.

`isFlying <tes3mobileActor/isFlying.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is flying.

`isJumping <tes3mobileActor/isJumping.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is jumping.

`isMovingBack <tes3mobileActor/isMovingBack.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving backwards.

`isMovingForward <tes3mobileActor/isMovingForward.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving forwards.

`isMovingLeft <tes3mobileActor/isMovingLeft.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving left.

`isMovingRight <tes3mobileActor/isMovingRight.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving right.

`isPlayerDetected <tes3mobileActor/isPlayerDetected.html>`_ (`boolean`_)
    Direct access to the actor's flag showing the player was detected on the last detection check.

`isPlayerHidden <tes3mobileActor/isPlayerHidden.html>`_ (`boolean`_)
    Direct access to the actor's flag showing the player was hidden on the last detection check.

`isRunning <tes3mobileActor/isRunning.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is running.

`isSneaking <tes3mobileActor/isSneaking.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is sneaking.

`isSwimming <tes3mobileActor/isSwimming.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is swimming.

`isTurningLeft <tes3mobileActor/isTurningLeft.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is turning left.

`isTurningRight <tes3mobileActor/isTurningRight.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is turning right.

`isWalking <tes3mobileActor/isWalking.html>`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is walking.

`jump <tes3mobileActor/jump.html>`_ (`number`_)
    Direct access to the actor's jump effect attribute.

`jumpingDisabled <tes3mobilePlayer/jumpingDisabled.html>`_ (`boolean`_)
    Toggle flag for if the player can jump.

`knownWerewolf <tes3mobilePlayer/knownWerewolf.html>`_ (`tes3globalVariable`_)
    Quick access to the KnownWerewolf global variable.

`lastGroundZ <tes3mobileActor/lastGroundZ.html>`_ (`number`_)
    No description available.

`lastUsedAlembic <tes3mobilePlayer/lastUsedAlembic.html>`_ (`tes3apparatus`_)
    The last used alchemy alembic.

`lastUsedAmmoCount <tes3mobilePlayer/lastUsedAmmoCount.html>`_ (`number`_)
    The amount of ammo for the ranged weapon that that was last equipped.

`lastUsedCalcinator <tes3mobilePlayer/lastUsedCalcinator.html>`_ (`tes3apparatus`_)
    The last used alchemy calcinator.

`lastUsedMortar <tes3mobilePlayer/lastUsedMortar.html>`_ (`tes3apparatus`_)
    The last used alchemy mortar.

`lastUsedRetort <tes3mobilePlayer/lastUsedRetort.html>`_ (`tes3apparatus`_)
    The last used alchemy retort.

`levelupPerSpecialization <tes3mobilePlayer/levelupPerSpecialization.html>`_ (`table`_)
    Array-style table access to how many skill levels there have been for each of the 3 specializations.

`levelUpProgress <tes3mobilePlayer/levelUpProgress.html>`_ (`number`_)
    The progress the player has made towards leveling up.

`levelupsPerAttribute <tes3mobilePlayer/levelupsPerAttribute.html>`_ (`table`_)
    Array-style table access to how many skill levels there have been for each of the 8 attributes.

`levitate <tes3mobileActor/levitate.html>`_ (`number`_)
    Direct access to the actor's levitate effect attribute.

`lightArmor <tes3mobileNPC/lightArmor.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's light armor skill statistic.

`longBlade <tes3mobileNPC/longBlade.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's long blade skill statistic.

`luck <tes3mobileActor/luck.html>`_ (`tes3statistic`_)
    Direct access to the actor's luck attribute statistic.

`magicDisabled <tes3mobilePlayer/magicDisabled.html>`_ (`boolean`_)
    Toggle flag for if the player can use magic.

`magicka <tes3mobileActor/magicka.html>`_ (`tes3statistic`_)
    Access to the actor's magicka statistic.

`magickaMultiplier <tes3mobileActor/magickaMultiplier.html>`_ (`tes3statistic`_)
    Access to the actor's magicka multiplier statistic.

`markLocation <tes3mobilePlayer/markLocation.html>`_ (`tes3markData`_)
    Access to the structure that holds the player's current mark/recall location.

`marksman <tes3mobileNPC/marksman.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's marksman skill statistic.

`mediumArmor <tes3mobileNPC/mediumArmor.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's medium armor skill statistic.

`mercantile <tes3mobileNPC/mercantile.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's mercantile skill statistic.

`mobToMobCollision <tes3mobileActor/mobToMobCollision.html>`_ (`boolean`_)
    Allows modifying if this actor will collide with other actors. When true (default), the actor cannot move through other actors. When false, the actor is allowed to move through other actors, and other actors can move through it.

May be useful when free movement is required in crowded situations, or to temporarily let the player move past an actor.

`mouseLookDisabled <tes3mobilePlayer/mouseLookDisabled.html>`_ (`boolean`_)
    Toggle flag for if the player's mouse look controls are disabled.

`movementCollision <tes3mobileObject/movementCollision.html>`_ (`boolean`_)
    Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.

`movementFlags <tes3mobileObject/movementFlags.html>`_ (`number`_)
    Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.

`moveSpeed <tes3mobileNPC/moveSpeed.html>`_ (`number`_)
    The calculated base movement speed.

`mysticism <tes3mobileNPC/mysticism.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's mysticism skill statistic.

`nextActionWeight <tes3mobileActor/nextActionWeight.html>`_ (`number`_)
    No description available.

`object <tes3mobileNPC/object.html>`_ (`tes3npcInstance`_)
    The actor object that maps to this mobile.

`objectType <tes3mobileObject/objectType.html>`_ (`number`_)
    The type of mobile object. Maps to values in tes3.objectType.

`paralyze <tes3mobileActor/paralyze.html>`_ (`number`_)
    Direct access to the actor's paralyze effect attribute.

`personality <tes3mobileActor/personality.html>`_ (`tes3statistic`_)
    Direct access to the actor's personality attribute statistic.

`playerDistance <tes3mobileObject/playerDistance.html>`_ (`number`_)
    The distance to the player. Updated every frame when the mobile is in an active cell.

`position <tes3mobileObject/position.html>`_ (`tes3vector3`_)
    A vector that represents the 3D position of the object.

`prevMovementFlags <tes3mobileObject/prevMovementFlags.html>`_ (`number`_)
    Access to the root mobile object movement flags from the previous frame, represented as an integer. Should not be accessed directly.

`readiedAmmo <tes3mobileActor/readiedAmmo.html>`_ (`tes3equipmentStack`_)
    The currently equipped ammo.

`readiedAmmoCount <tes3mobileActor/readiedAmmoCount.html>`_ (`number`_)
    The number of ammo equipped for the readied ammo.

`readiedShield <tes3mobileActor/readiedShield.html>`_ (`tes3equipmentStack`_)
    The currently equipped shield.

`readiedWeapon <tes3mobileActor/readiedWeapon.html>`_ (`tes3equipmentStack`_)
    The currently equipped weapon.

`reference <tes3mobileObject/reference.html>`_ (`tes3reference`_)
    Access to the reference object for the mobile, if any.

`resistBlightDisease <tes3mobileActor/resistBlightDisease.html>`_ (`number`_)
    Direct access to the actor's blight disease resistance effect attribute.

`resistCommonDisease <tes3mobileActor/resistCommonDisease.html>`_ (`number`_)
    Direct access to the actor's common disease resistance effect attribute.

`resistCorprus <tes3mobileActor/resistCorprus.html>`_ (`number`_)
    Direct access to the actor's corprus disease resistance effect attribute.

`resistFire <tes3mobileActor/resistFire.html>`_ (`number`_)
    Direct access to the actor's fire resistance effect attribute.

`resistFrost <tes3mobileActor/resistFrost.html>`_ (`number`_)
    Direct access to the actor's frost resistance effect attribute.

`resistMagicka <tes3mobileActor/resistMagicka.html>`_ (`number`_)
    Direct access to the actor's magicka resistance effect attribute.

`resistNormalWeapons <tes3mobileActor/resistNormalWeapons.html>`_ (`number`_)
    Direct access to the actor's normal weapon resistance effect attribute.

`resistParalysis <tes3mobileActor/resistParalysis.html>`_ (`number`_)
    Direct access to the actor's paralysis resistance effect attribute.

`resistPoison <tes3mobileActor/resistPoison.html>`_ (`number`_)
    Direct access to the actor's poison resistance effect attribute.

`resistShock <tes3mobileActor/resistShock.html>`_ (`number`_)
    Direct access to the actor's shock resistance effect attribute.

`restHoursRemaining <tes3mobilePlayer/restHoursRemaining.html>`_ (`number`_)
    How many hours are left while resting.

`restoration <tes3mobileNPC/restoration.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's restoration skill statistic.

`runSpeed <tes3mobileNPC/runSpeed.html>`_ (`number`_)
    The calculated run movement speed.

`sanctuary <tes3mobileActor/sanctuary.html>`_ (`number`_)
    Direct access to the actor's sanctuary effect attribute.

`scanInterval <tes3mobileActor/scanInterval.html>`_ (`number`_)
    No description available.

`scanTimer <tes3mobileActor/scanTimer.html>`_ (`number`_)
    No description available.

`security <tes3mobileNPC/security.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's security skill statistic.

`shield <tes3mobileActor/shield.html>`_ (`number`_)
    Direct access to the actor's shield effect attribute.

`shortBlade <tes3mobileNPC/shortBlade.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's short blade skill statistic.

`silence <tes3mobileActor/silence.html>`_ (`number`_)
    Direct access to the actor's silence effect attribute.

`skillProgress <tes3mobilePlayer/skillProgress.html>`_ (`table`_)
    Array-style table access to skill progress for each of the 27 skills.

`skills <tes3mobileNPC/skills.html>`_ (`table`_)
    An array-style table with access to the twenty seven NPC skill statistics.

`sleeping <tes3mobilePlayer/sleeping.html>`_ (`boolean`_)
    Toggle flag for if the player is currently sleeping.

`sneak <tes3mobileNPC/sneak.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's sneak skill statistic.

`sound <tes3mobileActor/sound.html>`_ (`number`_)
    Direct access to the actor's sound effect attribute.

`spear <tes3mobileNPC/spear.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's spear skill statistic.

`speechcraft <tes3mobileNPC/speechcraft.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's speechcraft skill statistic.

`speed <tes3mobileActor/speed.html>`_ (`tes3statistic`_)
    Direct access to the actor's speed attribute statistic.

`spellReadied <tes3mobileActor/spellReadied.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if the actor has a spell readied.

`strength <tes3mobileActor/strength.html>`_ (`tes3statistic`_)
    Direct access to the actor's strength attribute statistic.

`swiftSwim <tes3mobileActor/swiftSwim.html>`_ (`number`_)
    Direct access to the actor's swift swim effect attribute.

`swimRunSpeed <tes3mobileNPC/swimRunSpeed.html>`_ (`number`_)
    The calculated swim movement speed while running.

`swimSpeed <tes3mobileNPC/swimSpeed.html>`_ (`number`_)
    The calculated swim movement speed.

`talkedTo <tes3mobileActor/talkedTo.html>`_ (`boolean`_)
    Direct access to the actor's flag that shows it was recently talked to.

`telekinesis <tes3mobilePlayer/telekinesis.html>`_ (`number`_)
    Direct access to the player's telekinesis effect attribute.

`torchSlot <tes3mobileActor/torchSlot.html>`_ (`tes3equipmentStack`_)
    The currently equipped light.

`travelling <tes3mobilePlayer/travelling.html>`_ (`boolean`_)
    Toggle flag for if the player is currently travelling.

`unarmored <tes3mobileNPC/unarmored.html>`_ (`tes3skillStatistic`_)
    Direct access to the NPC's unarmored skill statistic.

`underwater <tes3mobileActor/underwater.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if the actor is under water.

`vanityDisabled <tes3mobilePlayer/vanityDisabled.html>`_ (`boolean`_)
    Toggle flag for if the player's vanity camera is disabled.

`velocity <tes3mobileObject/velocity.html>`_ (`tes3vector3`_)
    A vector that represents the 3D velocity of the object.

`viewSwitchDisabled <tes3mobilePlayer/viewSwitchDisabled.html>`_ (`boolean`_)
    Toggle flag for if the player can switch between first person and vanity cameras.

`visionBonus <tes3mobilePlayer/visionBonus.html>`_ (`number`_)
    Direct access to the player's vision bonus effect attribute.

`waiting <tes3mobilePlayer/waiting.html>`_ (`boolean`_)
    Toggle flag for if the player is currently waiting.

`walkSpeed <tes3mobileNPC/walkSpeed.html>`_ (`number`_)
    The calculated walk movement speed.

`waterBreathing <tes3mobileActor/waterBreathing.html>`_ (`number`_)
    Direct access to the actor's water breathing effect attribute.

`waterWalking <tes3mobileActor/waterWalking.html>`_ (`number`_)
    Direct access to the actor's water walking effect attribute.

`weaponDrawn <tes3mobileActor/weaponDrawn.html>`_ (`boolean`_)
    Friendly access to the actor's flag that shows if the weapon model is visible. When readying a weapon, there is a short period of time at the start of the animation, where the weapon is not visible yet. This flag will only be set after this initial stage is done. This flag is still set with hand-to-hand even though it doesn't use a model. Setting this to false while a weapon is drawn will normally cause the actor to play its weapon draw animation again.

`weaponReady <tes3mobilePlayer/weaponReady.html>`_ (`boolean`_)
    Toggle flag for if the player has a weapon ready. Setting it to true will cause the player to take out their weapon. Setting it to false will cause them to put it away.

`werewolf <tes3mobileActor/werewolf.html>`_ (`boolean`_)
    Friendly access to the actor's flag that controls if the actor in werewolf form.

`width <tes3mobileActor/width.html>`_ (`number`_)
    No description available.

`willpower <tes3mobileActor/willpower.html>`_ (`tes3statistic`_)
    Direct access to the actor's willpower attribute statistic.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3mobilePlayer/alwaysRun
    tes3mobilePlayer/attackDisabled
    tes3mobilePlayer/autoRun
    tes3mobilePlayer/birthsign
    tes3mobilePlayer/bounty
    tes3mobilePlayer/cameraHeight
    tes3mobilePlayer/castReady
    tes3mobilePlayer/clawMultiplier
    tes3mobilePlayer/controlsDisabled
    tes3mobilePlayer/dialogueList
    tes3mobilePlayer/firstPerson
    tes3mobilePlayer/firstPersonReference
    tes3mobilePlayer/inactivityTime
    tes3mobilePlayer/inJail
    tes3mobilePlayer/is3rdPerson
    tes3mobilePlayer/jumpingDisabled
    tes3mobilePlayer/knownWerewolf
    tes3mobilePlayer/lastUsedAlembic
    tes3mobilePlayer/lastUsedAmmoCount
    tes3mobilePlayer/lastUsedCalcinator
    tes3mobilePlayer/lastUsedMortar
    tes3mobilePlayer/lastUsedRetort
    tes3mobilePlayer/levelupPerSpecialization
    tes3mobilePlayer/levelUpProgress
    tes3mobilePlayer/levelupsPerAttribute
    tes3mobilePlayer/magicDisabled
    tes3mobilePlayer/markLocation
    tes3mobilePlayer/mouseLookDisabled
    tes3mobilePlayer/restHoursRemaining
    tes3mobilePlayer/skillProgress
    tes3mobilePlayer/sleeping
    tes3mobilePlayer/telekinesis
    tes3mobilePlayer/travelling
    tes3mobilePlayer/vanityDisabled
    tes3mobilePlayer/viewSwitchDisabled
    tes3mobilePlayer/visionBonus
    tes3mobilePlayer/waiting
    tes3mobilePlayer/weaponReady

Methods
----------------------------------------------------------------------------------------------------

`applyDamage <tes3mobileActor/applyDamage.html>`_ (`method`_)
    Damages the actor, with options to control mitigation and difficulty scaling. Invokes the 'damage' and 'damaged' events, with 'script' source. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.

`applyFatigueDamage <tes3mobileActor/applyFatigueDamage.html>`_ (`method`_)
    Damages the actor's fatigue, with accompanying reaction from the reciever. Invokes the 'damageHandToHand' and 'damagedHandToHand' events, with 'script' source. Returns the actual fatigue damage done.

`applyHealthDamage <tes3mobileActor/applyHealthDamage.html>`_ (`method`_)
    **Deprecated, please use applyDamage instead.**

Damages the actor.

`calcEffectiveDamage <tes3mobileActor/calcEffectiveDamage.html>`_ (`method`_)
    Calculates the damage that would be inflicted to an actor after armor and/or resistance. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.

`equip <tes3mobileNPC/equip.html>`_ (`method`_)
    Equips an item, optionally adding the item if needed. If the best match is already equipped, it does not perform an unequip-equip cycle, but does return true.

`exerciseSkill <tes3mobilePlayer/exerciseSkill.html>`_ (`method`_)
    Exercises a skill, providing experience in it.

`getActiveMagicEffects <tes3mobileActor/getActiveMagicEffects.html>`_ (`method`_)
    Fetches a filtered list of the active magic effects on the actor. Returns a table with tes3activeMagicEffect items.

`getBootsWeight <tes3mobileActor/getBootsWeight.html>`_ (`method`_)
    Gets the weight of the boots equipped on the actor, or 0 if no boots are equipped.

`getFatigueTerm <tes3mobileActor/getFatigueTerm.html>`_ (`method`_)
    Gets the fatigue-based skill scaling term used by many game mechanics, based on the actor's current and maximum fatigue. It is equal to ``max(0, fFatigueBase - fFatigueMult * max(0, 1 - fatigue.current/fatigue.base))``

`getPowerUseTimestamp <tes3mobileActor/getPowerUseTimestamp.html>`_ (`method`_)
    Finds the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp().

`getSkillStatistic <tes3mobileActor/getSkillStatistic.html>`_ (`method`_)
    Fetches the statistic object of a skill with a given index. This is the way to access skills for any type of actor, as creatures have a limited version of the skill system. Note that creatures share a statistic between multiple skills (they only have combat, magic, and stealth stats), so many values will be the same.

`getSkillValue <tes3mobileActor/getSkillValue.html>`_ (`method`_)
    Fetches the current value of a skill with a given index. This is the way to access skills for any type of actor, as creatures have a limited version of the skill system. Note that creatures share a statistic between multiple skills (they only have combat, magic, and stealth stats), so many values will be the same.

`getViewToActor <tes3mobileActor/getViewToActor.html>`_ (`method`_)
    No description available.

`getViewToPoint <tes3mobileActor/getViewToPoint.html>`_ (`method`_)
    No description available.

`getViewToPointWithFacing <tes3mobileActor/getViewToPointWithFacing.html>`_ (`method`_)
    No description available.

`getWeaponSpeed <tes3mobileActor/getWeaponSpeed.html>`_ (`method`_)
    Fetches the weapon speed of the actor's currently equipped weapon, or 1.0 if no weapon is equipped.

`hasFreeAction <tes3mobileActor/hasFreeAction.html>`_ (`method`_)
    If true, the actor isn't paralyzed, dead, stunned, or otherwise unable to take action.

`hasUsedPower <tes3mobileActor/hasUsedPower.html>`_ (`method`_)
    Check if a power has been used and is recharging.

`isAffectedByObject <tes3mobileActor/isAffectedByObject.html>`_ (`method`_)
    Determines if the actor is currently being affected by a given alchemy, enchantment, or spell.

`levelSkill <tes3mobilePlayer/levelSkill.html>`_ (`method`_)
    Checks to see if a skill is ready to be leveled up, and performs any levelup logic.

`rechargePower <tes3mobileActor/rechargePower.html>`_ (`method`_)
    Makes a power immediately available for casting again.

`setPowerUseTimestamp <tes3mobileActor/setPowerUseTimestamp.html>`_ (`method`_)
    Sets the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp.

`startCombat <tes3mobileActor/startCombat.html>`_ (`method`_)
    Forces the actor into combat with another actor.

`startDialogue <tes3mobileActor/startDialogue.html>`_ (`method`_)
    Starts dialogue with this actor for the player.

`stopCombat <tes3mobileActor/stopCombat.html>`_ (`method`_)
    Ends combat for the actor.

`unequip <tes3mobileNPC/unequip.html>`_ (`method`_)
    Unequips one or more items from the actor.

`updateDerivedStatistics <tes3mobileActor/updateDerivedStatistics.html>`_ (`method`_)
    Updates statistics derived from attributes, which are magicka, fatigue, and encumbrance. Will also update the UI if used on the player. Normally handled automatically when you use tes3.modStatistic.

`updateOpacity <tes3mobileActor/updateOpacity.html>`_ (`method`_)
    Updates the actor's visual opacity. Used after modifying applied chameleon or invisiblity effects.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3mobilePlayer/exerciseSkill
    tes3mobilePlayer/levelSkill

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3actionData`: ../../lua/type/tes3actionData.html
.. _`tes3actorAnimationController`: ../../lua/type/tes3actorAnimationController.html
.. _`tes3aiPlanner`: ../../lua/type/tes3aiPlanner.html
.. _`tes3apparatus`: ../../lua/type/tes3apparatus.html
.. _`tes3birthsign`: ../../lua/type/tes3birthsign.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3combatSession`: ../../lua/type/tes3combatSession.html
.. _`tes3equipmentStack`: ../../lua/type/tes3equipmentStack.html
.. _`tes3globalVariable`: ../../lua/type/tes3globalVariable.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3markData`: ../../lua/type/tes3markData.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3mobileNPC`: ../../lua/type/tes3mobileNPC.html
.. _`tes3mobileObject`: ../../lua/type/tes3mobileObject.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
.. _`tes3npcInstance`: ../../lua/type/tes3npcInstance.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3skillStatistic`: ../../lua/type/tes3skillStatistic.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
.. _`tes3statistic`: ../../lua/type/tes3statistic.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
