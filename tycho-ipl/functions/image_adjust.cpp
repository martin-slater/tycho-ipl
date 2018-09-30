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
#include "image_adjust.h"
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

	static const char Name[] = "image_adjust";
	static const char Desc[] = "Adjust the contrast and brightness of an image";
	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "brightness", "Brightness in range [0,255] to add to pixel.", value::make_integer(0)),
		param_desc(ObjectType::Float, "contrast", "Amount to scale existing pixel by.", value::make_float(0))
	};

	//----------------------------------------------------------------------------

	image_adjust::image_adjust() :
		function(Group::Leveling, Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool image_adjust::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");

		float bias = inputs.get_float("contrast");
		int gain = inputs.get_integer("brightness");
		image* dst = src->clone();
		execute(src, dst, bias, gain);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void image_adjust::execute(image* in_src, image* in_dst, 
		float gain, int bias)
	{
		IMAGE_PROC_ASSERT(in_src);
		IMAGE_PROC_ASSERT(in_dst);

		cv::Mat& src = *in_src->get_opencv();
		cv::Mat& dst = *in_dst->get_opencv();

		for (int y = 0; y < src.rows; y++)
		{
			for (int x = 0; x < src.cols; x++)
			{
				for (int c = 0; c < 3; c++)
				{
					dst.at<cv::Vec3b>(y, x)[c] =
						cv::saturate_cast<uchar>(gain*(src.at<cv::Vec3b>(y, x)[c]) + bias);
				}
			}
		}
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
