color_reduce_libimagequant(src, num_colors)
===========================================

Reduces the number of colors in the image to a specified number.

**Inputs**

.. csv-table::
	:header: "name", "type", "default", "description"
	:widths: 20,10,10,60

	"**src**", "*image*", "", "Source image"
	"**num_colors**", "*int*", "256", "Number of colors to reduce the image to. If it is 0 then it will automatically determine the number of colors."

**Outputs**

.. csv-table::
	:header: "name", "type", "default", "description"
	:widths: 20,10,10,60

	"**dst**", "*image*", "", "Destination image"

