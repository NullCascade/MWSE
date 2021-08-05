tes3
====================================================================================================

The tes3 library provides the majority of the functions for interacting with the game system.

Values
----------------------------------------------------------------------------------------------------

`animationState <tes3/animationState.html>`_ (`any`_)
    Constant values relating to animation state.

`dataHandler <tes3/dataHandler.html>`_ (`tes3dataHandler`_)
    One of the core game objects.

`game <tes3/game.html>`_ (`tes3game`_)
    One of the core game objects.

`installDirectory <tes3/installDirectory.html>`_ (`string`_)
    The currently executed root Morrowind installation path.

`magicSchoolSkill <tes3/magicSchoolSkill.html>`_ (`tes3.skill.* constants`_)
    These constants are used to convert magic schools to their respective skill. These constants map to their respective tes3.skill constants.

`mobilePlayer <tes3/mobilePlayer.html>`_ (`tes3mobilePlayer`_)
    The player's mobile actor.

`player <tes3/player.html>`_ (`tes3reference`_)
    A reference to the player.

`worldController <tes3/worldController.html>`_ (`tes3worldController`_)
    One of the core game objects.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3/animationState
    tes3/dataHandler
    tes3/game
    tes3/installDirectory
    tes3/magicSchoolSkill
    tes3/mobilePlayer
    tes3/player
    tes3/worldController

Functions
----------------------------------------------------------------------------------------------------

`addArmorSlot <tes3/addArmorSlot.html>`_ (`function`_)
    Registers a new armor slot. Adds a new place for armor pieces with a matching slot number to equip to.

`addItem <tes3/addItem.html>`_ (`function`_)
    Adds an item to a given reference's inventory or mobile's inventory.

`addItemData <tes3/addItemData.html>`_ (`function`_)
    Creates an item data if there is room for a new stack in a given inventory. This can be then used to add custom user data or adjust an item's condition. This will return nil if no item data could be allocated for the item -- for example if the reference doesn't have the item in their inventory or each item of that type already has item data.

`addJournalEntry <tes3/addJournalEntry.html>`_ (`function`_)
    This function creates a new journal entry. It can be called once the world controller is loaded.

`addMagicEffect <tes3/addMagicEffect.html>`_ (`function`_)
    This function creates a new custom magic effect. The effect can be scripted through lua.

`addSoulGem <tes3/addSoulGem.html>`_ (`function`_)
    Causes a misc item to be recognized as a soul gem, so that it can be used for soul trapping.

`adjustSoundVolume <tes3/adjustSoundVolume.html>`_ (`function`_)
    Changes the volume of a sound that is playing on a given reference.

`advanceTime <tes3/advanceTime.html>`_ (`function`_)
    Advances the game time. Can be used to simulate player resting.

`applyMagicSource <tes3/applyMagicSource.html>`_ (`function`_)
    

`beginTransform <tes3/beginTransform.html>`_ (`function`_)
    Changes a reference to werewolf form. This function works only on a reference infected with Lycanthropy, be it the player or any other reference. Returns true if successful.

`cancelAnimationLoop <tes3/cancelAnimationLoop.html>`_ (`function`_)
    Signals looping animations on the actor to stop looping and play to the end. The animation will continue, playing past the loop point until the end frame. Useful for exiting looping animations cleanly.

`canRest <tes3/canRest.html>`_ (`function`_)
    This function returns true if player can rest.

`cast <tes3/cast.html>`_ (`function`_)
    Casts a spell from a given reference to a target reference. Touch effects will hit the target at any range, while target effects will create a projectile. By default, the spell always casts successfully and does not consume magicka. By default, an actor casting will stop and perform its cast animation, but the 'instant' flag can start the cast instantly, and allow more control over the spell.

When the caster is the player, the target parameter is optional; without a target, the player's touch effects will only hit targets in front of them, and target effects will create a projectile in the direction the player is facing. Currently as a limitation, instant must be true to allow the player to cast spells.

`checkMerchantOffersService <tes3/checkMerchantOffersService.html>`_ (`function`_)
    Checks if a merchant will offer a service to you, including dialogue checks like disposition and faction membership. A specific service can be checked, or if no service is given, a generic dialogue check is made. If the service if refused, the dialogue reply for the refusal may also returned (it may be nil, as there may not always be a reply available).

`checkMerchantTradesItem <tes3/checkMerchantTradesItem.html>`_ (`function`_)
    Determines if a merchant trades in a given item.

