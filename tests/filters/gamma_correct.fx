input float gamma = 1.0;

call gamma_correct(
	src = __src__, 
	dst = __dst__,
	gamma=gamma);
