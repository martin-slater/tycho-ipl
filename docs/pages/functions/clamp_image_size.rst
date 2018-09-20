clamp_image_size(src, size, enlarge)
====================================

Clamp an image to a given size whilst maintaining its aspect ratio

**Inputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**src**", "*image*", "", "Source image"
   "**size**", "*int*", "", "Size in pixels to clamp longest edge to."
   "**enlarge**", "*bool*", "false", "True to scale smaller images to the given size."

**Outputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**dst**", "*image*", "", "Destination image"

