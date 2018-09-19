auto_level_histogram_clip(src, clip_percent)
============================================

Auto levels image lighting based on the image histogram.

**Inputs**

.. csv-table::
	:header: "name", "type", "default", "description"
	:widths: 20,10,10,60

	"**src**", "*image*", "", "Source image"
	"**clip_percent**", "*float*", "1.000", "percentage to clip from histogram"

**Outputs**

.. csv-table::
	:header: "name", "type", "default", "description"
	:widths: 20,10,10,60

	"**dst**", "*image*", "", "Destination image"

