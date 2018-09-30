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
#include "edge_sobel.h"
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
	static const char Name[] = "edge_sobel";
	static const char Desc[] = "Find edges using Sobel filter";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),

		param_desc(ObjectType::Integer,
		"kernel_size",
		"Higher increases sensitivity. Must be one of 1, 3, 5 or 7.",
		value::make_integer(3),
		std::bind(validation::odd_only_integer, std::placeholders::_1, 7)),

		param_desc(ObjectType::Float,
		"scale",
		"Scale factor for the computed derivative values",
		value::make_float(1)),

		param_desc(ObjectType::Float,
		"delta",
		"Delta value that is added to the results prior to storing them.",
		value::make_float(0)),

		param_desc(ObjectType::Boolean,
		"invert",
		"Invert the results",
		value::make_boolean(false))
	};

	//----------------------------------------------------------------------------

	edge_sobel::edge_sobel() :
		function(Group::EdgeDetection, Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------

	bool edge_sobel::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int kernel_size = inputs.get_integer("kernel_size");
		float scale = inputs.get_float("scale");
		float delta = inputs.get_float("delta");
		bool invert = inputs.get_boolean("invert");
		image* dst = src->clone();
		execute(src, dst, kernel_size, scale, delta, invert);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void edge_sobel::execute(image* in_src, image* in_dst, 
		int kernel_size, float scale, float delta, bool invert)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		std::vector<cv::Mat> dst_channels;
		std::vector<cv::Mat> src_channels;

		cv::split(src, src_channels);
		for (int c = 0; c < src.channels(); ++c)
		{
			Mat dx, dy, abs_dx, abs_dy, res;
			Sobel(src_channels[c], dx, CV_16S, 1, 0, kernel_size, scale, delta);
			Sobel(src_channels[c], dy, CV_16S, 0, 1, kernel_size, scale, delta);
			convertScaleAbs(dx, abs_dx);
			convertScaleAbs(dy, abs_dy);
			addWeighted(abs_dx, 0.5, abs_dy, 0.5, 0, res);
			dst_channels.push_back(res);
		}
		cv::merge(dst_channels, dst);
		in_dst->convert_to(in_dst, image::Format::Grey);

		if (invert)
			cv::bitwise_not(*in_dst->get_opencv(), *in_dst->get_opencv());
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
