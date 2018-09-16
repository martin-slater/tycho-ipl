input int kernel_size = 5;
input float color_distance = 0.1;

call color_reduce_im_mean_shift(
	src = __src__, 
	dst = __dst__,
	kernel_size = kernel_size,
	color_distance = color_distance);

call visualize_palette(
	src = __dst__,
	dst = __dst__);