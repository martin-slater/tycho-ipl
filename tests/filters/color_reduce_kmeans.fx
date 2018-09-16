input int num_colors = 16;
input int num_attempts = 3;
input int term_epsilon = 60;
input int term_iterations = 3;

call color_reduce_kmeans(
	src = __src__, 
	dst = __dst__,
	num_colors = num_colors,
	num_attempts = num_attempts,
	term_epsilon = term_epsilon,
	term_iterations = term_iterations);

call visualize_palette(
	src = __dst__,
	dst = __dst__);