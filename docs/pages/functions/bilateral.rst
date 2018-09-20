bilateral(src, sigma_color, filter_size, sigma_space, iterations)
=================================================================

Apply bilateral filter to the image. This maintains edges and softens flat regions.

**Inputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**src**", "*image*", "", "Source image"
   "**sigma_color**", "*int*", "20", ""
   "**filter_size**", "*int*", "9", ""
   "**sigma_space**", "*int*", "78", ""
   "**iterations**", "*int*", "1", "Number of times to apply the filter"

**Outputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**dst**", "*image*", "", "Destination image"

