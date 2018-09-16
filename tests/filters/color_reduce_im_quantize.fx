input int num_colors = 256;

call color_reduce_im_quantize(
	src = __src__,
	dst = __dst__,
	num_colors = num_colors);

call visualize_palette(
	src = __dst__,
	dst = __dst__);