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
#include "copy.h"
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

	static const char Name[] = "copy";
	static const char Desc[] = "Copy an image";
	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "mask", "Image to mask copy against", value())
	};

	//----------------------------------------------------------------------------

	copy::copy() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{
	}

	//----------------------------------------------------------------------------

	bool copy::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");

		value mask_val;
		image *mask = nullptr;
		if (inputs.try_get("mask", mask_val) && mask_val.get_type() == ObjectType::Image)
			mask = mask_val.get_image();

		image* dst = src->clone();
		execute(src, dst, mask);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void copy::execute(image* in_src, image* in_dst, image* mask)
	{
		IMAGE_PROC_ASSERT(in_src);
		IMAGE_PROC_ASSERT(in_dst);

		cv::Mat& src = *in_src->get_opencv();
		cv::Mat& dst = *in_dst->get_opencv();


		memset((char*)dst.data, 0, dst.step * dst.rows);

		if (mask)
			src.copyTo(dst, *mask->get_opencv());
		else
			src.copyTo(dst);
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
