
xFirstStatic
========================================================

**Parameters:**

- *None*

**Returned:**

- ``long`` **reference**: The first reference in the current cell's "Static" list.

`xFirstStatic`_ returns the first reference in the current cell's "Static" list. Despite the name of this function, the associated list can contain references of any `Object Type`_ other than those exclusive to `xFirstNPC`_. Once acquired, the reference can be used with various other MWSE functions, such as `xRefID`_ and `xRefType`_. Passing the reference as the argument to the `xNextRef`_ function will return the next reference in its associated list.

.. note:: Scripts that intend to process *all* references in the loaded cells will need to independently handle the separate lists provided by `xFirstItem`_, `xFirstStatic`_, and `xFirstNPC`_.

.. _`Object Type`: ../../references.html#object-types
.. _`xFirstItem`: xFirstItem.html
.. _`xFirstNPC`: xFirstNPC.html
.. _`xFirstStatic`: xFirstStatic.html
.. _`xNextRef`: xNextRef.html
.. _`xRefID`: xRefID.html
.. _`xRefType`: xRefType.html