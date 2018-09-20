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
#include "rescale.h"
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
	static const char Name[] = "rescale";
	static const char Desc[] = "Proportionally resize an image";
	
	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Float, "scale_x", "Amount to scale width", value::make_float(1.0f)),
		param_desc(ObjectType::Float, "scale_y", "Amount to scale height", value::make_float(1.0f))
		};

	//----------------------------------------------------------------------------

	rescale::rescale() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool rescale::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		float scale_x = inputs.get_float("scale_x");
		float scale_y = inputs.get_float("scale_y");
		image* dst = src->clone();
		execute(src, dst, scale_x, scale_y);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void rescale::execute(image* in_src, image* in_dst, float scale_x, float scale_y)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		Size size(
			(int)(src.size().width * scale_x),
			(int)(src.size().height * scale_y));

		resize(src, dst, size, 0, 0, INTER_LINEAR);
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
