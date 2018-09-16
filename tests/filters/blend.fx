const int num_loops = 17;

# auto adjust brightness and contrast before any further processing
temp image adjusted;
call auto_level(
    src= __src__, 
	dst=adjusted,
    clip_percent=1.0);

call edge_laplacian(src=grey, kernel_size=edge_kernel_size, dst=edges);
call threshold(src = edges, dst = edges, type = THRESH_BINARY_INV, threshold = 254, maxval = 255);
call copy(src = cartoon, mask = edges, dst = __dst__);	

