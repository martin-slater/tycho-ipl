//----------------------------------------------------------------------------
// Image Processing Library
//
// MIT License
// Copyright (c) 2018 Martin A Slater (mslater@hellinc.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "arithmetic.h"
#include "common.h"
#include "../key_value.h"
#include "../context.h"
#include "../image.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{

	IMAG_PROC_DEFINE_BINARY_FUNCTION(
		"add",
		"Component wise add two images", add)
	{
		cv::add(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_SCALED_BINARY_FUNCTION(
		"scaled_add",
		"Compute src1 * scale + src2", scaled_add)
	{
		cv::scaleAdd(*src1->get_opencv(), scale, *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_BINARY_FUNCTION(
		"subtract",
		"Component wise subtract two images", subtract)
	{
		cv::subtract(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_BINARY_FUNCTION(
		"multiply",
		"Component wise multiply two images", multiply)
	{
		cv::multiply(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_SCALED_BINARY_FUNCTION(
		"scaled_multiple",
		"Component wise multiply two images", scaled_multiply)
	{
		cv::multiply(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv(), scale);
	}

	IMAG_PROC_DEFINE_BINARY_FUNCTION(
		"divide",
		"Component wise divide two images", divide)
	{
		cv::multiply(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_BINARY_FUNCTION(
		"bitwise_and",
		"Component wise bitwise and two images", bitwise_and)
	{
		cv::bitwise_and(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_BINARY_FUNCTION(
		"bitwise_or",
		"Component wise bitwise or two images", bitwise_or)
	{
		cv::bitwise_or(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_BINARY_FUNCTION(
		"bitwise_xor",
		"Component wise and two images", bitwise_xor)
	{
		cv::bitwise_xor(*src1->get_opencv(), *src2->get_opencv(), *dst->get_opencv());
	}

	IMAG_PROC_DEFINE_UNARY_FUNCTION(
		"max",
		"max", max)
	{
		std::vector<cv::Mat> channels;
		cv::split(*src->get_opencv(), channels);
		cv::Mat dst_mat(src->get_height(), src->get_width(), CV_8UC1);
		
		for (int y = 0; y < src->get_height(); ++y)
		{
			for (int x = 0; x < src->get_width(); ++x)
			{
				uchar max = 0;
				for (auto & channel : channels)
				{
					uchar val = channel.at<uchar>(y, x);
					max = std::max(max, val);
				}

				dst_mat.at<uchar>(y, x) = max;
			}
		}
		dst->set_mat(dst_mat, image::Format::Grey);
	}

	IMAG_PROC_DEFINE_UNARY_FUNCTION(
		"min",
		"min", min)
	{
		std::vector<cv::Mat> channels;
		cv::split(*src->get_opencv(), channels);
		cv::Mat dst_mat(src->get_height(), src->get_width(), CV_8UC1);

		for (int y = 0; y < src->get_height(); ++y)
		{
			for (int x = 0; x < src->get_width(); ++x)
			{
				uchar min = 255;
				for (auto & channel : channels)
				{
					uchar val = channel.at<uchar>(y, x);
					min = std::min(min, val);
				}

				dst_mat.at<uchar>(y, x) = min;
			}
		}
		dst->set_mat(dst_mat, image::Format::Grey);
	}

} // end namespace
} // end namespace
} // end namespace
