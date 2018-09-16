input int brightness = 255;
input float contrast = 0.5;

call image_adjust(
	src = __src__, dst = __dst__, 
	brightness=brightness, 
	contrast=contrast);