`claimSpellEffectId <tes3/claimSpellEffectId.html>`_ (`function`_)
    This function is used to claim a unique spell effect name and id. This is needed before actually creating a new effect by calling tes3.addMagicEffect. A claimed effect id can be retrieved as: tes3.effect.effectName (just like any regular spell effect).

`clearMarkLocation <tes3/clearMarkLocation.html>`_ (`function`_)
    This function deletes the player's mark location.

`createObject <tes3/createObject.html>`_ (`function`_)
    Create an object and returns it. The created object will be part of the saved game. Supported object types are those that have their own create function, such as tes3activator for example.

`createReference <tes3/createReference.html>`_ (`function`_)
    Similar to mwscript's PlaceAtPC or PlaceAtMe, this creates a new reference in the game world.

`decrementKillCount <tes3/decrementKillCount.html>`_ (`function`_)
    Decreases player's kill count of a certain type of actor by one.

`deleteObject <tes3/deleteObject.html>`_ (`function`_)
    Deletes a game object from the system. This can be dangerous, use with caution.

`disableKey <tes3/disableKey.html>`_ (`function`_)
    Disables the use of a keyboard key.

`dropItem <tes3/dropItem.html>`_ (`function`_)
    Drops one or more items from a reference's inventory onto the ground at their feet. It will unequip the item if it is equipped. The return value will be nil if no matching item was found.

`enableKey <tes3/enableKey.html>`_ (`function`_)
    Enables the use of a keyboard key.

`fadeIn <tes3/fadeIn.html>`_ (`function`_)
    Similar to the vanilla FadeIn mwscript command.

`fadeOut <tes3/fadeOut.html>`_ (`function`_)
    Similar to the vanilla FadeOut mwscript command.

`fadeTo <tes3/fadeTo.html>`_ (`function`_)
    Similar to the vanilla FadeTo mwscript command.

`findActorsInProximity <tes3/findActorsInProximity.html>`_ (`function`_)
    Searches for active mobile actors which are within ``range`` distance from the ``reference`` or ``position`` argument. It only finds mobiles which have active AI, and can include the player. This function is used by the game for area-of-effect hits. It has a small amount of overhead, so try not to use it too much.

`findBirthsign <tes3/findBirthsign.html>`_ (`function`_)
    Fetches the core game birthsign object for a given birthsign ID. If the birthsign with a given ID doesn't exist, nil is returned.

`findClass <tes3/findClass.html>`_ (`function`_)
    Fetches the core game character class object for a given class ID. If the class with a given ID doesn't exist, nil is returned.

`findClosestExteriorReferenceOfObject <tes3/findClosestExteriorReferenceOfObject.html>`_ (`function`_)
    Using the same engine function used by the vanilla intervention spells, locations the first reference of a given object in the exterior world.

`findDialogue <tes3/findDialogue.html>`_ (`function`_)
    Locates a root dialogue topic that can then be filtered down for a specific actor to return a specific dialogue info. For example, a type of ``tes3.dialogueType.greeting`` and a page of ``tes3.dialoguePage.greeting.greeting0`` will return the "Greeting 0" topic.

`findGlobal <tes3/findGlobal.html>`_ (`function`_)
    Fetches the core game object that represents a global variable.

`findGMST <tes3/findGMST.html>`_ (`function`_)
    Fetches the core game object that represents a game setting. While this function accepts a name, it is recommended to use the tes3.GMST constants.

`findRegion <tes3/findRegion.html>`_ (`function`_)
    Fetches the core game region object for a given region ID. If the region with a given ID doesn't exist, nil is returned.

`force1stPerson <tes3/force1stPerson.html>`_ (`function`_)
    Forces the player's POV to first person the next simulation frame, and returns true if the POV has been changed.

`force3rdPerson <tes3/force3rdPerson.html>`_ (`function`_)
    Forces the player's POV to third person the next simulation frame, and returns true if the POV has been changed.

`get3rdPersonCameraOffset <tes3/get3rdPersonCameraOffset.html>`_ (`function`_)
    Returns the camera's offset from the player's head while in 3rd person view.

Note this function can be used once WorldController and MobilePlayer have finished initializing.

`getActiveCells <tes3/getActiveCells.html>`_ (`function`_)
    Returns a table of active cells. If indoors, the table will have only one entry. If outdoors, the 9 surrounding cells will be provided.

`getAnimationGroups <tes3/getAnimationGroups.html>`_ (`function`_)
    This function fetches a reference's attached animation groups.

