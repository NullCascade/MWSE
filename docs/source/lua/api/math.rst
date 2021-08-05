math
====================================================================================================

This library is an interface to the standard C math library. It provides all its functions inside the table math.

Functions
----------------------------------------------------------------------------------------------------

`clamp <math/clamp.html>`_ (`function`_)
    Returns a value, limited by upper and lower bounds.

`lerp <math/lerp.html>`_ (`function`_)
    Performs linear interpolation between values v0 and v1. Returns a value that is t percent between them.

`remap <math/remap.html>`_ (`function`_)
    Returns a value, scaled from expected values [lowIn, highIn] to [lowOut, highOut].

For example, a value of 7 remapped from [0,10] to [0,100] would be 70.

`round <math/round.html>`_ (`function`_)
    Rounds a number to a given count of digits.



.. toctree::
    :hidden:
    :maxdepth: 1

    math/clamp
    math/lerp
    math/remap
    math/round

.. _`function`: ../../lua/type/function.html
