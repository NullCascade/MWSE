
WeatherController
========================================================

Controls the weather simulation.


Properties
--------------------------------------------------------

weathers (`table`_)
    An array of all available weathers.

currentWeather (`Weather`_)
    The weather object for the current weather.

nextWeather (`Weather`_)
    The weather object for the next weather, the transition target.

transitionScalar (`number`_)
    A number between 0 and 1 measuring the transition between the current and next weather. Equal to 0 if no transition is occurring.

hoursBetweenWeatherChanges (`number`_)
    Time in hours between each regional weather randomization.

daysRemaining (`number`_)
    Integer days remaining until the next regional weather randomization.

hoursRemaining (`number`_)
    Time in hours remaining until the next regional weather randomization.

lastActiveRegion (`Region`_)
    Last exterior region that was simulated. While in interiors, this is still valid.
    
    **Warning:** On loading a game this will be ``nil`` until a cell with weather is visited.

currentFogColor (`Vector3`_)
    Fog color.

currentSkyColor (`Vector3`_)
    Sky color.

masser (`Moon`_)
    Masser moon data.

secunda (`Moon`_)
    Secunda moon data.

sunglareFaderAngleMax (`number`_)
    Angle between eye vector and sun, in radians, where sun blindness starts to appear.

sunglareFaderColor (`Vector3`_)
    Sun blindness overlay color.

sunglareFaderMax (`number`_)
    Sun blindness overlay maximum alpha.

sunriseDuration (`number`_)
    Sunrise duration in hours.

sunriseHour (`number`_)
    Sunrise start time.

sunsetDuration (`number`_)
    Sunset duration in hours.

sunsetHour (`number`_)
    Sunset start time.

timescaleClouds (`boolean`_)
    Controls if cloud movement is timescaled or realtime.

underwaterColor (`Vector3`_)
    Underwater ambient color.

underwaterColorWeight (`number`_)
    Blend fraction (0-1) of underwaterColor over weather ambient color.

underwaterDayFog (`Vector3`_)
    Underwater fog color at day time.

underwaterIndoorFog (`Vector3`_)
    Underwater fog color in interiors.

underwaterNightFog (`Vector3`_)
    Underwater fog color at night time.

underwaterSunriseFog (`Vector3`_)
    Underwater fog color at sunrise.

underwaterSunsetFog (`Vector3`_)
    Underwater fog color at sunset.


Functions
--------------------------------------------------------

**switchImmediate** (`number`_ weatherId)
    Changes the current weather to type ``weatherId`` immediately, also setting the region's current weather. This will not trigger a weatherChangedImmediate event.

**switchTransition** (`number`_ weatherId)
    Starts a weather transition to type ``weatherId``, also setting the region's current weather. This will not trigger a weatherTransitionStarted event, but weatherTransitionFinished will work.


.. _`boolean`: ../lua/boolean.html
.. _`number`: ../lua/number.html
.. _`table`: ../lua/table.html

.. _`Moon`: moon.html
.. _`Region`: region.html
.. _`Vector3`: vector3.html
.. _`Weather`: weather.html