`getAnimationTiming <tes3/getAnimationTiming.html>`_ (`function`_)
    This function fetches a reference's attached animation groups' timings.

`getArchiveList <tes3/getArchiveList.html>`_ (`function`_)
    Returns a 1-indexed table of active archives.

`getAttachment <tes3/getAttachment.html>`_ (`function`_)
    Fetches an attachment with a given type.

`getAttributeName <tes3/getAttributeName.html>`_ (`function`_)
    Returns the lowercase identifying name of an attribute for a given numerical, 0-based index. E.g. "strength".

`getCameraPosition <tes3/getCameraPosition.html>`_ (`function`_)
    Returns the camera's position.

`getCameraVector <tes3/getCameraVector.html>`_ (`function`_)
    Returns the camera look vector.

`getCell <tes3/getCell.html>`_ (`function`_)
    Finds a cell, either by an id or an X/Y grid position.

`getCumulativeDaysForMonth <tes3/getCumulativeDaysForMonth.html>`_ (`function`_)
    Gets the number of days that have passed leading up to the start of a given month.

`getCurrentAIPackageId <tes3/getCurrentAIPackageId.html>`_ (`function`_)
    Returns an actor's current AI package ID, just as the mwscript function `GetCurrentAIPackage` would.

`getCurrentWeather <tes3/getCurrentWeather.html>`_ (`function`_)
    Gets the currently active weather, from the player's current region.

`getCursorPosition <tes3/getCursorPosition.html>`_ (`function`_)
    Returns a table with values x and y that contain the current cursor position.

`getDaysInMonth <tes3/getDaysInMonth.html>`_ (`function`_)
    Returns the number of days in a given month. This may be altered if a Morrowind Code Patch feature was installed.

`getDialogueInfo <tes3/getDialogueInfo.html>`_ (`function`_)
    Locates and returns a Dialogue Info by a given id. This involves file IO and is an expensive call. Results should be cached.

`getEffectMagnitude <tes3/getEffectMagnitude.html>`_ (`function`_)
    This function returns the total magnitude and total unresisted magnitude of a certain spell effect affecting a reference. It returns a pair of numbers, the first being the post-resistance magnitude (see examples). The unresisted magnitude is the magnitude before the actor's resistances are applied; it is always an integer, so it is used in some UI elements.

`getEquippedItem <tes3/getEquippedItem.html>`_ (`function`_)
    Returns an actor's equipped item stack, provided a given filter

`getFaction <tes3/getFaction.html>`_ (`function`_)
    Fetches the core game faction object for a given faction ID.

`getFileExists <tes3/getFileExists.html>`_ (`function`_)
    Determines if a file exists in the user's Data Files.

`getFileSource <tes3/getFileSource.html>`_ (`function`_)
    Determines if a file exists on the filesystem or inside of a bsa. The returned string will be "file" or "bsa".

`getGlobal <tes3/getGlobal.html>`_ (`function`_)
    Retrieves the value of a global value, or nil if the global could not be found.

`getInputBinding <tes3/getInputBinding.html>`_ (`function`_)
    Gets the input configuration for a given keybind.

`getItemIsStolen <tes3/getItemIsStolen.html>`_ (`function`_)
    This function checks item's stolen flag.

`getJournalIndex <tes3/getJournalIndex.html>`_ (`function`_)
    Gets the index of a given journal, or nil if no valid journal could be found.

`getKillCount <tes3/getKillCount.html>`_ (`function`_)
    Returns how many times the player killed an actor. If no actor is specified, total number of kills player commited will be returned.

`getKillCounts <tes3/getKillCounts.html>`_ (`function`_)
    Returns a table with complete player kill counts. The table returned is formated so actor IDs are table keys and kill count of that type of actor is value.

`getLanguage <tes3/getLanguage.html>`_ (`function`_)
    Gets the language as an ISO string (e.g. "eng"), determined by the language entry in Morrowind.ini.

`getLanguageCode <tes3/getLanguageCode.html>`_ (`function`_)
    Gets the language code, determined by the language entry in Morrowind.ini.

`getLastExteriorPosition <tes3/getLastExteriorPosition.html>`_ (`function`_)
    Returns the last exterior position of the player.

`getLocked <tes3/getLocked.html>`_ (`function`_)
    Determines if a given reference is a locked door or container.

`getLockLevel <tes3/getLockLevel.html>`_ (`function`_)
    Gets an locked reference's lock level. If no lock data is available, it will return nil.

