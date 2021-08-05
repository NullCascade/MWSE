table
====================================================================================================

This library provides generic functions for table manipulation. It provides all its functions inside the table table.

Functions
----------------------------------------------------------------------------------------------------

`bininsert <table/bininsert.html>`_ (`function`_)
    Inserts a given value through BinaryInsert into the table sorted by [, comp].

If 'comp' is given, then it must be a function that receives two table elements, and returns true when the first is less than the second, e.g. comp = function(a, b) return a > b end, will give a sorted table, with the biggest value on position 1. [, comp] behaves as in table.sort(table, value [, comp]) returns the index where 'value' was inserted

`binsearch <table/binsearch.html>`_ (`function`_)
    Performs a binary search for a given value.

If the value is found: It returns a table holding all the mathing indices (e.g. { startindice,endindice } ) endindice may be the same as startindice if only one matching indice was found

If compval is given: then it must be a function that takes one value and returns a second value2, to be compared with the input value, e.g.: compvalue = function( value ) return value[1] end

If reversed is set to true: then the search assumes that the table is sorted in reverse order (largest value at position 1) note when reversed is given compval must be given as well, it can be nil/_ in this case

Return value: on success: a table holding matching indices (e.g. { startindice,endindice } ) on failure: nil

`choice <table/choice.html>`_ (`function`_)
    Returns a random element from the given table.

`clear <table/clear.html>`_ (`function`_)
    This clears all keys and values from a table, but preserves the allocated array/hash sizes. This is useful when a table, which is linked from multiple places, needs to be cleared and/or when recycling a table for use by the same context. This avoids managing backlinks, saves an allocation and the overhead of incremental array/hash part growth.

Please note this function is meant for very specific situations. In most cases it's better to replace the (usually single) link with a new table and let the garbage collection do its work.

`copy <table/copy.html>`_ (`function`_)
    Shallowly copies a table's contents to a destination table. If no destination table is provided, a new table will be created. Note that sub tables will not be copied, and will still refer to the same data.

`copymissing <table/copymissing.html>`_ (`function`_)
    Copies a table's contents from one table to another, including subtitles. If a non-table key is already defined, it will not be overwritten. Metatables are not copied.

`deepcopy <table/deepcopy.html>`_ (`function`_)
    Copies a table's contents. All subtables will also be copied, as will any metatable.

`empty <table/empty.html>`_ (`function`_)
    Returns true if the table is empty, otherwise, it returns false.

`find <table/find.html>`_ (`function`_)
    Returns the key for a given value, or nil if the table does not contain the value.

`keys <table/keys.html>`_ (`function`_)
    Returns an array-style table of all keys in the given table, t. Optionally, it will sort the returned table.

`new <table/new.html>`_ (`function`_)
    This creates a pre-sized table. This is useful for big tables if the final table size is known and automatic table resizing is too expensive.

`removevalue <table/removevalue.html>`_ (`function`_)
    Removes a value from a given table. Returns true if the value was successfully removed.

`size <table/size.html>`_ (`function`_)
    Returns the number of elements inside the table. Unlike the length operator (#) this will work with any table.

`traverse <table/traverse.html>`_ (`function`_)
    This function is used to iterate over values and its subtables in the table.

Each "node" is an object with a children table of other "nodes", each of which might have their own children. For example, a sceneNode is made up of niNodes, and each niNodes can have a list of niNode children. This is best used for recursive data structures like UI elements and sceneNodes etc.

`values <table/values.html>`_ (`function`_)
    Returns an array-style table of all values in the given table, t. Optionally, it will sort the returned table.



.. toctree::
    :hidden:
    :maxdepth: 1

    table/bininsert
    table/binsearch
    table/choice
    table/clear
    table/copy
    table/copymissing
    table/deepcopy
    table/empty
    table/find
    table/keys
    table/new
    table/removevalue
    table/size
    table/traverse
    table/values

.. _`function`: ../../lua/type/function.html
