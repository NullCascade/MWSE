tes3sound
====================================================================================================

A sound object created in the TES3 CS.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`filename <tes3sound/filename.html>`_ (`string`_)
    Path to the sound file.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`maxDistance <tes3sound/maxDistance.html>`_ (`number`_)
    The maximum distance the sound will be played at. Defaults to 0. If both this and minDistance are 0, the values are ignored.

`minDistance <tes3sound/minDistance.html>`_ (`number`_)
    The minimum distance the sound will be played at. Defaults to 0.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`volume <tes3sound/volume.html>`_ (`number`_)
    Direct access to sound's volume.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3sound/filename
    tes3sound/maxDistance
    tes3sound/minDistance
    tes3sound/volume

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3sound/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`isLooping <tes3sound/isLooping.html>`_ (`method`_)
    Returns true if the sound is actively looping.

`isPlaying <tes3sound/isPlaying.html>`_ (`method`_)
    Returns true if the sound is actively playing.

`play <tes3sound/play.html>`_ (`method`_)
    This metod plays the sound.

`stop <tes3sound/stop.html>`_ (`method`_)
    This stops playing the sound.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3sound/__tojson
    tes3sound/isLooping
    tes3sound/isPlaying
    tes3sound/play
    tes3sound/stop

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
