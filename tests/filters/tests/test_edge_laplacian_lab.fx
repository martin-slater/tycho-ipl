temp image clean, clean_rgb, edge3, edge5, edge7, edge9;

call denoise(src = __src__, dst = clean_rgb, strength = 4);
call auto_level(src = clean_rgb, dst = clean_rgb);
call convert_image(src = clean_rgb, dst = clean, format = FORMAT_LAB);


call edge_laplacian(src = clean, dst = edge3, kernel_size = 3, invert = true);
call edge_laplacian(src = clean, dst = edge5, kernel_size = 5, invert = true);
call edge_laplacian(src = clean, dst = edge7, kernel_size = 7, invert = true);
call edge_laplacian(src = clean, dst = edge9, kernel_size = 9, invert = true);

call experiment_add_image(src = edge3, name = "kernel_size = 3");
call experiment_add_image(src = edge5, name = "kernel_size = 5");
call experiment_add_image(src = edge7, name = "kernel_size = 7");
call experiment_add_image(src = edge9, name = "kernel_size = 9");
