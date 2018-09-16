input int tone_adjust = 0;

call sepia_yiq(
	src = __src__, 
	dst = __dst__,
	offset = tone_adjust);
