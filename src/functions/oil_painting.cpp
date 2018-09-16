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
#include "oil_painting.h"
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
		"kernel_size",
		"Radius around each pixel to examine",
		value::make_integer(5)),
		param_desc(
		ObjectType::Integer,
		"levels",
		"Number of intensity levels",
		value::make_integer(20)),
	};

	//----------------------------------------------------------------------------

	oil_painting::oil_painting() : function(
		"oil_painting",
		"Transform the image to have an oil painted appearance",
		Inputs, function::DefaultOutputs(), declaration_list())
	{

	}

	//----------------------------------------------------------------------------

	bool oil_painting::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		int radius = inputs.get_integer("kernel_size");
		int levels = inputs.get_integer("levels");
		image *src = inputs.get_image("src");
		image* dst = src->clone();
		execute(src, dst, radius, levels);
		outputs.set_image("dst", dst);
		return true;

	}

	//----------------------------------------------------------------------------

	void oil_painting::execute(image* in_src, image* in_dst, int kernel_size, int num_levels)
	{
		// http://supercomputingblog.com/graphics/oil-painting-algorithm/

		cv::Mat& src_mat = *in_src->get_opencv();
		cv::Mat& dst_mat = *in_dst->get_opencv();
		
		const int kwidth = (kernel_size - 1) / 2;
		const float kwidth_sq = (kernel_size / 2.0f) * (kernel_size / 2.0f);
		std::vector<int> counts(num_levels);
		std::vector<cv::Vec3f> averages(num_levels);

		if (in_src->format_is(image::Format::RGB))
		{
			for (int y = 0; y < in_src->get_height(); y++)
			{
				for (int x = 0; x < in_src->get_width(); x++)
				{					
					// iterate over all the pixels within the given radius (square)
					int c = std::max<int>(y - kwidth, 0);
					int r = std::max<int>(x - kwidth, 0);
					int cc = std::min<int>(in_src->get_height()-1, y + kwidth);
					int rr = std::min<int>(in_src->get_width()-1, x + kwidth);
					
 					memset(&counts[0], 0L, sizeof(int) * num_levels);
 					memset(&averages[0], 0L, sizeof(cv::Vec3f) * num_levels);


					int max_index = -1;
					int max_count = -1;
					for (; c <= cc; ++c)
					{
						for (int cr = r; cr <= rr; ++cr)
						{
							int dx = x - cr;
							int dy = y - c;
							int r2 = dx * dx + dy * dy;

							if (r2 < kwidth_sq)
							{
								cv::Vec3b pixel = src_mat.at<cv::Vec3b>(c, cr);
								int bucket = (int)(((float)((pixel[0] + pixel[1] + pixel[2]) / 3.0f) / 255.0f) * (num_levels - 1));

								IMAGE_PROC_ASSERT(bucket < num_levels);

								counts[bucket] += 1;
								averages[bucket] += pixel;
								if (counts[bucket] > max_count)
								{
									max_count = counts[bucket];
									max_index = bucket;
								}
							}
						}
					}
					auto avg = averages[max_index];
					dst_mat.at<cv::Vec3b>(y, x)[0] = cv::saturate_cast<uchar>(avg[0] / max_count);
					dst_mat.at<cv::Vec3b>(y, x)[1] = cv::saturate_cast<uchar>(avg[1] / max_count);
					dst_mat.at<cv::Vec3b>(y, x)[2] = cv::saturate_cast<uchar>(avg[2] / max_count);
				}
			}
		}
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
