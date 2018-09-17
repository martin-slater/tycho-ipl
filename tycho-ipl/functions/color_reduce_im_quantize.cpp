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

#ifdef HAVE_IMAGE_MAGICK

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "color_reduce_im_quantize.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
#include "../image_magick_utils.h"

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

	static const char Name[] = "color_reduce_im_quantize";
	static const char Desc[] = "Reduces the number of colors in the image to a specified number.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer,
		"num_colors",
		"Number of colors to reduce the image to. This must be a power of 2 (2, 4, 8, 16, etc).",
		value::make_integer(256),
		std::bind(validation::pow2_integer, std::placeholders::_1))
	};

	//----------------------------------------------------------------------------

	color_reduce_im_quantize::color_reduce_im_quantize() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{

	}

	//----------------------------------------------------------------------------

	bool color_reduce_im_quantize::dispatch(context * ctx, const kv_dict & inputs, kv_dict & outputs)
	{
		image* src = inputs.get_image("src");
		int num_colors = inputs.get_integer("num_colors");

		image* dst = src->clone();
		execute(src, dst, num_colors);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------


	void color_reduce_im_quantize::execute(image* in_src, image* in_dst, int num_colors)
	{
		using namespace cv;
		namespace im = Magick;

		IMAGE_PROC_ASSERT(in_src->get_format() == image::Format::RGB);

		im::Image im_img;
		image_magick_utils::convert_to_im(*in_src, im_img);

		im_img.quantizeColorSpace(im::LabColorspace);
		im_img.quantizeColors(num_colors);
		im_img.quantizeDitherMethod(im::NoDitherMethod);
		im_img.quantize();

		image_magick_utils::convert_from_im(im_img, *in_dst);
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace

#endif // HAVE_IMAGE_MAGICK