input float scale = 1;
input float delta = 0;
input int   kernel_size = 3;

call greyscale(src = __src__, dst = __dst__);
call edge_sobel(
	src = __src__, dst = __dst__,
	kernel_size=kernel_size,
	scale=scale,
	delta=delta,
	invert=true);
call min(src = __dst__, dst = __dst__);