`getMagicEffect <tes3/getMagicEffect.html>`_ (`function`_)
    Fetches the core game Magic Effect object for a given ID.

`getMagicSourceInstanceBySerial <tes3/getMagicSourceInstanceBySerial.html>`_ (`function`_)
    Fetches an instance of the magic source of a given serial number.

`getModList <tes3/getModList.html>`_ (`function`_)
    Fetches the list of the active ESM and ESP files.

`getObject <tes3/getObject.html>`_ (`function`_)
    Fetches the core game object for a given object ID.

`getOwner <tes3/getOwner.html>`_ (`function`_)
    Returns the object's owner, or nil if the object is unowned.

`getPlayerCell <tes3/getPlayerCell.html>`_ (`function`_)
    Fetches the cell that the player is currently in.

`getPlayerEyePosition <tes3/getPlayerEyePosition.html>`_ (`function`_)
    Returns the position of the player's eyes.

`getPlayerEyeVector <tes3/getPlayerEyeVector.html>`_ (`function`_)
    Returns the look direction of the player's eyes.

`getPlayerGold <tes3/getPlayerGold.html>`_ (`function`_)
    Gets the gold count carried by the player.

`getPlayerTarget <tes3/getPlayerTarget.html>`_ (`function`_)
    This function is used to see what the player is looking at. Unlike a real raycast, this does not work in all circumstances. As a general rule, it will return the reference if the information box is shown when it is looked at.

`getReference <tes3/getReference.html>`_ (`function`_)
    Fetches the first reference for a given base object ID.

`getRegion <tes3/getRegion.html>`_ (`function`_)
    Gets the current region the player is in. This checks the player's current cell first, but will fall back to the last exterior cell.

`getScript <tes3/getScript.html>`_ (`function`_)
    Locates and returns a script by a given id.

`getSimulationTimestamp <tes3/getSimulationTimestamp.html>`_ (`function`_)
    Returns a UNIX-style timestamp based on in-world simulation time since the start of the era.

`getSkill <tes3/getSkill.html>`_ (`function`_)
    Fetches the core game object for a given skill ID.

`getSkillName <tes3/getSkillName.html>`_ (`function`_)
    Returns the identifying name of a skill for a given numerical, 0-based index. E.g. "Enchant".

`getSound <tes3/getSound.html>`_ (`function`_)
    Locates and returns a sound by a given id.

`getSoundGenerator <tes3/getSoundGenerator.html>`_ (`function`_)
    Returns a sound generator by a given creature id and type.

`getSoundPlaying <tes3/getSoundPlaying.html>`_ (`function`_)
    Without a reference, this function returns true if the sound is playing unattached or on any reference. With a reference, it returns true if the sound is playing on that specific reference.

`getSpecializationName <tes3/getSpecializationName.html>`_ (`function`_)
    Returns the lowercase identifying name of a specialization type for a given numerical, 0-based index. E.g. "magic".

`getTopMenu <tes3/getTopMenu.html>`_ (`function`_)
    Gets the top-level UI menu.

`getTrap <tes3/getTrap.html>`_ (`function`_)
    Gets the trap on a given reference.

`getViewportSize <tes3/getViewportSize.html>`_ (`function`_)
    Returns both the viewport width and the viewport height. Note that this the real resolution of the screen. For a value scaled by MGE's menu scaling, see the same-named function in the tes3ui namespace. To get the scale used, check getViewportScale in the tes3ui namespace.

`getWerewolfKillCount <tes3/getWerewolfKillCount.html>`_ (`function`_)
    Returns the number of kills player commited as a Werewolf.

`hammerKey <tes3/hammerKey.html>`_ (`function`_)
    Simulates hammering a key.

`hasCodePatchFeature <tes3/hasCodePatchFeature.html>`_ (`function`_)
    Attempts to determine if a given Morrowind Code Patch feature is enabled. This may not be possible, in which case nil will be returned.

`hasOwnershipAccess <tes3/hasOwnershipAccess.html>`_ (`function`_)
    Determines if a reference has access to another object, including its inventory. References have access to their own things, and the player has access to dead NPC's items.

`incrementKillCount <tes3/incrementKillCount.html>`_ (`function`_)
    Increases player's kill count of a certain type of actor by one.

`is3rdPerson <tes3/is3rdPerson.html>`_ (`function`_)
    Returns if the game is in 3rd person.

