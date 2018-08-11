
tes3.removeSound
========================================================

**Parameters:**

- `Sound`_ or `string`_ **sound**:
    The sound object, or id of the sound to look for.

- `Reference`_ or `MobileActor`_ or `string`_ **reference** ``optional``:
    The reference the sound is attached to.

**Returned:**

- **None.**

Stops a sound playing. Without a reference, it will match unattached sounds. With a reference, it will only match a sound playing on that specific reference.

.. _`string`: ../../type/lua/string.html

.. _`MobileActor`: ../../type/tes3/mobileActor.html
.. _`Reference`: ../../type/tes3/reference.html
.. _`Sound`: ../../type/tes3/sound.html
