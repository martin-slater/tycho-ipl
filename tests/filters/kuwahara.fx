input int kernel_size = 5;

call clamp_image_size(src = __src__, dst = __src__, size = 1024);
call kuwahara(
	src = __src__, 
	dst = __dst__,
	kernel_size = kernel_size);
