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
#include "remove_intensity.h"
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
	//----------------------------------------------------------------------------


	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(
			ObjectType::Integer, 
			"black-cutoff", 
			"Value under which the sum (A + G + B) will be clamped to 0", 
			value::make_integer(64))
	};

	//----------------------------------------------------------------------------

	remove_intensity::remove_intensity() : function(
		"remove_intensity",
		"Removes the intensity from the image. If the image is in a colour space that " \
		"contains an intensity channel (Lab, HLS, HSV) then this will be cleared. If the " \
		"image is in RGB space then each texel is normalized as sum = R + G + B, " \
		"R' = R / sum, G' = G / sum, B' = B / sum. If sum is less than black-cutoff " \
		"then it is clamped to zero.",
		Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------

	bool remove_intensity::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		int cutoff = inputs.get_integer("black-cutoff");
		image *src = inputs.get_image("src");
		image* dst = src->clone();
		execute(src, dst, cutoff);
		outputs.set_image("dst", dst);
		return true;

	}

	//----------------------------------------------------------------------------

	void remove_intensity::execute(image* in_src, image* in_dst, int black_cutoff)
	{
		cv::Mat& src_mat = *in_src->get_opencv();
		cv::Mat& dst_mat = *in_dst->get_opencv();
			
		if (in_src->format_is(image::Format::RGB))
		{
			for (int y = 0; y < in_src->get_height(); y++)
			{
				for (int x = 0; x < in_src->get_width(); x++)
				{
					cv::Vec3b pixel = src_mat.at<cv::Vec3b>(y, x);
					double sum = pixel[0] + pixel[1] + pixel[2];
					for (int c = 0; c < 3; ++c)
					{
						double val = pixel[c];
						if (sum > black_cutoff)
						{
							val /= sum;
							val *= 255;
						}
						else
							val = 0;

						assert(val < 256);
						dst_mat.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(val);
					}
				}
			}
		}
		else
		{
			int channel = get_intensity_channel(in_src->get_format());
			IMAGE_PROC_ASSERT(channel != -1);
			std::vector<cv::Mat> channels;
			cv::split(src_mat, channels);
			channels[channel] = cv::Scalar(0);
			cv::merge(channels, dst_mat);
		}
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
