input float high = 50;
input float low = 25;
input int   kernel_size = 5;

call edge_canny(
	src = __src__, dst = __dst__,
	threshold_low=low,
	threshold_high=high,
	kernel_size=kernel_size,
	invert=true);
call min(src = __dst__, dst = __dst__);
