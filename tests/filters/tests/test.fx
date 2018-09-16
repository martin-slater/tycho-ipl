temp image clean, out_rgb, levelled, edges;

call denoise(src = __src__, dst = clean, strength = 4);
call auto_level(src = clean, dst = out_rgb);
call remove_intensity(src = out_rgb, dst = out_rgb);
call save_image(src = out_rgb, path = "!!remove_intensity_rgb.jpg");
#call auto_level(src = out_rgb, dst = levelled);
#call image_adjust(src = out_rgb, dst = out_rgb, contrast = 0.5);
call edge_laplacian(src = out_rgb, dst = edges, invert=true, kernel_size=5);
call denoise(src = edges, dst = edges, strength=10);

temp image final;
call copy(src = edges, dst = final);
call min(src = edges, dst = final);
#call threshold(src = final, dst=final, type = THRESH_BINARY, threshold = 175, maxval = 255);


# apply repeated bilateral blurs to a half size image.This preserves
# edges whilst maintain edge boudaries.
temp image half_sized;
call rescale(
	src = clean,
	scale_x = 0.5,
	scale_y = 0.5,
	dst = half_sized);

const int bilateral_filter_size = 9;
const int bilateral_sigma_color = 20;
const int bilateral_sigma_space = 7;
const int num_loops = 17;
temp image filtered;
call bilateral(
	src = half_sized,
	dst = filtered,
	filter_size = bilateral_filter_size,
	sigma_color = bilateral_sigma_color,
	sigma_space = bilateral_sigma_space,
	iterations = num_loops);

temp image cartoon;
call resize(
	src = filtered,
	width = __width__,
	height = __height__,
	dst = cartoon);

call copy(src = final, dst = __dst__);

temp image blend;
call convert_image(src = final, dst = blend, format = FORMAT_RGB);
call blend_darken(src1 = cartoon, src2 = blend, dst = __dst__);

call save_image(src = cartoon, path = "!!test_final.jpg");
