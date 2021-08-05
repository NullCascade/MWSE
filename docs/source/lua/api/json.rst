json
====================================================================================================

Provides support for interacting with json data through an extended dkjson module.

Values
----------------------------------------------------------------------------------------------------

`null <json/null.html>`_ (`number`_)
    You can use this value for setting explicit null values.

`version <json/version.html>`_ (`string`_)
    Current version of dkjson.



.. toctree::
    :hidden:
    :maxdepth: 1

    json/null
    json/version

Functions
----------------------------------------------------------------------------------------------------

`decode <json/decode.html>`_ (`function`_)
    Decode string into a table.

`encode <json/encode.html>`_ (`function`_)
    Create a string representing the object. Object can be a table, a string, a number, a boolean, nil, json.null or any object with a function __tojson in its metatable. A table can only use strings and numbers as keys and its values have to be valid objects as well. It raises an error for any invalid data types or reference cycles.

`loadfile <json/loadfile.html>`_ (`function`_)
    Loads the contents of a file through json.decode. Files loaded from Data Files\MWSE\{fileName}.json.

`quotestring <json/quotestring.html>`_ (`function`_)
    Quote a UTF-8 string and escape critical characters using JSON escape sequences. This function is only necessary when you build your own __tojson functions.

`savefile <json/savefile.html>`_ (`function`_)
    Saves a serializable table to Data Files\MWSE\{fileName}.json, using json.encode.



.. toctree::
    :hidden:
    :maxdepth: 1

    json/decode
    json/encode
    json/loadfile
    json/quotestring
    json/savefile

.. _`function`: ../../lua/type/function.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
