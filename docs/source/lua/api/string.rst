string
====================================================================================================

This library provides generic functions for string manipulation, such as finding and extracting substrings, and pattern matching. When indexing a string in Lua, the first character is at position 1 (not at 0, as in C). Indices are allowed to be negative and are interpreted as indexing backwards, from the end of the string. Thus, the last character is at position -1, and so on.

Functions
----------------------------------------------------------------------------------------------------

`endswith <string/endswith.html>`_ (`function`_)
    Returns true if a string ends with a given pattern.

`format <string/format.html>`_ (`function`_)
    This function creates a string, given various values. The format follows the printf format, with the additional option of %q to automatically quote a string.

`insert <string/insert.html>`_ (`function`_)
    Returns a string where one string has been inserted into another at a given position.

`multifind <string/multifind.html>`_ (`function`_)
    Performs the logic of find, using a table of patterns.

If any of the available patterns match, the matching pattern is returned followed by the normal results of the find.

`startswith <string/startswith.html>`_ (`function`_)
    Returns true if a string begins with a given pattern.



.. toctree::
    :hidden:
    :maxdepth: 1

    string/endswith
    string/format
    string/insert
    string/multifind
    string/startswith

.. _`function`: ../../lua/type/function.html
