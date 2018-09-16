temp image rgb, yiq0, yiq_neg4, yiq_pos4, yiq_neg8, yiq_neg16, yiq_pos8, yiq_pos16;

call clamp_image_size(src = __src__, dst = __src__, size = 800);

call sepia_rgb(src = __src__, dst = rgb);
call sepia_yiq(src = __src__, dst = yiq0,      offset = 0);
call sepia_yiq(src = __src__, dst = yiq_neg4,  offset = -4);
call sepia_yiq(src = __src__, dst = yiq_neg8,  offset = -8);
call sepia_yiq(src = __src__, dst = yiq_neg16, offset = -16);
call sepia_yiq(src = __src__, dst = yiq_pos4,  offset = 4);
call sepia_yiq(src = __src__, dst = yiq_pos8,  offset = 8);
call sepia_yiq(src = __src__, dst = yiq_pos16, offset = 16);


call experiment_add_image(src = rgb,       name = "sepia_rgb");
call experiment_add_image(src = yiq_neg16, name = "sepia_yiq_neg16");
call experiment_add_image(src = yiq_neg8, name = "sepia_yiq_neg8");
call experiment_add_image(src = yiq_neg4, name = "sepia_yiq_neg4");
call experiment_add_image(src = yiq0, name = "sepia_yiq0");
call experiment_add_image(src = yiq_pos4, name = "sepia_yiq_pos4");
call experiment_add_image(src = yiq_pos8,  name = "sepia_yiq_pos8");
call experiment_add_image(src = yiq_pos16, name = "sepia_yiq_pos16");


