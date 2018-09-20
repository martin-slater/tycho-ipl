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
#include "edge_canny.h"
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
	static const char Name[] = "edge_canny";
	static const char Desc[] = "Canny edge detection filter";
	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(
			ObjectType::Integer, 
			"kernel_size", 
			"Kernel size for the sobel operator", 
			value::make_integer(3),
			std::bind(validation::odd_only_integer, std::placeholders::_1, 9)),

		param_desc(ObjectType::Float, "threshold_low", "First threshold for the hysteresis procedure", value::make_float(25.0f)),
		param_desc(ObjectType::Float, "threshold_high", "Second threshold for the hysteresis procedure", value::make_float(50.0f)),
		param_desc(ObjectType::Boolean, "invert", "Invert the results", value::make_boolean(false))
	};

	//----------------------------------------------------------------------------

	edge_canny::edge_canny() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------

	bool edge_canny::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int ksize = inputs.get_integer("kernel_size");
		float tlow = inputs.get_float("threshold_low");
		float thigh = inputs.get_float("threshold_high");
		bool invert = inputs.get_boolean("invert");

		image* dst = src->clone();
		execute(src, dst, tlow, thigh, ksize, invert);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void edge_canny::execute(image* in_src, image* in_dst, 
		float threshold_low, float threashold_high, int ksize, bool invert)
	{
		using namespace cv;

		IMAGE_PROC_ASSERT((ksize & 1) == 1);

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		std::vector<cv::Mat> channels;
		cv::split(src, channels);
		for (int c = 0; c < src.channels(); ++c)
		{
			Canny(channels[c], channels[c], threshold_low, threashold_high, ksize);
		}
		cv::merge(channels, dst);
		in_dst->convert_to(in_dst, image::Format::Grey);

		if (invert)
			cv::bitwise_not(*in_dst->get_opencv(), *in_dst->get_opencv());
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
