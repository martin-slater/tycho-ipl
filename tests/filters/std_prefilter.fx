call clamp_image_size(src = __src__, dst = __src__, size = 1024, enlarge = true);
call auto_level(src = __src__, dst = __src__);
call denoise(src = __src__, dst = __dst__);
