
tes3.playSound
========================================================

**Parameters:**

- `Sound`_ or `string`_ **sound**:
    The sound object, or id of the sound to look for.

- `Reference`_ or `MobileActor`_ or `string`_ **reference** ``optional``:
    The reference to attach the sound to.

- `boolean`_ **loop** ``optional``:
    The sound will loop if this is ``true``. Defaults to ``false``.

- `number`_ **mixChannel** ``optional``:
    A channel index that determines which game volume setting affects this sound. The channels are master, voice, effects, footsteps, and music, which can be selected from the table ``tes3.soundMix``. The default is ``effects``.

- `number`_ **volume** ``optional``:
    The volume of the playback in the range [0.0, 1.0]. This final volume is also affected by the mix channel. The default is ``1.0``.

- `number`_ **pitch** ``optional``:
    The pitch-shift multiplier. For 22kHz audio (most typical) it can have the range [0.005, 4.5]; for 44kHz audio it can have the range [0.0025, 2.25]. The default is ``1.0``.

**Returned:**

- `boolean`_ **result**: ``true`` if the sound object existed.

This function plays ``sound``. If reference is given, the sound is played by an emitter located at the reference's centre. If no reference is given, the sound is output directly.

Examples
--------------------------------------------------------

.. code-block:: lua

  local s = tes3.getSound("sneeze")
  tes3.playSound{ sound = s, mixChannel = tes3.soundMix.voice, volume = 0.7 }

.. _`boolean`: ../../type/lua/boolean.html
.. _`number`: ../../type/lua/number.html
.. _`string`: ../../type/lua/string.html

.. _`MobileActor`: ../../type/tes3/mobileActor.html
.. _`Reference`: ../../type/tes3/reference.html
.. _`Sound`: ../../type/tes3/sound.html
