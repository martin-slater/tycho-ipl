input int num_loops = 17;
input int filter_size = 9;

call color_reduce_kmeans(
    src=__src__,
    dst=__src__
);

temp image grey;
call greyscale(
    src=__src__,
    dst=grey);

# apply repeated bilateral blurs to a half size image. This preserves
# edges whilst maintain edge boudaries.
temp image half_sized;
call rescale(
	src = __src__,
    scale_x=0.5,
    scale_y=0.5,
    dst=half_sized);

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
call resize(
    src=filtered, 
    width = __width__,
    height = __height__,
    dst=cartoon);


# find edges in greyscale image
const int edge_kernel_size = 5;
temp image edges;

call denoise(src=grey, dst=grey, strength=11);
call edge_laplacian(src=grey, kernel_size=edge_kernel_size, dst=edges);
call threshold(src = edges, dst = edges, type = THRESH_BINARY_INV, threshold = 254, maxval = 255);
temp image t2;
call copy(src = cartoon, mask = edges, dst = t2);
call denoise(src=t2, dst=__dst__, strength=11);

