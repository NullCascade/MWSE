lpeg.R
====================================================================================================

Returns a pattern that matches any single character belonging to one of the given ranges.
	
Each range is a string xy of length 2, representing all characters with code between the codes of x and y (both inclusive).

As an example, the pattern lpeg.R("09") matches any digit, and lpeg.R("az", "AZ") matches any ASCII letter.

Returns
----------------------------------------------------------------------------------------------------

result (`pattern`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The input pattern.

.. _`pattern`: ../../../lua/type/pattern.html
