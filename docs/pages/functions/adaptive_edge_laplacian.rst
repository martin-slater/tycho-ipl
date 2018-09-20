adaptive_edge_laplacian(src, edge_percent, min, invert, adaptive_cutoff)
========================================================================

Find edges using a Laplacian filter. This will search the filter space for the result that contains the closest to the requested percentage of edge pixels

**Inputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**src**", "*image*", "", "Source image"
   "**edge_percent**", "*int*", "5", "Percentage of edge pixels to target"
   "**min**", "*int*", "64", "Minimum value to be classified as an edge pixels. Pixels >= than this will be forced to 255"
   "**invert**", "*bool*", "false", "Invert the results"
   "**adaptive_cutoff**", "*bool*", "false", "Refine the best image found searching the filter space by changing the cutoff value"

**Outputs**

.. csv-table::
   :header: "name", "type", "default", "description"
   :widths: 20,10,10,60

   "**dst**", "*image*", "", "Destination image"

