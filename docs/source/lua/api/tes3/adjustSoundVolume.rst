
tes3.adjustSoundVolume
========================================================

**Parameters:**

- `Sound`_ or `string`_ **sound**:
    The sound object, or id of the sound to look for.

- `Reference`_ or `MobileActor`_ or `string`_ **reference** ``optional``:
    The reference the sound is attached to.

- `number`_ **mixChannel** ``optional``:
    A channel index that determines which game volume setting affects this sound. The channels are master, voice, effects, footsteps, and music, which can be selected from the table ``tes3.soundMixChannel``. The default is ``effects``.

- `number`_ **volume** ``optional``:
    The volume of the playback in the range [0.0, 1.0]. This final volume is also affected by the mix channel. The default is ``1.0``.

**Returned:**

- **None.**

Adjusts the volume of a sound that is already playing.

.. _`boolean`: ../../type/lua/boolean.html
.. _`number`: ../../type/lua/number.html
.. _`string`: ../../type/lua/string.html

.. _`MobileActor`: ../../type/tes3/mobileActor.html
.. _`Reference`: ../../type/tes3/reference.html
.. _`Sound`: ../../type/tes3/sound.html
