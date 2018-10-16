simplify_colors(src, merge_distance, min_coverage)
==================================================

Simply the colors used in the image. This is not useful for general color reduction but can be useful when using logo type images that have antialiasing that needs to be removed to leave only the most dominant colors in the image

**Inputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**src**", "*image*", "", "Source image"
   "**merge_distance**", "*float*", "2.000", "Maximum euclidean distance between colors to consider them for merging"
   "**min_coverage**", "*float*", "0.500", "Minimum percentage area to color must cover to be considered for keeping"

**Outputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**dst**", "*image*", "", "Destination image"

.. image:: images/simplify_colors.png
