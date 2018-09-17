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
#include "denoise.h"
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
	static const char Name[] = "denoise";
	static const char Desc[] = "Remove noise from the passed image.";
	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Float, "strength", "higher removes more noise but will remove more features", value::make_float(3.0f))
	};

	//----------------------------------------------------------------------------

	denoise::denoise() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{
	}

	//----------------------------------------------------------------------------

	bool denoise::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		float stength = inputs.get_float("strength");
		image* dst = src->clone();
		execute(src, dst, stength);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void denoise::execute(image* in_src, image* in_dst, float strength)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

#if 0 //def _DEBUG
		// this hangs in debug opencv lib
		src.copyTo(dst);
#else		
		if (src.channels() == 1)
			fastNlMeansDenoising(src, dst, strength);
		else
			fastNlMeansDenoisingColored(src, dst, strength);
#endif
	}

	//----------------------------------------------------------------------------

	
} // end namespace
} // end namespace
} // end namespace
