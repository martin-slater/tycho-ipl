temp image original, denoised, denoised_pal, simplified, simplified_pal;

call visualize_palette(src = __src__, dst = original);
call experiment_add_image(src = original, name = "Original");

call simplify_colors(src = __src__, dst = simplified);
call visualize_palette(src = simplified, dst = simplified_pal);
call experiment_add_image(src = simplified_pal, name = "Simplified");

#call denoise(src = __src__, dst = denoised, strength = 4);
#call visualize_palette(src = denoised, dst = denoised_pal);
#call experiment_add_image(src = denoised_pal, name = "Denoised");
