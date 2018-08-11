
tes3.getSoundPlaying
========================================================

**Parameters:**

- `Sound`_ or `string`_ **sound**:
    The sound object, or id of the sound to look for.

- `Reference`_ or `MobileActor`_ or `string`_ **reference** ``optional``:
    The reference to test.

**Returned:**

- `boolean`_ **result**: ``true`` if the sound was playing.

Without a reference, getSoundPlaying returns ``true`` if the sound is playing unattached or on any reference. With a reference, getSoundPlaying returns ``true`` if the sound is playing on that specific reference.

.. _`boolean`: ../../type/lua/boolean.html
.. _`string`: ../../type/lua/string.html

.. _`MobileActor`: ../../type/tes3/mobileActor.html
.. _`Reference`: ../../type/tes3/reference.html
.. _`Sound`: ../../type/tes3/sound.html
