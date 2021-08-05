tes3.playAnimation
====================================================================================================

Plays a given animation group. Optional flags can be used to define how the group starts.

When 'group' is specified, the actor AI is paused while playing the animation, as if in an idle state. When one or more of 'lower', 'upper', or 'shield' are specified without 'group', the actor AI retains control, and the animations are layered. e.g. tes3.playAnimation{reference = ..., upper = tes3.animationGroup.idle3, loopCount = 0} will play the idle3 animation once (without looping) on the upper body, while the player or NPC is still able to walk around. Using 'upper' defers combat actions until the animation is done. Using 'shield' still allows combat actions.

As a special case, tes3.playAnimation{reference = ..., group = 0} returns control to the AI, as the AI knows that is the actor's neutral idle state.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3mobileActor`_, `tes3reference`_, `string`_)
    The reference that will play the animation.

group (`number`_)
    Optional. The animation group id to start playing -- a value from 0 to 149. Applies the animation to the whole body. Maps to tes3.animationGroup.* constants.

lower (`number`_)
    Optional. Sets the animation group id for the lower body. This is used to combine different animations for each body section. Maps to tes3.animationGroup.* constants.

upper (`number`_)
    Optional. Sets the animation group id for the upper body. This is used to combine different animations for each body section. Maps to tes3.animationGroup.* constants.

shield (`number`_)
    Optional. Sets the animation group id for the shield arm. This is used to combine different animations for each body section. Maps to tes3.animationGroup.* constants.

startFlag (`number`_)
    Default: ``tes3.animationStartFlag.immediate``. A flag for starting the group with, using tes3.animationStartFlag.* constants.

loopCount (`number`_)
    Default: ``-1``. If provided, the animation will repeat its loop section a given number of times. To make an animation play through once, set loopCount = 0. Defaults to infinite looping.

mesh (`string`_)
    Optional. Deprecated. Please use tes3.loadAnimation (check its documentation) before calling playAnimation. You can also use loadAnimation to reset loaded animations to default.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
