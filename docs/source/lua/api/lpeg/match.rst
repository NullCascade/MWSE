lpeg.match
====================================================================================================

Attempt to match the given pattern against the subject string.
	
If the match succeeds, return the index in the subject of the first character after the match, or
the captured values (if the pattern captured any value).

An optional numeric argument init makes the match start at that position in the subject string. As 
usual in Lua libraries, a negative value counts from the end.

Unlike typical pattern-matching functions, match works only in anchored mode; that is, it tries to 
match the pattern with a prefix of the given subject string (at position init), not with an 
arbitrary substring of the subject. So, if we want to find a pattern anywhere in a string, we must
either write a loop in Lua or write a pattern that matches anywhere. This second approach is easy 
and quite efficient; see examples in the full documentation for details.

Returns
----------------------------------------------------------------------------------------------------

result (`values`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The pattern to match.

subject (`string`_)
    The string to match against.

init (`number`_)
    Optional. Start the match at this position in subject.

.. _`number`: ../../../lua/type/number.html
.. _`pattern`: ../../../lua/type/pattern.html
.. _`string`: ../../../lua/type/string.html
.. _`values`: ../../../lua/type/values.html
