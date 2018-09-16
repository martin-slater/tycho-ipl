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
#include "bilateral.h"
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
	static const char Name[] = "bilateral";
	static const char Desc[] = "Apply bilateral filter to the image. This maintains edges and softens flat regions.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "sigma_color", "", value::make_integer(20)),
		param_desc(ObjectType::Integer, "filter_size", "", value::make_integer(9)),
		param_desc(ObjectType::Integer, "sigma_space", "", value::make_integer(78)),
		param_desc(ObjectType::Integer, "iterations", "Number of times to apply the filter", value::make_integer(1))
	};

	//----------------------------------------------------------------------------

	bilateral::bilateral() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{
	}


	//----------------------------------------------------------------------------

	bool bilateral::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int sigma_space = inputs.get_integer("sigma_space");
		int sigma_color = inputs.get_integer("sigma_color");
		int filter_size = inputs.get_integer("filter_size");
		int iterations  = inputs.get_integer("iterations");
		image* dst = src->clone();
		execute(src, dst, iterations, sigma_space, sigma_color, filter_size);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void bilateral::execute(
		image* in_src, image* in_dst, 
		int iterations, 
		int sigma_space, int sigma_color, int filter_size)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		Mat tmp = src.clone();
		Mat *one = &tmp;
		Mat *two = &dst;	

		for (int i = 0; i < iterations; ++i)
		{
			bilateralFilter(*one, *two, filter_size, sigma_color, sigma_space);
			std::swap(one, two);
		}

		if ((iterations & 1) == 0)
		{
			// if even iterations then tmp contains the final image
			tmp.copyTo(dst);
		}

	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
