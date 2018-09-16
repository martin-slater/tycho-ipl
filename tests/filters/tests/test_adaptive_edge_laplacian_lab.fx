temp image clean, clean_rgb, edge1, edge5, edge10, edge20, edge40;
temp image edge1a, edge5a, edge10a, edge20a, edge40a;


call denoise(src = __src__, dst = clean_rgb, strength = 4);
call auto_level(src = clean_rgb, dst = clean_rgb);
call convert_image(src = clean_rgb, dst = clean, format = FORMAT_LAB);O

call adaptive_edge_laplacian(src = clean, dst = edge1, edge_percent = 1, invert = true);
call adaptive_edge_laplacian(src = clean, dst = edge5, edge_percent = 5, invert = true);
call adaptive_edge_laplacian(src = clean, dst = edge10, edge_percent = 10, invert = true);
call adaptive_edge_laplacian(src = clean, dst = edge20, edge_percent = 20, invert = true);
call adaptive_edge_laplacian(src = clean, dst = edge40, edge_percent = 40, invert = true);

call adaptive_edge_laplacian(src = clean, dst = edge1a, edge_percent = 1, invert = true, adaptive_cutoff = true);
call adaptive_edge_laplacian(src = clean, dst = edge5a, edge_percent = 5, invert = true, adaptive_cutoff = true);
call adaptive_edge_laplacian(src = clean, dst = edge10a, edge_percent = 10, invert = true, adaptive_cutoff = true);
call adaptive_edge_laplacian(src = clean, dst = edge20a, edge_percent = 20, invert = true, adaptive_cutoff = true);
call adaptive_edge_laplacian(src = clean, dst = edge40a, edge_percent = 40, invert = true, adaptive_cutoff = true);

call experiment_add_image(src = edge1, name = "Laplacian - 1%");
call experiment_add_image(src = edge1a, name = "Laplacian - 1% AC");
call experiment_add_image(src = edge5, name = "Laplacian - 5%");
call experiment_add_image(src = edge5a, name = "Laplacian - 5% AC");
call experiment_add_image(src = edge10, name = "Laplacian - 10%");
call experiment_add_image(src = edge10a, name = "Laplacian - 10% AC");
call experiment_add_image(src = edge20, name = "Laplacian - 20%");
call experiment_add_image(src = edge20a, name = "Laplacian - 20% AC");
call experiment_add_image(src = edge40, name = "Laplacian - 40%");
call experiment_add_image(src = edge40a, name = "Laplacian - 40% AC");


