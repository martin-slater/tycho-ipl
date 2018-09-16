temp image grey, 
           grey2hsv, 
		   grey2hsl, 
		   grey2rgb;

call convert_image(src = __src__, dst = grey, format = FORMAT_GREY);

# Grey -> Lab -> RGB
temp image lab2rgb;
temp image grey2lab;
call convert_image(src = grey, dst = grey2lab, format = FORMAT_LAB);
call convert_image(src = grey2lab, dst = lab2rgb, format = FORMAT_RGB);
call save_image(src = grey2lab, path = "grey_lab.jpg");
call save_image(src = lab2rgb, path = "grey_lab_rgb.jpg");



call convert_image(src = grey, dst = grey2hsv, format = FORMAT_HSV);
call convert_image(src = grey, dst = grey2hsl, format = FORMAT_HLS);
call convert_image(src = grey, dst = grey2rgb, format = FORMAT_RGB);

temp image grey2rgb2lab;
call convert_image(src = grey2rgb, dst = grey2rgb2lab, format = FORMAT_LAB);
call save_image(src = grey2rgb2lab, path = "rgb_grey_rgb_lab.jpg");

temp image grey2rgb2hsv;
call convert_image(src = grey2rgb, dst = grey2rgb2hsv, format = FORMAT_HSV);
call save_image(src = grey2rgb2hsv, path = "rgb_grey_rgb_hsv.jpg");

temp image grey2rgb2hls;
call convert_image(src = grey2rgb, dst = grey2rgb2hls, format = FORMAT_HLS);
call save_image(src = grey2rgb2hls, path = "rgb_grey_rgb_hls.jpg");


call save_image(src = grey, path = "grey.jpg");
call save_image(src = grey2hsv, path = "grey_hsv.jpg");
call save_image(src = grey2hsl, path = "grey_hsl.jpg");
call save_image(src = grey2rgb, path = "grey_rgb.jpg");

temp image hsv2rgb;
temp image hsl2rgb;
call convert_image(src = grey2hsv, dst = hsv2rgb, format = FORMAT_RGB);
call convert_image(src = grey2hsl, dst = hsl2rgb, format = FORMAT_RGB);
call save_image(src = hsv2rgb, path = "grey_hsv_rgb.jpg");
call save_image(src = hsl2rgb, path = "grey_hsl_rgb.jpg");


temp image lab;
temp image lab_rgb;
temp image lab_grey;
call convert_image(src = __src__, dst = lab, format = FORMAT_LAB);
call convert_image(src = lab, dst = lab_rgb, format = FORMAT_RGB);
call convert_image(src = lab, dst = lab_grey, format = FORMAT_GREY);
call save_image(src = lab, path = "lab.jpg");
call save_image(src = lab_rgb, path = "lab_rgb.jpg");
call save_image(src = lab_grey, path = "lab_grey.jpg");

temp image hsl;
temp image hsl_rgb;
temp image hsl_grey;
call convert_image(src = __src__, dst = hsl, format = FORMAT_HLS);
call convert_image(src = hsl , dst = hsl_rgb, format = FORMAT_RGB);
call convert_image(src = hsl, dst = hsl_grey, format = FORMAT_GREY);
call save_image(src = hsl, path = "hsl.jpg");
call save_image(src = hsl_rgb, path = "hsl_rgb.jpg");
call save_image(src = hsl_grey, path = "hsl_grey.jpg");

temp image hsv;
temp image hsv_rgb;
temp image hsv_grey;
call convert_image(src = __src__, dst = hsv, format = FORMAT_HSV);
call convert_image(src = hsv, dst = hsv_rgb, format = FORMAT_RGB);
call convert_image(src = hsv, dst = hsv_grey, format = FORMAT_GREY);
call save_image(src = hsv, path = "hsv.jpg");
call save_image(src = hsv_rgb, path = "hsv_rgb.jpg");
call save_image(src = hsv_grey, path = "hsv_grey.jpg");