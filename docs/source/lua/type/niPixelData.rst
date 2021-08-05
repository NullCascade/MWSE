niPixelData
====================================================================================================

Represents 2D arrays of pixel values, as well as pyramids of multiple mipmap levels, each of 2D arrays of pixel values. Also contains information representing the format of the pixels and dimensions of the arrays.

This type inherits from the following parent types: `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`bytesPerPixel <niPixelData/bytesPerPixel.html>`_ (`number`_)
    The number of bytes per pixel.

`mipMapLevels <niPixelData/mipMapLevels.html>`_ (`number`_)
    The number of MipMap levels.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.



.. toctree::
    :hidden:
    :maxdepth: 1

    niPixelData/bytesPerPixel
    niPixelData/mipMapLevels

Methods
----------------------------------------------------------------------------------------------------

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`createSourceTexture <niPixelData/createSourceTexture.html>`_ (`method`_)
    Creates a source texture from the current NiPixelData object.

`getHeight <niPixelData/getHeight.html>`_ (`method`_)
    Returns the height of the mipmap level at the given index, where level 1 is the largest.

`getWidth <niPixelData/getWidth.html>`_ (`method`_)
    Returns the width of the mipmap level at the given index, where level 1 is the largest.

`isInstanceOfType <niObject/isInstanceOfType.html>`_ (`method`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType <niObject/isOfType.html>`_ (`method`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`setPixelsByte <niPixelData/setPixelsByte.html>`_ (`method`_)
    Sets the pixel data from byte data.

`setPixelsFloat <niPixelData/setPixelsFloat.html>`_ (`method`_)
    Sets the pixel data from float data.



.. toctree::
    :hidden:
    :maxdepth: 1

    niPixelData/createSourceTexture
    niPixelData/getHeight
    niPixelData/getWidth
    niPixelData/setPixelsByte
    niPixelData/setPixelsFloat

Functions
----------------------------------------------------------------------------------------------------

`new <niPixelData/new.html>`_ (`function`_)
    Creates a new NiPixelData object.



.. toctree::
    :hidden:
    :maxdepth: 1

    niPixelData/new

.. _`function`: ../../lua/type/function.html
.. _`method`: ../../lua/type/method.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
