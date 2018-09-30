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
#include "morphological.h"
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
			"size",
			"",
			value::make_integer(3),
			std::bind(validation::positive_integer, std::placeholders::_1, 4096)
		),
		param_desc(
			ObjectType::Integer,
			"element",
			"Structuring element. One of MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE.",
			value::make_integer(cv::MORPH_ELLIPSE)
		)
	};

	//----------------------------------------------------------------------------

	morphological_function::morphological_function(const char* name, const char* desc) :
		function(Group::Structural, name, desc,  Inputs, function::DefaultOutputs(), declaration_list())
	{

	}

	//----------------------------------------------------------------------------

	bool morphological_function::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		int morph_size = inputs.get_integer("size");
		int element = inputs.get_integer("element");

		image *src = inputs.get_image("src");
		image* dst = src->clone();
		execute(src, dst, element, morph_size);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	dilate::dilate() :
		morphological_function("dilate", "dilate the image.")
	{}

	void dilate::execute(image* in_src, image* in_dst, int elem, int size)
	{
		using namespace cv;

		Mat element = getStructuringElement(elem,
			Size(2 * size + 1, 2 * size + 1),
			Point(size, size));

		cv::dilate(*in_src->get_opencv(), *in_dst->get_opencv(), element);
	}

	//----------------------------------------------------------------------------

	erode::erode() :
		morphological_function("erode", "erode the image.")
	{}

	void erode::execute(image* in_src, image* in_dst, int elem, int size)
	{
		using namespace cv;

		Mat element = getStructuringElement(elem,
			Size(2 * size + 1, 2 * size + 1),
			Point(size, size));

		cv::erode(*in_src->get_opencv(), *in_dst->get_opencv(), element);
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
