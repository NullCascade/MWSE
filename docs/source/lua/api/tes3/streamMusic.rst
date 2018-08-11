
tes3.streamMusic
========================================================

**Parameters:**

- `string`_ **path**:
    Path to the music file, relative to Data Files/music/.

- `number`_ **situation** ``optional``:
    Determines what kind of gameplay situation the music should stay active for.
    ``tes3.musicSituation.explore`` - Plays during non-combat gameplay, ends when combat starts.
    ``tes3.musicSituation.combat`` - Plays during combat, ends when combat end.
    ``tes3.musicSituation.uninterruptible`` - Always plays, ends at the end the of track. Default.

- `number`_ **crossfade** ``optional``:
    The duration in seconds of the crossfade from the old to the new track. The default is ``1.0``.


**Returned:**

- None.


This function interrupts the current music to play the specified music track. 

.. _`number`: ../../type/lua/number.html
.. _`string`: ../../type/lua/string.html
