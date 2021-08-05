tes3weatherController
====================================================================================================

A data structure that handles the weather.

Properties
----------------------------------------------------------------------------------------------------

`currentFogColor <tes3weatherController/currentFogColor.html>`_ (`tes3vector3`_)
    The current fog color.

`currentSkyColor <tes3weatherController/currentSkyColor.html>`_ (`tes3vector3`_)
    The current sky color.

`currentWeather <tes3weatherController/currentWeather.html>`_ (`tes3weather`_)
    The current weather.

`daysRemaining <tes3weatherController/daysRemaining.html>`_ (`number`_)
    The days remaining.

`hoursBetweenWeatherChange <tes3weatherController/hoursBetweenWeatherChange.html>`_ (`number`_)
    The hours between weather changes. Stored as a float value.

`hoursRemaining <tes3weatherController/hoursRemaining.html>`_ (`number`_)
    The hours remaining.

`lastActiveRegion <tes3weatherController/lastActiveRegion.html>`_ (`tes3region`_)
    Provides read-only access to last active region object.

`masser <tes3weatherController/masser.html>`_ (`tes3moon`_)
    Provides read-only access to the Masser moon object.

`nextWeather <tes3weatherController/nextWeather.html>`_ (`tes3weather`_)
    The next weather.

`sceneRainRoot <tes3weatherController/sceneRainRoot.html>`_ (`niNode`_)
    Provides read-only access to the scene rain root NiNode.

`sceneSkyLight <tes3weatherController/sceneSkyLight.html>`_ (`niDirectionalLight`_)
    Provides read-only access to the scene sky light directional light.

`sceneSkyRoot <tes3weatherController/sceneSkyRoot.html>`_ (`niNode`_)
    Provides read-only access to the scene sky root NiNode.

`sceneSnowRoot <tes3weatherController/sceneSnowRoot.html>`_ (`niNode`_)
    Provides read-only access to the scene snow root NiNode.

`sceneStormRoot <tes3weatherController/sceneStormRoot.html>`_ (`niNode`_)
    Provides read-only access to the scene storm root NiNode.

`sceneSunBase <tes3weatherController/sceneSunBase.html>`_ (`niNode`_)
    Provides read-only access to the scenesun base NiNode.

`sceneSunGlare <tes3weatherController/sceneSunGlare.html>`_ (`niNode`_)
    Provides read-only access to the scene sun glare NiNode.

`sceneSunVis <tes3weatherController/sceneSunVis.html>`_ (`niNode`_)
    Provides read-only access to the scene sun vis NiNode.

`secunda <tes3weatherController/secunda.html>`_ (`tes3moon`_)
    Provides read-only access to the Secunda moon object.

`sunglareFaderAngleMax <tes3weatherController/sunglareFaderAngleMax.html>`_ (`number`_)
    The sunglare fader maximum angle.

`sunglareFaderColor <tes3weatherController/sunglareFaderColor.html>`_ (`tes3vector3`_)
    The sunglare fader color.

`sunglareFaderMax <tes3weatherController/sunglareFaderMax.html>`_ (`number`_)
    The sunglare fader maximum.

`sunriseDuration <tes3weatherController/sunriseDuration.html>`_ (`number`_)
    The sunrise duration.

`sunriseHour <tes3weatherController/sunriseHour.html>`_ (`number`_)
    The sunrise hour.

`sunsetDuration <tes3weatherController/sunsetDuration.html>`_ (`number`_)
    The sunset duration.

`sunsetHour <tes3weatherController/sunsetHour.html>`_ (`number`_)
    The sunset hour.

`timescaleClouds <tes3weatherController/timescaleClouds.html>`_ (`string`_)
    The timescale for clouds.

`transitionScalar <tes3weatherController/transitionScalar.html>`_ (`number`_)
    The scalar transition.

`underwaterColor <tes3weatherController/underwaterColor.html>`_ (`tes3vector3`_)
    The underwater color, represented as a vector.

`underwaterColorWeight <tes3weatherController/underwaterColorWeight.html>`_ (`number`_)
    The underwater color weight.

