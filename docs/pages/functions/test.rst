remove_intensity(src, black-cutoff=64)
=======================

Removes the intensity from the image. If the image is in a colour space that contains an intensity channel (Lab, HLS, HSV) then this will be cleared. If the image is in RGB space then each texel is normalized as sum = R + G + B, R' = R / sum, G' = G / sum, B' = B / sum. If sum is less than black-cutoff then it is clamped to zero.

Inputs
----------------------------------------

.. csv-table::
    :header: "name", "type", "default", "description"

    "**src**", "*image*", "", "Source image"
    "**black-cutoff**", "*int*", "25", "Value under which the sum (A + G + B) will be clamped to 0"

Outputs
-----------------------------------------

 <out>  dst     : Destination image