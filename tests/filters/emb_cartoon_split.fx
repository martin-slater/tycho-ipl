input int num_loops = 17;
input int filter_size = 9;


# apply repeated bilateral blurs to a half size image. This preserves
# edges whilst maintain edge boundaries.
temp image half_sized;
#call rescale(src = __src__, scale_x = 0.5, scale_y = 0.5, dst = half_sized);
call clamp_image_size(src = __src__, size = 100, dst = half_sized);

const int bilateral_sigma_color = 20;
const int bilateral_sigma_space = 7;

temp image filtered;
call bilateral(
    src=half_sized,
    dst=filtered, 
	filter_size = filter_size,
    sigma_color=bilateral_sigma_color, 
    sigma_space=bilateral_sigma_space,
	iterations=num_loops);

temp image cartoon;
call resize(src=filtered, width = __width__, height = __height__, dst=cartoon);
call gaussian_blur(src = cartoon, dst = cartoon, kernel_size = 5);
call gaussian_blur(src = cartoon, dst = cartoon, kernel_size = 5);


# find edges in greyscale image
const int edge_kernel_size = 3;
temp image edges;
temp image grey;

call greyscale(src = __src__, dst = grey);
call gaussian_blur(src = grey, dst = grey, kernel_size = 3);
call gaussian_blur(src = grey, dst = grey, kernel_size = 3);
call edge_canny(src = grey, kernel_size = edge_kernel_size, threshold_low = 60, threshold_high = 80, dst = edges);
call threshold(src = edges, dst = edges, type = THRESH_BINARY_INV, threshold = 254, maxval = 255);
call erode(src = edges, dst = edges, size=3);

call experiment_add_image(src = cartoon, name = "Low Freq");
call experiment_add_image(src = edges, name = "High Freq");

call copy(src = cartoon, mask = edges, dst = __dst__);


