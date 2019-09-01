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
#include "image_correct_gray_world.h"
#include "common.h"
#include "../image.h"
#include "../context.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{

static const char Name[] = "gray_world_adjust";
static const char Desc[] = "Adjust the image using the Gray World method";
static const std::vector<param_desc> Inputs = {
	function::DefaultInput(),
};

//----------------------------------------------------------------------------

image_correct_gray_world::image_correct_gray_world() : function(Group::Leveling, Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
{
}

//----------------------------------------------------------------------------

bool image_correct_gray_world::dispatch(context * /*ctx*/, const kv_dict &inputs, kv_dict &outputs)
{
	image *src = inputs.get_image("src");

	image *dst = src->clone();
	execute(src, dst);
	outputs.set_image("dst", dst);
	return true;
}

//----------------------------------------------------------------------------

void image_correct_gray_world::execute(image *in_src, image *in_dst)
{
	IMAGE_PROC_ASSERT(in_src);
	IMAGE_PROC_ASSERT(in_dst);

	const int BLUE = 0;
	const int GREEN = 1;
	const int RED = 2;

	cv::Mat &src = *in_src->get_opencv();
	cv::Mat &dst = *in_dst->get_opencv();

	// calculate average pixel intensities for each channel
	double sums[3] = {0, 0, 0};
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				sums[c] += src.at<cv::Vec3b>(y, x)[c];
			}
		}
	}

	const double numPixels = ((double)src.rows * src.cols);
	const double avgR = sums[RED] / numPixels;
	const double avgG = sums[GREEN] / numPixels;
	const double avgB = sums[BLUE] / numPixels;
	const double correctionR = avgG / avgR;
	const double correctionB = avgG / avgB;

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			const auto& srcPixel = src.at<cv::Vec3b>(y, x);
			auto& dstPixel = dst.at<cv::Vec3b>(y, x);
			dstPixel[RED] = cv::saturate_cast<uchar>(correctionR * (double)srcPixel[RED]);
			dstPixel[BLUE] = cv::saturate_cast<uchar>(correctionB * (double)srcPixel[BLUE]);
			dstPixel[GREEN] = srcPixel[GREEN];
		}
	}
}

//----------------------------------------------------------------------------

} // namespace functions
} // namespace image_processing
} // namespace tycho
