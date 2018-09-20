edge_sobel(src, kernel_size, scale, delta, invert)
==================================================

Find edges using Sobel filter

**Inputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**src**", "*image*", "", "Source image"
   "**kernel_size**", "*int*", "3", "Higher increases sensitivity. Must be one of 1, 3, 5 or 7."
   "**scale**", "*float*", "1.000", "Scale factor for the computed derivative values"
   "**delta**", "*float*", "0.000", "Delta value that is added to the results prior to storing them."
   "**invert**", "*bool*", "false", "Invert the results"

**Outputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**dst**", "*image*", "", "Destination image"