`isAffectedBy <tes3/isAffectedBy.html>`_ (`function`_)
    This function check whether a reference is affected by a certain object or effect.

Note reference.object.spells:contains(spellID) will give the same output as this function for abilities, diseases, and curses, because having them in your spell list also makes them affect you.

`isModActive <tes3/isModActive.html>`_ (`function`_)
    Determines if the player has a given ESP or ESM file active.

`iterate <tes3/iterate.html>`_ (`function`_)
    This function returns a function that iterates over a tes3iterator object. This is useful for for loops.

`iterateObjects <tes3/iterateObjects.html>`_ (`function`_)
    Iteration function used for looping over game options.

`loadAnimation <tes3/loadAnimation.html>`_ (`function`_)
    Loads an animation and uses it to override existing animations on an actor. Animation groups present in the file will replace the actor's animation groups. The actor's model must be already loaded into memory to have its animations modified. The overridden animations only persist until the actor is unloaded.

Calling this more than once will remove the previous animation override before loading the new animation. Therefore, if applying animations to the player, you should call this before every playAnimation so that different animation mods can co-exist. For NPCs under your mod's control, you only need to do it when the player enters the cell containing the NPC.

Calling this without a file argument will reset the reference's animations to default.

`loadGame <tes3/loadGame.html>`_ (`function`_)
    Loads a game.

`loadMesh <tes3/loadMesh.html>`_ (`function`_)
    Loads a mesh file and provides a scene graph object.

`loadSourceTexture <tes3/loadSourceTexture.html>`_ (`function`_)
    Loads a source texture file and provides the niSourceTexture object.

`lock <tes3/lock.html>`_ (`function`_)
    Locks an object, and optionally sets a locked reference's lock level. Returns true if the object can be and wasn't already locked.

`loopTArray <tes3/loopTArray.html>`_ (`function`_)
    This function returns a function that iterates over a tes3tarray object. This is useful for for loops.

`makeSafeObjectHandle <tes3/makeSafeObjectHandle.html>`_ (`function`_)
    Returns a safe handle for the object. To use the object use :getObject(). To check if it still exists use :valid().

`menuMode <tes3/menuMode.html>`_ (`function`_)
    Returns true if the player is currently in menu mode.

`messageBox <tes3/messageBox.html>`_ (`function`_)
    Displays a message box. This may be a simple toast-style message, or a box with choice buttons.

`modStatistic <tes3/modStatistic.html>`_ (`function`_)
    Modifies a statistic on a given actor. This should be used instead of manually setting values on the game structures, to ensure that events and GUI elements are properly handled. Either skill, attribute, or name must be provided.

`newGame <tes3/newGame.html>`_ (`function`_)
    Starts a new game.

`onMainMenu <tes3/onMainMenu.html>`_ (`function`_)
    Returns true if the player is on the main menu, and hasn't loaded a game yet.

`persuade <tes3/persuade.html>`_ (`function`_)
    Attempts a persuasion attempt on an actor, potentially adjusting their disposition. Returns true if the attempt was a success.

`playAnimation <tes3/playAnimation.html>`_ (`function`_)
    Plays a given animation group. Optional flags can be used to define how the group starts.

When 'group' is specified, the actor AI is paused while playing the animation, as if in an idle state. When one or more of 'lower', 'upper', or 'shield' are specified without 'group', the actor AI retains control, and the animations are layered. e.g. tes3.playAnimation{reference = ..., upper = tes3.animationGroup.idle3, loopCount = 0} will play the idle3 animation once (without looping) on the upper body, while the player or NPC is still able to walk around. Using 'upper' defers combat actions until the animation is done. Using 'shield' still allows combat actions.

As a special case, tes3.playAnimation{reference = ..., group = 0} returns control to the AI, as the AI knows that is the actor's neutral idle state.

`playItemPickupSound <tes3/playItemPickupSound.html>`_ (`function`_)
    Plays the sound responsible for picking up or putting down an item.

`playSound <tes3/playSound.html>`_ (`function`_)
    Plays a sound on a given reference. Provides control over volume (including volume channel), pitch, and loop control.

`playVoiceover <tes3/playVoiceover.html>`_ (`function`_)
    Causes a target actor to play a voiceover.

`positionCell <tes3/positionCell.html>`_ (`function`_)
    Positions a reference to another place.

`pushKey <tes3/pushKey.html>`_ (`function`_)
    Simulates pushing a keyboard key.

