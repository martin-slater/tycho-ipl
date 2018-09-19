image_adjust(src, brightness, contrast)
=======================================

Adjust the contrast and brightness of an image

**Inputs**

.. csv-table::
	:header: "name", "type", "default", "description"
	:widths: 20,10,10,60

	"**src**", "*image*", "", "Source image"
	"**brightness**", "*int*", "0", "Brightness in range [0,255] to add to pixel."
	"**contrast**", "*float*", "0.000", "Amount to scale existing pixel by."

**Outputs**

.. csv-table::
	:header: "name", "type", "default", "description"
	:widths: 20,10,10,60

	"**dst**", "*image*", "", "Destination image"

