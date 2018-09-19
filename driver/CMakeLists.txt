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
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
project(ty_ipl_driver)


if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(${PROJECT_NAME} main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${OPENCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} tycho_ipl)
target_link_libraries(${PROJECT_NAME} libimagequant)

if(HAVE_IMAGE_MAGICK)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/../3rdparty/Magick/dll/"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()


# OS X xcode clang does not support std::filesystem yet so we need to use
# homebrew installed version and as this library is still not official we
# need to link the experimental lib.
if(APPLE)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_link_libraries(${PROJECT_NAME} /usr/local/opt/llvm/lib/libc++experimental.a)
    endif()
endif()

