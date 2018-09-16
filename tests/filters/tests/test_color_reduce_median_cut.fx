temp image c256, c128, c64, c32, c16, c8, c4;
temp image lab, lc256, lc128, lc64, lc32, lc16, lc8, lc4;

call clamp_image_size(src = __src__, dst = __src__, size = 512);


call convert_image(src = __src__, dst = lab, format = FORMAT_LAB);
call color_reduce_median_cut(src = lab, dst = lc256, num_colors = 256);
call color_reduce_median_cut(src = lab, dst = lc128, num_colors = 128);
call color_reduce_median_cut(src = lab, dst = lc64, num_colors = 64);
call color_reduce_median_cut(src = lab, dst = lc32, num_colors = 32);
call color_reduce_median_cut(src = lab, dst = lc16, num_colors = 16);
call color_reduce_median_cut(src = lab, dst = lc8, num_colors = 8);
call color_reduce_median_cut(src = lab, dst = lc4, num_colors = 4);

call convert_image(src = lc256, dst = lc256, format = FORMAT_RGB);
call convert_image(src = lc128, dst = lc128, format = FORMAT_RGB);
call convert_image(src = lc64, dst = lc64, format = FORMAT_RGB);
call convert_image(src = lc32, dst = lc32, format = FORMAT_RGB);
call convert_image(src = lc16, dst = lc16, format = FORMAT_RGB);
call convert_image(src = lc8, dst = lc8, format = FORMAT_RGB);
call convert_image(src = lc4, dst = lc4, format = FORMAT_RGB);

call color_reduce_median_cut(src = __src__, dst = c256, num_colors = 256);
call color_reduce_median_cut(src = __src__, dst = c128, num_colors = 128);
call color_reduce_median_cut(src = __src__, dst = c64, num_colors = 64);
call color_reduce_median_cut(src = __src__, dst = c32, num_colors = 32);
call color_reduce_median_cut(src = __src__, dst = c16, num_colors = 16);
call color_reduce_median_cut(src = __src__, dst = c8, num_colors = 8);
call color_reduce_median_cut(src = __src__, dst = c4, num_colors = 4);


call experiment_add_image(src = c256, name = "median_cuts_256");
call experiment_add_image(src = lc256, name = "median_cuts_256_lab");
call experiment_add_image(src = c128, name = "median_cuts_128");
call experiment_add_image(src = lc128, name = "median_cuts_128_lab");
call experiment_add_image(src = c64,  name = "median_cuts_64");
call experiment_add_image(src = lc64,  name = "median_cuts_64_lab");
call experiment_add_image(src = c32, name = "median_cuts_32");
call experiment_add_image(src = lc32, name = "median_cuts_32_lab");
call experiment_add_image(src = c16, name = "median_cuts_16");
call experiment_add_image(src = lc16, name = "median_cuts_16_lab");
call experiment_add_image(src = c8, name = "median_cuts_8");
call experiment_add_image(src = lc8, name = "median_cuts_8_lab");
call experiment_add_image(src = c4, name = "median_cuts_4");
call experiment_add_image(src = lc4, name = "median_cuts_4_lab");
