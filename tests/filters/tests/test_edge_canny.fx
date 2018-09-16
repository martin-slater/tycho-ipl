input float high = 50;
input float low = 25; 

temp image clean, edge3, edge5, edge7;

call denoise(src = __src__, dst = clean, strength = 4);
call auto_level(src = clean, dst = clean);

call edge_canny(src = clean, dst = edge3, threshold_low = low, threshold_high = high, kernel_size = 3, invert = true);
call edge_canny(src = clean, dst = edge5, threshold_low = low, threshold_high = high, kernel_size = 5, invert = true);
call edge_canny(src = clean, dst = edge7, threshold_low = low, threshold_high = high, kernel_size = 7, invert = true);


call experiment_add_image(src = edge3, name = "kernel_size = 3");
call experiment_add_image(src = edge5, name = "kernel_size = 5");
call experiment_add_image(src = edge7, name = "kernel_size = 7");