`random <tes3/random.html>`_ (`function`_)
    Returns a value from Morrowind's random number generator. This is not preferrable to lua's math.random function, but may be necessary for reproducing Morrowind's generation.

`rayTest <tes3/rayTest.html>`_ (`function`_)
    Performs a ray test and returns various information related to the result(s). If findAll is set, the result will be a table of results, otherwise only the first result is returned.

`releaseKey <tes3/releaseKey.html>`_ (`function`_)
    Simulates releasing a keyboard key.

`removeEffects <tes3/removeEffects.html>`_ (`function`_)
    Removes magic effects from a given reference. Requires that either the effect or castType parameter be provided.

`removeItem <tes3/removeItem.html>`_ (`function`_)
    Removes an item from a given reference's inventory.

`removeSound <tes3/removeSound.html>`_ (`function`_)
    Stops a sound playing. Without a reference, it will match unattached sounds. With a reference, it will only match a sound playing on that specific reference.

`runLegacyScript <tes3/runLegacyScript.html>`_ (`function`_)
    This function will compile and run a mwscript chunk of code. This is not ideal to use, but can be used for features not yet exposed to lua.

`saveGame <tes3/saveGame.html>`_ (`function`_)
    Saves the game.

`say <tes3/say.html>`_ (`function`_)
    Plays a sound file, with an optional alteration and subtitle.

**Note**: MP3 voice files must conform to the MPEG Layer-3, 64 Kbps 44100 kHz, 16-bit mono specification.

`set3rdPersonCameraOffset <tes3/set3rdPersonCameraOffset.html>`_ (`function`_)
    Changes the 3rd person camera's offset from the player's head.

Note this function can be used once WorldController and MobilePlayer have finished initializing.

`setAIActivate <tes3/setAIActivate.html>`_ (`function`_)
    Configures a mobile actor to activate an object.

`setAIEscort <tes3/setAIEscort.html>`_ (`function`_)
    Configures a mobile actor to escort another actor to a destination.

`setAIFollow <tes3/setAIFollow.html>`_ (`function`_)
    Configures a mobile actor to follow another actor to a destination.

`setAITravel <tes3/setAITravel.html>`_ (`function`_)
    Configures a mobile actor to travel to a destination.

`setAIWander <tes3/setAIWander.html>`_ (`function`_)
    Configures a mobile actor to wander around a cell.

`setAnimationTiming <tes3/setAnimationTiming.html>`_ (`function`_)
    This function sets a reference's animation groups' timings to a specified value.

`setDestination <tes3/setDestination.html>`_ (`function`_)
    Sets or changes the destination of a door to a new location.

`setEnabled <tes3/setEnabled.html>`_ (`function`_)
    Enables or disables a reference.

`setGlobal <tes3/setGlobal.html>`_ (`function`_)
    Sets the value of a global value. If the global could not be found, the function returns false.

`setItemIsStolen <tes3/setItemIsStolen.html>`_ (`function`_)
    This function changes an item's stolen flag. Morrowind handles stealing by marking the base item (not the inventory stack) with with NPCs that you have stolen that item from. The NPC will recognize an item is stolen if they are marked as stolen on the base item.

`setJournalIndex <tes3/setJournalIndex.html>`_ (`function`_)
    Sets the index of a given journal in a way similar to the mwscript function SetJournalIndex.

`setKillCount <tes3/setKillCount.html>`_ (`function`_)
    Sets player's kill count of a certain type of actor.

`setLockLevel <tes3/setLockLevel.html>`_ (`function`_)
    Sets a locked reference's lock level. This does not lock the object.

`setMarkLocation <tes3/setMarkLocation.html>`_ (`function`_)
    This function sets the player's mark location to the one specified.

`setOwner <tes3/setOwner.html>`_ (`function`_)
    This function sets the owner of a reference.

`setPlayerControlState <tes3/setPlayerControlState.html>`_ (`function`_)
    Enables or disables player's controls state.

`setSourceless <tes3/setSourceless.html>`_ (`function`_)
    Sets an object (of any kind) to be sourceless, which are objects the game does not store in savegames. This can be useful for mod-created temporary objects which are not necessary to save.

`setStatistic <tes3/setStatistic.html>`_ (`function`_)
    Sets a statistic on a given actor. This should be used instead of manually setting values on the game structures, to ensure that events and GUI elements are properly handled. Either skill, attribute, or name must be provided.

`setTrap <tes3/setTrap.html>`_ (`function`_)
    Sets the trap on a given reference.

