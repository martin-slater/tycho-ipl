input int kernel_size = 5;
input int num_levels = 10;

call clamp_image_size(src = __src__, dst = __src__, size = 1024);
call oil_painting(
	src = __src__, 
	dst = __dst__,
	kernel_size = kernel_size,
	levels=num_levels);
