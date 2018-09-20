threshold(src, threshold, maxval, type)
=======================================

Remove noise from the passed image.

**Inputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**src**", "*image*", "", "Source image"
   "**threshold**", "*int*", "", "Threshold value"
   "**maxval**", "*int*", "", "Maximum value to use with the THRESH_BINARY and THRESH_BINARY_INV thresholding types"
   "**type**", "*int*", "", "Thresholding type"

**Outputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**dst**", "*image*", "", "Destination image"

