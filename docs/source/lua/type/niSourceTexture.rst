niSourceTexture
====================================================================================================

Represents all texture objects that are created from a file or a block of in-memory pixel data. NiSourceTexture objects represent both static and dynamic content, as NiSourceTexture data objects can have their pixel data modified on the fly to implement dynamic texture behavior.

This type inherits from the following parent types: `niObjectNET`_, `niObject`_

Properties
----------------------------------------------------------------------------------------------------

`fileName <niSourceTexture/fileName.html>`_ (`string`_)
    The platform-independent version of the filename from which the image was created, or NULL if the image was created from pixel data.

`formatPrefs <niSourceTexture/formatPrefs.html>`_ (`niFormatPrefs`_)
    The `NiFormatPrefs` of the texture.

`height <niSourceTexture/height.html>`_ (`number`_)
    The height of the texture.

`isStatic <niSourceTexture/isStatic.html>`_ (`boolean`_)
    The static flag.

`name <niObjectNET/name.html>`_ (`string`_)
    The human-facing name of the given object.

`pixelData <niSourceTexture/pixelData.html>`_ (`niPixelData`_)
    The app-level pixel data.

`platformFilename <niSourceTexture/platformFilename.html>`_ (`string`_)
    The platform-specific version of the filename.

`references <niObject/references.html>`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation <niObject/runTimeTypeInformation.html>`_ (`niRTTI`_)
    The runtime type information for this object.

`width <niSourceTexture/width.html>`_ (`number`_)
    The width of the texture.



.. toctree::
    :hidden:
    :maxdepth: 1

    niSourceTexture/fileName
    niSourceTexture/formatPrefs
    niSourceTexture/height
    niSourceTexture/isStatic
    niSourceTexture/pixelData
    niSourceTexture/platformFilename
    niSourceTexture/width

Methods
----------------------------------------------------------------------------------------------------

`clearPixelData <niSourceTexture/clearPixelData.html>`_ (`method`_)
    Detaches any pixel data associated with this texture. Any render-specific data will be maintained, and remain in the GPU's memory.

`clone <niObject/clone.html>`_ (`method`_)
    Creates a copy of this object.

`isInstanceOfType <niObject/isInstanceOfType.html>`_ (`method`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType <niObject/isOfType.html>`_ (`method`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`loadPixelDataFromFile <niSourceTexture/loadPixelDataFromFile.html>`_ (`method`_)
    Loads the file associated with the texture into memory, and makes it accessible from the pixelData property.

`prependController <niObjectNET/prependController.html>`_ (`method`_)
    Add a controller to the object as the first controller.

`removeAllControllers <niObjectNET/removeAllControllers.html>`_ (`method`_)
    Removes all controllers.

`removeController <niObjectNET/removeController.html>`_ (`method`_)
    Removes a controller from the object.



.. toctree::
    :hidden:
    :maxdepth: 1

    niSourceTexture/clearPixelData
    niSourceTexture/loadPixelDataFromFile

Functions
----------------------------------------------------------------------------------------------------

`createFromPath <niSourceTexture/createFromPath.html>`_ (`function`_)
    Creates an NiSourceTexture from the given filepath.



.. toctree::
    :hidden:
    :maxdepth: 1

    niSourceTexture/createFromPath

.. _`boolean`: ../../lua/type/boolean.html
.. _`function`: ../../lua/type/function.html
.. _`method`: ../../lua/type/method.html
.. _`niFormatPrefs`: ../../lua/type/niFormatPrefs.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niObjectNET`: ../../lua/type/niObjectNET.html
.. _`niPixelData`: ../../lua/type/niPixelData.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
