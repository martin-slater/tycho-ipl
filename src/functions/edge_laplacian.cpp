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
#include "edge_laplacian.h"
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
	static const char Name[] = "edge_laplacian";
	static const char Desc[] = "Find edges using laplacian filter";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),

		param_desc(ObjectType::Integer,
			"kernel_size", 
			"Higher increases sensitivity. Must be an odd value.", 
			value::make_integer(3),
			std::bind(validation::odd_only_integer, std::placeholders::_1, 9)),
		
		param_desc(ObjectType::Boolean, 
			"invert", 
			"Invert the results", 
			value::make_boolean(false))
	};

	//----------------------------------------------------------------------------

	edge_laplacian::edge_laplacian() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{
	}

	//----------------------------------------------------------------------------

	bool edge_laplacian::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int kernel_size = inputs.get_integer("kernel_size");
		bool invert = inputs.get_boolean("invert");
		image* dst = src->clone();
		execute(src, dst, kernel_size, invert);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void edge_laplacian::execute(image* in_src, image* in_dst, int kernel_size, bool invert)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		std::vector<cv::Mat> channels;
		cv::split(src, channels);
		for (int c = 0; c < src.channels(); ++c)
		{
			Laplacian(channels[c], channels[c], CV_8U, kernel_size);
		}
		cv::merge(channels, dst);
		in_dst->convert_to(in_dst, image::Format::Grey);

		if(invert)
			cv::bitwise_not(*in_dst->get_opencv(), *in_dst->get_opencv());
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
