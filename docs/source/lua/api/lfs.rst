lfs
====================================================================================================

LuaFileSystem is a Lua library developed to complement the set of functions related to file systems offered by the standard Lua distribution. This library has been further extended by MWSE.

Functions
----------------------------------------------------------------------------------------------------

`attributes <lfs/attributes.html>`_ (`function`_)
    Returns a table with the file attributes corresponding to filepath (or nil followed by an error message and a system-dependent error code in case of error). If the second optional argument is given and is a string, then only the value of the named attribute is returned (this use is equivalent to lfs.attributes(filepath)[request_name], but the table is not created and only one attribute is retrieved from the O.S.). if a table is passed as the second argument, it (result_table) is filled with attributes and returned instead of a new table. The attributes are described as follows; attribute mode is a string, all the others are numbers, and the time related attributes use the same time reference of os.time: dev, mode, nlink, access, modification, change, size, permissions.

`chdir <lfs/chdir.html>`_ (`function`_)
    Changes the current working directory to the given path. Note that this is global. Do not use this function without restoring the previous directory at the end of your logic. Failure to do so can cause unrecoverable errors.

`currentdir <lfs/currentdir.html>`_ (`function`_)
    Changes the current working directory to the given path.

`dir <lfs/dir.html>`_ (`function`_)
    Creates a Lua iterator over the entries of a given directory. This can be used to create a for loop of directories. Be wary that current and parent directory markers may be available in the results.

`directoryexists <lfs/directoryexists.html>`_ (`function`_)
    Simple check to determine if a directory exists.

`fileexists <lfs/fileexists.html>`_ (`function`_)
    Simple check to determine if a file exists.

`link <lfs/link.html>`_ (`function`_)
    Creates a link. This is a hard link by default.

`lock <lfs/lock.html>`_ (`function`_)
    Locks a file or a part of it. This function works on open files; the file handle should be specified as the first argument.

`lock_dir <lfs/lock_dir.html>`_ (`function`_)
    Creates a lockfile (called lockfile.lfs) in path if it does not exist and returns the lock. If the lock already exists checks if it's stale, using the second parameter (default for the second parameter is INT_MAX, which in practice means the lock will never be stale. The lock object has a method to free it.

`mkdir <lfs/mkdir.html>`_ (`function`_)
    Creates a new directory.

`rmdir <lfs/rmdir.html>`_ (`function`_)
    Removes an existing directory. Unless using the recursive parameter, only empty directories can be removed.

`setmode <lfs/setmode.html>`_ (`function`_)
    Sets the writing mode for a file.

`touch <lfs/touch.html>`_ (`function`_)
    Set access and modification times of a file. Both access and modified times are measured in seconds, matching the OS time function.

`unlock <lfs/unlock.html>`_ (`function`_)
    Unlocks a file or a part of it. This function works on open files; the file handle should be specified as the first argument.



.. toctree::
    :hidden:
    :maxdepth: 1

    lfs/attributes
    lfs/chdir
    lfs/currentdir
    lfs/dir
    lfs/directoryexists
    lfs/fileexists
    lfs/link
    lfs/lock
    lfs/lock_dir
    lfs/mkdir
    lfs/rmdir
    lfs/setmode
    lfs/touch
    lfs/unlock

.. _`function`: ../../lua/type/function.html
