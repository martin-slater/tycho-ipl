input int num_colors = 16;

call color_reduce_median_cut(
	src = __src__, 
	dst = __dst__,
	num_colors = num_colors);

call visualize_palette(
	src = __dst__,
	dst = __dst__);