`setVanityMode <tes3/setVanityMode.html>`_ (`function`_)
    Toggles the camera into third person mode. Returns true if changed to vanity mode.

Note that unlike the vanity mode caused by not doing anything for a while, this vanity mode must be toggled to go off.

`setWerewolfKillCount <tes3/setWerewolfKillCount.html>`_ (`function`_)
    Sets player's kill count as a werewolf.

`showAlchemyMenu <tes3/showAlchemyMenu.html>`_ (`function`_)
    This function opens alchemy menu.

`showRepairServiceMenu <tes3/showRepairServiceMenu.html>`_ (`function`_)
    Opens the service repair menu.

`showRestMenu <tes3/showRestMenu.html>`_ (`function`_)
    This function opens the resting menu and returns true on success. If the player can't rest currently, it returns false.

Various parameters can be used to allow resting in situations not normally possible.

`skipAnimationFrame <tes3/skipAnimationFrame.html>`_ (`function`_)
    Skips a given reference's animation for a single frame.

`streamMusic <tes3/streamMusic.html>`_ (`function`_)
    This function interrupts the current music to play the specified music track.

`tapKey <tes3/tapKey.html>`_ (`function`_)
    Simulates tapping a keyboard key.

`testLineOfSight <tes3/testLineOfSight.html>`_ (`function`_)
    Checks if there is a clear line of sight between two references, or two points in space, in the same method that the game uses for actor vision. You should pass two references, or two positions and heights.

Only certain object types count as blocking for the purposes of the test:
Statics, activators, containers, doors, and lights with non-zero weight.

The collision root node is used for testing, if present in the model.

`togglePOV <tes3/togglePOV.html>`_ (`function`_)
    Forces a toggle of the player's POV the next simulation frame, and returns if the player was previously in 3rd person. Multiple calls in the same frame will not stack.

`transferItem <tes3/transferItem.html>`_ (`function`_)
    Moves one or more items from one reference to another. Returns the actual amount of items successfully transferred.

`triggerCrime <tes3/triggerCrime.html>`_ (`function`_)
    Emulates the player committing a crime.

`undoTransform <tes3/undoTransform.html>`_ (`function`_)
    Changes a reference back from werewolf form to human. This function works only on a reference infected with Lycanthropy, be it the player or any other reference. Returns true if successful.

`unhammerKey <tes3/unhammerKey.html>`_ (`function`_)
    Stops simulating hammering a keyboard key.

`unlock <tes3/unlock.html>`_ (`function`_)
    Unlocks an object. Returns true if the object can be and wasn't already unlocked.

`updateInventoryGUI <tes3/updateInventoryGUI.html>`_ (`function`_)
    Forces the GUI to update inventory-relevant elements for a given reference. This can be used after many calls to inventory-manipulating functions while passing updateGUI as false to resync inventory tiles, container weights, and companion data.

`updateJournal <tes3/updateJournal.html>`_ (`function`_)
    Updates the journal index in a way similar to the mwscript function Journal.

`wakeUp <tes3/wakeUp.html>`_ (`function`_)
    This function wakes player up and returns true if successful.

