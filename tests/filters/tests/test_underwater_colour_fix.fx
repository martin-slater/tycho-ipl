temp image component_stretch, opencv, gw, gwc;

call clamp_image_size(src = __src__, dst = __src__, size = 800);
call auto_level_rgb_stretch(src = __src__, dst = component_stretch);
call gray_world_adjust(src = __src__, dst = gw);
call gray_world_adjust(src = component_stretch, dst = gwc);

call experiment_add_image(src = component_stretch, name = "component_stretch");
call experiment_add_image(src = gw, name = "gw");
call experiment_add_image(src = gwc, name = "gwc");
