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
project(tycho_ipl)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "FATAL: In-source builds are not allowed.You should create a separate directory for build files.")
endif()

# Configure OpenCV build. Disable as much as we can, we just want the lib
set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/externals/opencv)

option(BUILD_EXAMPLES OFF)
option(BUILD_TESTS OFF)
option(BUILD_PERF_TESTS OFF)
option(BUILD_opencv_apps OFF)
option(BUILD_opencv_python2 OFF)
option(BUILD_opencv_python3 OFF)
option(BUILD_DOCS OFF)
option(BUILD_JAVA OFF)
option(WITH_VFW OFF)
option(WITH_WEBP OFF)
option(WITH_DSHOW OFF)
option(WITH_MSMF OFF)
option(WITH_WIN32UI OFF)
option(WITH_QUICKTIME OFF)
option(WITH_QTKIT OFF)
option(WITH_FFMPEG OFF)
option(WITH_AVFOUNDATION OFF)
add_subdirectory(${OpenCV_DIR})

set(OPENCV_INCLUDE_DIRS
    ${OpenCV_DIR}/include
    ${OpenCV_DIR}/modules/core/include
    ${OpenCV_DIR}/modules/imgproc/include
    ${OpenCV_DIR}/modules/highgui/include
    ${OpenCV_DIR}/modules/imgcodecs/include
    ${OpenCV_DIR}/modules/videoio/include
    ${OpenCV_DIR}/modules/photo/include
    ${CMAKE_SOURCE_DIR}/build)

# Setup compiler warning level
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/externals/opencv/" CACHE PATH "OpenCV include directory")

add_subdirectory(3rdparty/libimagequant)
add_subdirectory(tycho-ipl)
add_subdirectory(driver)