Note this function returns false if WorldController or MobilePlayer aren't initialized, and if the player is not sleeping or waiting



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3/addArmorSlot
    tes3/addItem
    tes3/addItemData
    tes3/addJournalEntry
    tes3/addMagicEffect
    tes3/addSoulGem
    tes3/adjustSoundVolume
    tes3/advanceTime
    tes3/applyMagicSource
    tes3/beginTransform
    tes3/cancelAnimationLoop
    tes3/canRest
    tes3/cast
    tes3/checkMerchantOffersService
    tes3/checkMerchantTradesItem
    tes3/claimSpellEffectId
    tes3/clearMarkLocation
    tes3/createObject
    tes3/createReference
    tes3/decrementKillCount
    tes3/deleteObject
    tes3/disableKey
    tes3/dropItem
    tes3/enableKey
    tes3/fadeIn
    tes3/fadeOut
    tes3/fadeTo
    tes3/findActorsInProximity
    tes3/findBirthsign
    tes3/findClass
    tes3/findClosestExteriorReferenceOfObject
    tes3/findDialogue
    tes3/findGlobal
    tes3/findGMST
    tes3/findRegion
    tes3/force1stPerson
    tes3/force3rdPerson
    tes3/get3rdPersonCameraOffset
    tes3/getActiveCells
    tes3/getAnimationGroups
    tes3/getAnimationTiming
    tes3/getArchiveList
    tes3/getAttachment
    tes3/getAttributeName
    tes3/getCameraPosition
    tes3/getCameraVector
    tes3/getCell
    tes3/getCumulativeDaysForMonth
    tes3/getCurrentAIPackageId
    tes3/getCurrentWeather
    tes3/getCursorPosition
    tes3/getDaysInMonth
    tes3/getDialogueInfo
    tes3/getEffectMagnitude
    tes3/getEquippedItem
    tes3/getFaction
    tes3/getFileExists
    tes3/getFileSource
    tes3/getGlobal
    tes3/getInputBinding
    tes3/getItemIsStolen
    tes3/getJournalIndex
    tes3/getKillCount
    tes3/getKillCounts
    tes3/getLanguage
    tes3/getLanguageCode
    tes3/getLastExteriorPosition
    tes3/getLocked
    tes3/getLockLevel
    tes3/getMagicEffect
    tes3/getMagicSourceInstanceBySerial
    tes3/getModList
    tes3/getObject
    tes3/getOwner
    tes3/getPlayerCell
    tes3/getPlayerEyePosition
    tes3/getPlayerEyeVector
    tes3/getPlayerGold
    tes3/getPlayerTarget
    tes3/getReference
    tes3/getRegion
    tes3/getScript
    tes3/getSimulationTimestamp
    tes3/getSkill
    tes3/getSkillName
    tes3/getSound
    tes3/getSoundGenerator
    tes3/getSoundPlaying
    tes3/getSpecializationName
    tes3/getTopMenu
    tes3/getTrap
    tes3/getViewportSize
    tes3/getWerewolfKillCount
    tes3/hammerKey
    tes3/hasCodePatchFeature
    tes3/hasOwnershipAccess
    tes3/incrementKillCount
    tes3/is3rdPerson
    tes3/isAffectedBy
    tes3/isModActive
    tes3/iterate
    tes3/iterateObjects
    tes3/loadAnimation
    tes3/loadGame
    tes3/loadMesh
    tes3/loadSourceTexture
    tes3/lock
    tes3/loopTArray
    tes3/makeSafeObjectHandle
    tes3/menuMode
    tes3/messageBox
    tes3/modStatistic
    tes3/newGame
    tes3/onMainMenu
    tes3/persuade
    tes3/playAnimation
    tes3/playItemPickupSound
    tes3/playSound
    tes3/playVoiceover
    tes3/positionCell
    tes3/pushKey
    tes3/random
    tes3/rayTest
    tes3/releaseKey
    tes3/removeEffects
    tes3/removeItem
    tes3/removeSound
    tes3/runLegacyScript
    tes3/saveGame
    tes3/say
    tes3/set3rdPersonCameraOffset
    tes3/setAIActivate
    tes3/setAIEscort
    tes3/setAIFollow
    tes3/setAITravel
    tes3/setAIWander
    tes3/setAnimationTiming
    tes3/setDestination
    tes3/setEnabled
    tes3/setGlobal
    tes3/setItemIsStolen
    tes3/setJournalIndex
    tes3/setKillCount
    tes3/setLockLevel
    tes3/setMarkLocation
    tes3/setOwner
    tes3/setPlayerControlState
    tes3/setSourceless
    tes3/setStatistic
    tes3/setTrap
    tes3/setVanityMode
    tes3/setWerewolfKillCount
    tes3/showAlchemyMenu
    tes3/showRepairServiceMenu
    tes3/showRestMenu
    tes3/skipAnimationFrame
    tes3/streamMusic
    tes3/tapKey
    tes3/testLineOfSight
    tes3/togglePOV
    tes3/transferItem
    tes3/triggerCrime
    tes3/undoTransform
    tes3/unhammerKey
    tes3/unlock
    tes3/updateInventoryGUI
    tes3/updateJournal
    tes3/wakeUp

.. _`any`: ../../lua/type/any.html
.. _`function`: ../../lua/type/function.html
.. _`string`: ../../lua/type/string.html
.. _`tes3.skill.* constants`: ../../lua/type/tes3.skill.* constants.html
.. _`tes3dataHandler`: ../../lua/type/tes3dataHandler.html
.. _`tes3game`: ../../lua/type/tes3game.html
.. _`tes3mobilePlayer`: ../../lua/type/tes3mobilePlayer.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3worldController`: ../../lua/type/tes3worldController.html
