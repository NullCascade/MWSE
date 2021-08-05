mwse
====================================================================================================

The mwse library provides methods for interacting with MWSE itself, rather than direct TES3 objects.

Values
----------------------------------------------------------------------------------------------------

`buildDate <mwse/buildDate.html>`_ (`number`_)
    A numerical representation of the date that version of MWSE currently being used was built on.

Formatted as YYYYMMDD.

`gameTimers <mwse/gameTimers.html>`_ (`mwseTimerController`_)
    The mwseTimerController responsible for game-type timers.

`realTimers <mwse/realTimers.html>`_ (`mwseTimerController`_)
    The mwseTimerController responsible for real-type timers.

`simulateTimers <mwse/simulateTimers.html>`_ (`mwseTimerController`_)
    The mwseTimerController responsible for simulate-type timers.

`version <mwse/version.html>`_ (`number`_)
    A numerical representation of the release version of MWSE currently being used.

Formatted as AAABBBCCC, where A is the major version, BBB is the minor version, and CCC is the patch version. BBB and CCC are forward-padded.

It is usually better to use mwse.buildDate instead.



.. toctree::
    :hidden:
    :maxdepth: 1

    mwse/buildDate
    mwse/gameTimers
    mwse/realTimers
    mwse/simulateTimers
    mwse/version

Functions
----------------------------------------------------------------------------------------------------

`getVersion <mwse/getVersion.html>`_ (`function`_)
    Equivalent to mwse.version.

`getVirtualMemoryUsage <mwse/getVirtualMemoryUsage.html>`_ (`function`_)
    Returns the amount of memory used, in bytes.

`loadConfig <mwse/loadConfig.html>`_ (`function`_)
    Loads a config table from Data Files\MWSE\config\{fileName}.json.
	
If the default values table is passed, empty keys in the config will be filled in using its values. Additionally, if no file exists, the function will return the default table.

`log <mwse/log.html>`_ (`function`_)
    This function writes information to the mwse.log file in the user's installation directory.

The message accepts formatting and additional parameters matching string.format's usage.

`longToString <mwse/longToString.html>`_ (`function`_)
    Converts a TES3 object type (e.g. from tes3.objectType) into an uppercase, 4-character string.

`overrideScript <mwse/overrideScript.html>`_ (`function`_)
    Configures MWSE to execute a given function instead when a script would run.

`saveConfig <mwse/saveConfig.html>`_ (`function`_)
    Saves a config table to Data Files\MWSE\config\{fileName}.json.

`stringToLong <mwse/stringToLong.html>`_ (`function`_)
    Converts an uppercase, 4-character string into a TES3 object type.



.. toctree::
    :hidden:
    :maxdepth: 1

    mwse/getVersion
    mwse/getVirtualMemoryUsage
    mwse/loadConfig
    mwse/log
    mwse/longToString
    mwse/overrideScript
    mwse/saveConfig
    mwse/stringToLong

.. _`function`: ../../lua/type/function.html
.. _`mwseTimerController`: ../../lua/type/mwseTimerController.html
.. _`number`: ../../lua/type/number.html
