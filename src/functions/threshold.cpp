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
#include "threshold.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
#include "../program.h"


//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{
	static const char Name[] = "threshold";
	static const char Desc[] = "Remove noise from the passed image.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "threshold", "Threshold value"),
		param_desc(ObjectType::Integer, "maxval", "Maximum value to use with the THRESH_BINARY and THRESH_BINARY_INV thresholding types"),
		param_desc(ObjectType::Integer, "type", "Thresholding type")
	};

	static const declaration_list Constants = {
		declaration(ObjectType::Integer, declaration::TypeModifier::Constant, "THRESH_BINARY", value::make_integer(0)),
		declaration(ObjectType::Integer, declaration::TypeModifier::Constant, "THRESH_BINARY_INV", value::make_integer(1)),
		declaration(ObjectType::Integer, declaration::TypeModifier::Constant, "THRESH_TRUNC", value::make_integer(2)),
		declaration(ObjectType::Integer, declaration::TypeModifier::Constant, "THRESH_TOZERO", value::make_integer(3)),
		declaration(ObjectType::Integer, declaration::TypeModifier::Constant, "THRESH_TOZERO_INV", value::make_integer(4))
	};

	//----------------------------------------------------------------------------

	threshold::threshold() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), Constants)
	{
	}

	//----------------------------------------------------------------------------


	bool threshold::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int threshold = inputs.get_integer("threshold");
		int maxval = inputs.get_integer("maxval");
		int type = inputs.get_integer("type");
		image* dst = src->clone();
		execute(src, dst, threshold, maxval, type);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void threshold::execute(image* in_src, image* in_dst, int threshold, int maxval, int type)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		cv::threshold(src, dst, threshold, maxval, type);;
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
