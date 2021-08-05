tes3region
====================================================================================================

A structure that contains region information.

This type inherits from the following parent types: `tes3baseObject`_

Properties
----------------------------------------------------------------------------------------------------

`blocked <tes3baseObject/blocked.html>`_ (`boolean`_)
    The blocked state of the object.

`deleted <tes3baseObject/deleted.html>`_ (`boolean`_)
    The deleted state of the object.

`disabled <tes3baseObject/disabled.html>`_ (`boolean`_)
    The disabled state of the object.

`id <tes3baseObject/id.html>`_ (`string`_)
    The unique identifier for the object.

`modified <tes3baseObject/modified.html>`_ (`boolean`_)
    The modification state of the object since the last save.

`name <tes3region/name.html>`_ (`string`_)
    The region's name. Must be a string less than 32 characters.

`objectFlags <tes3baseObject/objectFlags.html>`_ (`number`_)
    The raw flags of the object.

`objectType <tes3baseObject/objectType.html>`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`persistent <tes3baseObject/persistent.html>`_ (`boolean`_)
    The persistent flag of the object.

`sleepCreature <tes3region/sleepCreature.html>`_ (`tes3leveledCreature`_)
    The region's leveled creature that can be spawned while the player is sleeping.

`sounds <tes3region/sounds.html>`_ (`table`_)
    Array-style table for the different region sounds. Each object in the table is a tes3regionSound.

`sourceless <tes3baseObject/sourceless.html>`_ (`boolean`_)
    The soruceless flag of the object.

`sourceMod <tes3baseObject/sourceMod.html>`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData <tes3baseObject/supportsLuaData.html>`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`weather <tes3region/weather.html>`_ (`tes3weather`_)
    The region's current weather.

`weatherChanceAsh <tes3region/weatherChanceAsh.html>`_ (`number`_)
    The chance the Ash weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceBlight <tes3region/weatherChanceBlight.html>`_ (`number`_)
    The chance the Blight weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceBlizzard <tes3region/weatherChanceBlizzard.html>`_ (`number`_)
    The chance the Blizzard weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceClear <tes3region/weatherChanceClear.html>`_ (`number`_)
    The chance the Clear weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceCloudy <tes3region/weatherChanceCloudy.html>`_ (`number`_)
    The chance the Cloudy weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceFoggy <tes3region/weatherChanceFoggy.html>`_ (`number`_)
    The chance the Foggy weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceOvercast <tes3region/weatherChanceOvercast.html>`_ (`number`_)
    The chance the Overcast weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceRain <tes3region/weatherChanceRain.html>`_ (`number`_)
    The chance the Rain weather-type will be used. Must be an integer between 0 - 100.

`weatherChances <tes3region/weatherChances.html>`_ (`table`_)
    Array-style table for the different weather chances. Each object in the table is a number, between 1 - 100. The index of the table is the weather type.

`weatherChanceSnow <tes3region/weatherChanceSnow.html>`_ (`number`_)
    The chance the Snow weather-type will be used. Must be an integer between 0 - 100.

`weatherChanceThunder <tes3region/weatherChanceThunder.html>`_ (`number`_)
    The chance the Thunder weather-type will be used. Must be an integer between 0 - 100.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3region/name
    tes3region/sleepCreature
    tes3region/sounds
    tes3region/weather
    tes3region/weatherChanceAsh
    tes3region/weatherChanceBlight
    tes3region/weatherChanceBlizzard
    tes3region/weatherChanceClear
    tes3region/weatherChanceCloudy
    tes3region/weatherChanceFoggy
    tes3region/weatherChanceOvercast
    tes3region/weatherChanceRain
    tes3region/weatherChances
    tes3region/weatherChanceSnow
    tes3region/weatherChanceThunder

Methods
----------------------------------------------------------------------------------------------------

`__tojson <tes3baseObject/__tojson.html>`_ (`method`_)
    Serializes the object to json.

`changeWeather <tes3region/changeWeather.html>`_ (`method`_)
    Changes the current weather for the region to the provided weather-type parameter.

`randomizeWeather <tes3region/randomizeWeather.html>`_ (`method`_)
    Changes the weather to a random weather type.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3region/changeWeather
    tes3region/randomizeWeather

.. _`boolean`: ../../lua/type/boolean.html
.. _`method`: ../../lua/type/method.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3leveledCreature`: ../../lua/type/tes3leveledCreature.html
.. _`tes3weather`: ../../lua/type/tes3weather.html
