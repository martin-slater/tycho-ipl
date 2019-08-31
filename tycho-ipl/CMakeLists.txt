#----------------------------------------------------------------------------
# Image Processing Library
#
# MIT License
# Copyright (c) 2018 Martin A Slater (mslater@hellinc.net)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  

project(tycho_ipl)

set( BASE_SRCS
    contact_sheet.cpp
    contact_sheet.h
    context.cpp
    context.h
    exception.cpp
    exception.h
    forward_decls.h
    function.cpp
    function.h
    image_magick_utils.cpp
    image_magick_utils.h
    image_processing_abi.h
    image.cpp
    image.h
    key_value.cpp
    key_value.h
    program.cpp
    program.h
    result_set.cpp
    result_set.h
    utils.cpp
    utils.h
)

source_group( "" FILES ${BASE_SRCS} )

set( RUNTIME_SRCS
    runtime/experiment_runner.cpp
    runtime/experiment_runner.h
    runtime/output_interface.cpp
    runtime/output_interface.h
    runtime/runner.cpp
    runtime/runner.h
    runtime/session_options.cpp
    runtime/session_options.h
    runtime/simple_runner.cpp
    runtime/simple_runner.h
)
source_group( "runtime" FILES ${RUNTIME_SRCS} )

set( FUNCTION_SRCS
    functions/adaptive_edge_laplacian.cpp
    functions/adaptive_edge_laplacian.h
    functions/arithmetic.cpp
    functions/arithmetic.h
    functions/auto_level_component_stretch.cpp
    functions/auto_level_component_stretch.h
    functions/auto_level_histogram_clip.cpp
    functions/auto_level_histogram_clip.h
    functions/auto_level_open_cv.cpp
    functions/auto_level_open_cv.h
    functions/bilateral.cpp
    functions/bilateral.h
    functions/blend_functions.cpp
    functions/blend_functions.h
    functions/color_reduce_im_mean_shift.cpp
    functions/color_reduce_im_mean_shift.h
    functions/color_reduce_im_quantize.cpp
    functions/color_reduce_im_quantize.h
    functions/color_reduce_kmeans_cluster.cpp
    functions/color_reduce_kmeans_cluster.h
    functions/color_reduce_lib_image_quant.cpp
    functions/color_reduce_lib_image_quant.h
    functions/color_reduce_median_cut.cpp
    functions/color_reduce_median_cut.h
    functions/color_reduce_population.cpp
    functions/color_reduce_population.h
    functions/common.h
    functions/copy.cpp
    functions/copy.h
    functions/denoise.cpp
    functions/denoise.h
    functions/edge_canny.cpp
    functions/edge_canny.h
    functions/edge_laplacian.cpp
    functions/edge_laplacian.h
    functions/edge_sobel.cpp
    functions/edge_sobel.h
    functions/factory.cpp
    functions/factory.h
    functions/gamma_correct.cpp
    functions/gamma_correct.h
    functions/gaussian_blur.cpp
    functions/gaussian_blur.h
    functions/greyscale.cpp
    functions/greyscale.h
    functions/image_adjust.cpp
    functions/image_adjust.h
    functions/image_functions.cpp
    functions/image_functions.h
    functions/interface_functions.cpp
    functions/interface_functions.h
    functions/kuwahara.cpp
    functions/kuwahara.h
    functions/morphological.cpp
    functions/morphological.h
    functions/nary_functions.cpp
    functions/nary_functions.h
    functions/oil_painting.cpp
    functions/oil_painting.h
    functions/remove_intensity.cpp
    functions/remove_intensity.h
    functions/rescale.cpp
    functions/rescale.h
    functions/resize.cpp
    functions/resize.h
    functions/sepia_rgb.cpp
    functions/sepia_rgb.h
    functions/sepia_yiq.cpp
    functions/sepia_yiq.h
    functions/simplify_colors.cpp
    functions/simplify_colors.h
    functions/threshold.cpp
    functions/threshold.h
    functions/visualize_palette.cpp
    functions/visualize_palette.h
    functions/image_correct_gray_world.h
    functions/image_correct_gray_world.cpp
)
source_group("functions" FILES ${FUNCTION_SRCS})

set( ALL_SRCS
    ${BASE_SRCS}
    ${RUNTIME_SRCS}
    ${FUNCTION_SRCS}
)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(tycho_ipl STATIC ${ALL_SRCS})
include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${libimagequant_INCLUDE})
target_link_libraries(tycho_ipl
    opencv_core
    opencv_imgproc
    opencv_highgui
    opencv_photo
    libimagequant)

if(HAVE_IMAGE_MAGICK)
    include_directories(${IMAGE_MAGICK_INCLUDE_DIR})
    link_directories(${IMAGE_MAGICK_LIB_DIR})
    target_link_libraries(${PROJECT_NAME}
        ${IMAGE_MAGICK_LIBS}
    )
endif()
