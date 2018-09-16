input int kernel_size = 5;
call edge_laplacian(
	src = __src__, 
	dst = __dst__,
	kernel_size=kernel_size,
	invert=true
	);
call min(src = __dst__, dst = __dst__);