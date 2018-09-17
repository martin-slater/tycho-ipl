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
#include "color_reduce_population.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
#include <cmath>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

using namespace cv;

namespace tycho
{
namespace image_processing
{
namespace functions
{

	static const char Name[] = "color_reduce_population";
	static const char Desc[] = "Reduces the number of colors in the image to a specified number.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer,
		"num_colors",
		"Number of colors to reduce the image to.",
		value::make_integer(256),
		std::bind(validation::positive_integer, std::placeholders::_1, std::numeric_limits<int>::max()))
	};

	//----------------------------------------------------------------------------

	color_reduce_population::color_reduce_population() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{

	}

	//----------------------------------------------------------------------------

	bool color_reduce_population::dispatch(context * /*ctx*/, const kv_dict & inputs, kv_dict & outputs)
	{
		image* src = inputs.get_image("src");
		int num_colors = inputs.get_integer("num_colors");

		image* dst = src->clone();
		execute(src, dst, num_colors);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void color_reduce_population::execute(image* /*in_src*/, image* /*in_dst*/, int /*num_colors*/)
	{
#if 0
		using namespace cv;

		Mat& src = *in_src->get_opencv();

		// Bucket size is cube root of the number of colours. This will likely give us
		// fractional bucket sizes.
		auto bucket_size = std::cbrt(num_colors);

		
		
		// array of indices into the source image
		std::vector<int> indices;
		indices.resize(src.rows * src.cols);
		for (int i = 0; i < src.rows * src.cols; ++i)
			indices[i] = i;
#endif
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
