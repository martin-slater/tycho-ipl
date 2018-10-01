Scripting support
======================

A simple scripting language is supplied that is sufficient for
combining multiple filters (algorithms). By convention these have
a .fx suffix.

Variable definitions
--------------------

A variable definition takes the following form

``<specifier> <type> name = <optional default>;``

where type is one of

* **int**, **float**, **image**, **string**, **bool**

and specifier is one of


.. csv-table::
   :widths: 20, 80

   "**input**", "Variables defined as inputs can be set from the command line"
   "**temp**", "Temporary variable used in the script"
   "**const**", "Constants, must be initialised on declaration."


**Default variables**

The runtime exposes the following variables automatically to the executing
script.

.. csv-table::
    :header: "name", "type", "description"
    :widths: 20, 20, 60

    "__src__", "**image**", "Source input image to the script"
    "__dst__", "**image**", "Destination image to return the result of the script in"
    "__width__", "**int**", "Width of the source image"
    "__height__", "**int**", "Height of the source image"

Constants
---------

The  runtime exposes the following constants to the script.
Functions may also expose their own constants so check the specific
function documentation for more information.

**Image Formats**

.. csv-table::
   :header: "name", "description"
   :widths: 20, 80

   "FORMAT_RGB", "32bit RGB image"
   "FORMAT_LAB", "LAB color space"
   "FORMAT_HLS", "HSV image"
   "FORMAT_HSV", "HSV image"
   "FORMAT_GREY", "Greyscale image"


**Morphological structuring elements**

.. csv-table::
   :header: "Name", "description"
   :widths: 20, 80

   "MORPH_RECT", ""
   "MORPH_CROSS", ""
   "MORPH_ELLIPSE", ""

Function calls
--------------

A function call takes the following form.

``call <name>(<key>=<value>, ...)``

Each parameter is passed as key-value pair. All values types (int, float etc)
are passed by value and all images passed by reference.
i.e. to convert the input image to greyscale

``call greyscale(src=__src__, dst=__dst__)``


