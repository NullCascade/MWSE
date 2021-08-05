tes3light
====================================================================================================

A core light object. This isn't actually a light in the rendering engine, but something like a lamp or torch.

This type inherits from the following parent types: `tes3item`_, `tes3physicalObject`_, `tes3object`_, `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`boundingBox <tes3physicalObject/boundingBox.html>`_ (`tes3boundingBox`_)
    The bounding box for the object.

`canCarry <tes3light/canCarry.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light can be carried.

`color <tes3light/color.html>`_ (`table`_)
    Access to the light's base colors, in an array-style table of four values. The values can range from 0 to 255.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`flickers <tes3light/flickers.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation flickers.

`flickersSlowly <tes3light/flickersSlowly.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation flickers slowly.

`icon <tes3light/icon.html>`_ (`string`_)
    The path to the object's icon.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`isDynamic <tes3light/isDynamic.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light affects dynamically moving objects.

`isFire <tes3light/isFire.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light represents flame.

`isNegative <tes3light/isNegative.html>`_ (`boolean`_)
    Access to the light's flags, determining if the object creates darkness.

`isOffByDefault <tes3light/isOffByDefault.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light won't be active initially.

`mesh <tes3light/mesh.html>`_ (`string`_)
    The path to the object's mesh.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3light/name.html>`_ (`string`_)
    The player-facing name for the object.

`nextInCollection <tes3object/nextInCollection.html>`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`owningCollection <tes3object/owningCollection.html>`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`previousInCollection <tes3object/previousInCollection.html>`_ (`tes3object`_)
    The previous object in parent collection's list.

`pulses <tes3light/pulses.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation pulses.

`pulsesSlowly <tes3light/pulsesSlowly.html>`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation pulses slowly.

`radius <tes3light/radius.html>`_ (`number`_)
    The base radius of the light.

`scale <tes3object/scale.html>`_ (`number`_)
    The object's scale.

`sceneNode <tes3object/sceneNode.html>`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference <tes3object/sceneReference.html>`_ (`niNode`_)
    The scene graph reference node for this object.

`script <tes3light/script.html>`_ (`tes3script`_)
    The script that runs on the object.

`sound <tes3light/sound.html>`_ (`tes3sound`_)
    The sound that runs on the object.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList <tes3physicalObject/stolenList.html>`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`time <tes3light/time.html>`_ (`number`_)
    The amount of time that the light will last.

`value <tes3light/value.html>`_ (`number`_)
    The value of the object.

`weight <tes3light/weight.html>`_ (`number`_)
    The weight of the object.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3light/canCarry
    tes3light/color
    tes3light/flickers
    tes3light/flickersSlowly
    tes3light/icon
    tes3light/isDynamic
    tes3light/isFire
    tes3light/isNegative
    tes3light/isOffByDefault
    tes3light/mesh
    tes3light/name
    tes3light/pulses
    tes3light/pulsesSlowly
    tes3light/radius
    tes3light/script
    tes3light/sound
    tes3light/time
    tes3light/value
    tes3light/weight

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`getTimeLeft <tes3light/getTimeLeft.html>`_ (`method`_)
    Gets the time remaining for a light, given a tes3itemData, tes3reference, or tes3equipmentStack.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3light/getTimeLeft

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3script`: ../../lua/type/tes3script.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
