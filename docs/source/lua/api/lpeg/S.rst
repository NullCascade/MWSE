lpeg.S
====================================================================================================

Returns a pattern that matches any single character that appears in the given string.
	
As an example, the pattern lpeg.S("+-*/") matches any arithmetic operator.

Note that, if s is a character (that is, a string of length 1), then lpeg.P(s) is equivalent to 
lpeg.S(s) which is equivalent to lpeg.R(s..s). Note also that both lpeg.S("") and lpeg.R() are 
patterns that always fail.

Returns
----------------------------------------------------------------------------------------------------

result (`pattern`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

s (`string`_)
    The input string.

.. _`pattern`: ../../../lua/type/pattern.html
.. _`string`: ../../../lua/type/string.html
