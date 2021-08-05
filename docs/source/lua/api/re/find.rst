re.find
====================================================================================================

Searches the given pattern in the given subject.
	
If it finds a match, returns the index where this occurrence starts and the index where it ends. 
Otherwise, returns nil.

An optional numeric argument init makes the search starts at that position in the subject string.
As usual in Lua libraries, a negative value counts from the end. 

Returns
----------------------------------------------------------------------------------------------------

result (`number`_)
    No description available.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

subject (`string`_)
    The string to search.

pattern (`string`_)
    The pattern to search with.

init (`number`_)
    Optional. Start at this position in the subject string.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
