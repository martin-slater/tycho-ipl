temp image clean, out_rgb, out_lab, out_hsv, out_hls;

call denoise(src = __src__, dst = clean, strength = 4);
#call copy(src = __src__, dst = clean);
call remove_intensity(src = clean, dst = out_rgb);
call experiment_add_image(src = out_rgb, name = "remove_intensity_rgb");

call remove_intensity(src = clean, dst = out_lab);
call remove_intensity(src = clean, dst = out_hsv);
call remove_intensity(src = clean, dst = out_hls);

call experiment_add_image(src = out_lab, name = "remove_intensity_lab");
call experiment_add_image(src = out_hsv, name = "remove_intensity_hsv");
call experiment_add_image(src = out_hls, name = "remove_intensity_hls");

temp image edges;
call edge_laplacian(src = clean, kernel_size = 5, dst = edges, invert = true);
call experiment_add_image(src = edges, name = "laplacian_edge_clean");

temp image edges_rgb;
call edge_laplacian(src = out_rgb, kernel_size = 5, dst = edges_rgb, invert = true);
call experiment_add_image(src = edges_rgb, name = "laplacian_edge_rgb");

temp image edges_hsv;
call edge_laplacian(src = out_hsv, kernel_size = 5, dst = edges_hsv, invert = true);
call experiment_add_image(src = edges_hsv, name = "laplacian_edge_hsv");

temp image edges_hls;
call edge_laplacian(src = out_hls, kernel_size = 5, dst = edges_hls, invert = true);
call experiment_add_image(src = edges_hls, name = "laplacian_edge_hls");

temp image edges_lab;
call edge_laplacian(src = out_lab, kernel_size = 5, dst = edges_lab, invert = true);
call experiment_add_image(src = edges_lab, name = "laplacian_edge_lab");


const float threshold_low = 25;
const float threshold_high = 50;

temp image canny_edges;
call edge_canny(src = clean, threshold_high=threshold_high, threshold_low=threshold_low, dst = canny_edges, invert = true);
call experiment_add_image(src = canny_edges, name = "canny_edge_clean");

temp image canny_edges_rgb;
call edge_canny(src = out_rgb, threshold_high = threshold_high, threshold_low = threshold_low, dst = canny_edges_rgb, invert = true);
call experiment_add_image(src = canny_edges_rgb, name = "canny_edge_rgb");

temp image canny_edges_hsv;
call edge_canny(src = out_hsv, threshold_high = threshold_high, threshold_low = threshold_low, dst = canny_edges_hsv, invert = true);
call experiment_add_image(src = canny_edges_hsv, name = "canny_edge_hsv");

temp image canny_edges_hls;
call edge_canny(src = out_hls, threshold_high = threshold_high, threshold_low = threshold_low, dst = canny_edges_hls, invert = true);
call experiment_add_image(src = canny_edges_hls, name = "canny_edge_hls");

temp image canny_edges_lab;
call edge_canny(src = out_lab, threshold_high = threshold_high, threshold_low = threshold_low, dst = canny_edges_lab, invert = true);
call experiment_add_image(src = canny_edges_lab, name = "canny_edge_lab");

