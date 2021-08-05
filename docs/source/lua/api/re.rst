re
====================================================================================================

Provides a regular expression style syntax for pattern usage with lpeg.
	
The documentation provided here is only meant to be a quick reference. Please review the complete
documentation (with in-depth discussion and many examples) at: 
http://www.inf.puc-rio.br/~roberto/lpeg/re.html
	
There is also a copy in the MWSE repo at: deps/lpeg/re.html

Functions
----------------------------------------------------------------------------------------------------

`compile <re/compile.html>`_ (`function`_)
    Compiles the given string and returns an equivalent lpeg pattern.
	
The given string may define either an expression or a grammar. The optional defs table provides 
extra Lua values to be used by the pattern.

`find <re/find.html>`_ (`function`_)
    Searches the given pattern in the given subject.
	
If it finds a match, returns the index where this occurrence starts and the index where it ends. 
Otherwise, returns nil.

An optional numeric argument init makes the search starts at that position in the subject string.
As usual in Lua libraries, a negative value counts from the end. 

`gsub <re/gsub.html>`_ (`function`_)
    Does a global substitution, replacing all occurrences of pattern in the given subject by replacement.

`match <re/match.html>`_ (`function`_)
    Matches the given pattern against the given subject, returning all captures.

`updatelocale <re/updatelocale.html>`_ (`function`_)
    Updates the pre-defined character classes to the current locale.



.. toctree::
    :hidden:
    :maxdepth: 1

    re/compile
    re/find
    re/gsub
    re/match
    re/updatelocale

.. _`function`: ../../lua/type/function.html
