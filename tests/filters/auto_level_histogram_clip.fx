input float clip_percent = 1;
call auto_level_histogram_clip(
	src = __src__, 
	dst = __dst__,
	clip_percent=clip_percent);

