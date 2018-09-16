temp image component_stretch, opencv;
temp image histogram_clip0_5, histogram_clip1, histogram_clip2, histogram_clip4, histogram_clip8;

call clamp_image_size(src = __src__, dst = __src__, size = 800);
call auto_level_rgb_stretch(src = __src__, dst = component_stretch);
call auto_level_opencv(src = __src__, dst = opencv);
call auto_level_histogram_clip(src = __src__, dst = histogram_clip0_5, clip_percent = 0.5);
call auto_level_histogram_clip(src = __src__, dst = histogram_clip1, clip_percent = 1);
call auto_level_histogram_clip(src = __src__, dst = histogram_clip2, clip_percent = 2);
call auto_level_histogram_clip(src = __src__, dst = histogram_clip4, clip_percent = 4);
call auto_level_histogram_clip(src = __src__, dst = histogram_clip8, clip_percent = 8);

call experiment_add_image(src = histogram_clip0_5, name = "histogram_clip0_5");
call experiment_add_image(src = histogram_clip1, name = "histogram_clip1");
call experiment_add_image(src = histogram_clip2, name = "histogram_clip2");
call experiment_add_image(src = histogram_clip4, name = "histogram_clip4");
call experiment_add_image(src = histogram_clip8, name = "histogram_clip8");
call experiment_add_image(src = component_stretch, name = "component_stretch");
call experiment_add_image(src = opencv, name = "opencv");


