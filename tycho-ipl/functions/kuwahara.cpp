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
#include "kuwahara.h"
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
		value::make_integer(5),
		std::bind(validation::odd_only_integer, std::placeholders::_1, 255))
	};

	//----------------------------------------------------------------------------

	kuwahara::kuwahara() : function(
		Group::Artistic,
		"kuwahara",
		"Apply the kuwahara operator. This smooths textured regions whilst maintaining edges.",
		Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------

	bool kuwahara::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		int radius = inputs.get_integer("kernel_size");
		image *src = inputs.get_image("src");
		image* dst = src->clone();
		execute(src, dst, radius);
		outputs.set_image("dst", dst);
		return true;

	}

	//----------------------------------------------------------------------------

	void kuwahara::execute(image* in_src, image* in_dst, int kernel_size)
	{
		cv::Mat& src_mat = *in_src->get_opencv();
		cv::Mat& dst_mat = *in_dst->get_opencv();

		image intensity(image::Format::Grey, in_src->get_width(), in_src->get_height());
		in_src->convert_to(&intensity, image::Format::Grey);
		cv::Mat& src_int = *intensity.get_opencv();
		

		const int kwidth = (kernel_size - 1) / 2;
		const int num_bvals = (kwidth + 1) * (kwidth + 1);
		auto* bvals = (int*)alloca(sizeof(int) * num_bvals);

		cv::Vec2i quads[] = {
			cv::Vec2i(-kwidth, -kwidth),
			cv::Vec2i(0, -kwidth),
			cv::Vec2i(-kwidth, 0),
			cv::Vec2i(0, 0)
		};

		if (in_src->format_is(image::Format::RGB))
		{
			const int image_width = in_src->get_width();
			const int image_height = in_src->get_height();
			for (int y = 0; y < image_height; y++)
			{
				for (int x = 0; x < image_width; x++)
				{
					// calculate the average colour and standard deviation
					// of the intensity
					cv::Vec3b avg_clr(0,0,0);
					float lowest_sd = std::numeric_limits<float>::max();

					// kernel is split into four quadrants
					for (auto & quad : quads)
					{
						const int sqx = x + quad[0];
						const int sqy = y + quad[1];

						const int eqx = sqx + kwidth + 1;
						const int eqy = sqy + kwidth + 1;

						int cur_bval = 0;
						float bval_sum = 0;

						cv::Vec3f pixel_sum(0, 0, 0);
						for (int qy = sqy; qy < eqy; ++qy)
						{
							for (int qx = sqx; qx < eqx; ++qx)
							{
								int cx = std::min<int>(image_width - 1, std::max<int>(0, qx));
								int cy = std::min<int>(image_height - 1, std::max<int>(0, qy));

								cv::Vec3b pixel = src_mat.at<cv::Vec3b>(cy, cx);									
								int brightness = src_int.at<uchar>(cy, cx);
								bvals[cur_bval++] = brightness;
								bval_sum += brightness;
								pixel_sum += pixel;
							}
						}

						// calculate standard deviation
						float mean = bval_sum / num_bvals;
						float std_dev = 0;
						for (int v = 0; v < num_bvals; ++v)
						{
							float diff = bvals[v] - mean;
							float dev = diff * diff;
							std_dev += dev;
						}

						// we only care about the lowest standard deviation and not its
						// actual value so we do not need to take the sqrt or scale it
						//std_dev /= (num_bvals - 1);
						//std_dev = std::sqrt(std_dev);

						if (std_dev < lowest_sd)
						{
							lowest_sd = std_dev;
							avg_clr = pixel_sum / num_bvals;
						}
					}

					dst_mat.at<cv::Vec3b>(y, x) = avg_clr;
				}
			}
		}
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