`underwaterDayFog <tes3weatherController/underwaterDayFog.html>`_ (`number`_)
    The underwater day fog value.

`underwaterIndoorFog <tes3weatherController/underwaterIndoorFog.html>`_ (`number`_)
    The underwater indoor fog value.

`underwaterNightFog <tes3weatherController/underwaterNightFog.html>`_ (`number`_)
    The underwater night fog value.

`underwaterSunriseFog <tes3weatherController/underwaterSunriseFog.html>`_ (`number`_)
    The underwater sunrise fog value.

`underwaterSunsetFog <tes3weatherController/underwaterSunsetFog.html>`_ (`number`_)
    The underwater sunset fog value.

`weathers <tes3weatherController/weathers.html>`_ (`table`_)
    Array-style table for the different weather types. Each object in the table is a tes3weather.

`windVelocityCurrWeather <tes3weatherController/windVelocityCurrWeather.html>`_ (`tes3vector3`_)
    The wind velocity for the current weather.

`windVelocityNextWeather <tes3weatherController/windVelocityNextWeather.html>`_ (`tes3vector3`_)
    The wind velocity for the next weather.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3weatherController/currentFogColor
    tes3weatherController/currentSkyColor
    tes3weatherController/currentWeather
    tes3weatherController/daysRemaining
    tes3weatherController/hoursBetweenWeatherChange
    tes3weatherController/hoursRemaining
    tes3weatherController/lastActiveRegion
    tes3weatherController/masser
    tes3weatherController/nextWeather
    tes3weatherController/sceneRainRoot
    tes3weatherController/sceneSkyLight
    tes3weatherController/sceneSkyRoot
    tes3weatherController/sceneSnowRoot
    tes3weatherController/sceneStormRoot
    tes3weatherController/sceneSunBase
    tes3weatherController/sceneSunGlare
    tes3weatherController/sceneSunVis
    tes3weatherController/secunda
    tes3weatherController/sunglareFaderAngleMax
    tes3weatherController/sunglareFaderColor
    tes3weatherController/sunglareFaderMax
    tes3weatherController/sunriseDuration
    tes3weatherController/sunriseHour
    tes3weatherController/sunsetDuration
    tes3weatherController/sunsetHour
    tes3weatherController/timescaleClouds
    tes3weatherController/transitionScalar
    tes3weatherController/underwaterColor
    tes3weatherController/underwaterColorWeight
    tes3weatherController/underwaterDayFog
    tes3weatherController/underwaterIndoorFog
    tes3weatherController/underwaterNightFog
    tes3weatherController/underwaterSunriseFog
    tes3weatherController/underwaterSunsetFog
    tes3weatherController/weathers
    tes3weatherController/windVelocityCurrWeather
    tes3weatherController/windVelocityNextWeather

Methods
----------------------------------------------------------------------------------------------------

`calcSunDamageScalar <tes3weatherController/calcSunDamageScalar.html>`_ (`method`_)
    Calculates the sun damage based on the current weather.

`switchImmediate <tes3weatherController/switchImmediate.html>`_ (`method`_)
    Immediately switches the weather to the provided weather parameter.

`switchTransition <tes3weatherController/switchTransition.html>`_ (`method`_)
    Transitions the weather to the provided weather parameter based on the weather controller settings.

`updateVisuals <tes3weatherController/updateVisuals.html>`_ (`method`_)
    Updates the weather controller visuals. This should be called after any weather transitions.



.. toctree::
    :hidden:
    :maxdepth: 1

    tes3weatherController/calcSunDamageScalar
    tes3weatherController/switchImmediate
    tes3weatherController/switchTransition
    tes3weatherController/updateVisuals

.. _`method`: ../../lua/type/method.html
.. _`niDirectionalLight`: ../../lua/type/niDirectionalLight.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3moon`: ../../lua/type/tes3moon.html
.. _`tes3region`: ../../lua/type/tes3region.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weather`: ../../lua/type/tes3weather.html
