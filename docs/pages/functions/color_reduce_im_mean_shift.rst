color_reduce_im_mean_shift(src, kernel_size, color_distance)
============================================================

Reduces the number of colors in the image to a specified number.

**Inputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**src**", "*image*", "", "Source image"
   "**kernel_size**", "*int*", "7", "Size of kernel window. Must be odd and greater than 1."
   "**color_distance**", "*float*", "0.500", "Euclidean distance to merge colors in the range (0,1)"

**Outputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**dst**", "*image*", "", "Destination image"

