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
#include "color_reduce_lib_image_quant.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
#include "../../3rdparty/libimagequant/libimagequant.h"

#ifdef HAVE_LIBIMAGEQUANT

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{
	static const char Name[] = "color_reduce_libimagequant";
	static const char Desc[] = "Reduces the number of colors in the image to a specified number.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer,
		"num_colors",
		"Number of colors to reduce the image to. If it is 0 then it will automatically determine the number of colors.",
		value::make_integer(256),
		std::bind(validation::positive_integer, std::placeholders::_1, std::numeric_limits<int>::max()))
	};

	//----------------------------------------------------------------------------

	color_reduce_lib_image_quant::color_reduce_lib_image_quant() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{

	}

	//----------------------------------------------------------------------------
	
	bool color_reduce_lib_image_quant::dispatch(context * /*ctx*/, const kv_dict & inputs, kv_dict & outputs)
	{
		image* src = inputs.get_image("src");
		int num_colors = inputs.get_integer("num_colors");

		image* dst = src->clone();
		execute(src, dst, num_colors);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void color_reduce_lib_image_quant::execute(image* in_src, image* in_dst, int num_colors)
	{
		using namespace cv;

		// LibImageQuant only accepts image in RGBA format so if needed convert to RGB and add an alpha channel
		// then convert back after.
		const int width = in_src->get_width();
		const int height = in_src->get_height();
		const Mat& src = *in_src->get_opencv();

		std::vector<unsigned char> raw_pixels(width * height * 4);
		const int stride = width * 4;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const auto& psrc = src.at<Vec3b>(y, x);

				unsigned char* pdst = &raw_pixels[y * stride + x * 4];
				pdst[0] = psrc[0];
				pdst[1] = psrc[1];
				pdst[2] = psrc[2];
				pdst[3] = 255;
			}
		}

		liq_attr *attr = liq_attr_create();
		liq_set_max_colors(attr, num_colors);
		liq_image *image = liq_image_create_rgba(attr, &raw_pixels[0], width, height, 0);
		liq_result *res;
		liq_image_quantize(image, attr, &res);

		std::vector<unsigned char> out_8bpp(width * height);
		liq_write_remapped_image(res, image, &out_8bpp[0], width * height);
		const liq_palette *pal = liq_get_palette(res);

		Mat& dst = *in_dst->get_opencv();

		for (int y = 0; y < in_dst->get_height(); ++y)
		{
			for (int x = 0; x < in_dst->get_width(); ++x)
			{
				auto& pdst = dst.at<Vec3b>(y, x);
				const unsigned char src_index = out_8bpp[y * width + x];			
				pdst[0] = pal->entries[src_index].r;
				pdst[1] = pal->entries[src_index].g;
				pdst[2] = pal->entries[src_index].b;
			}
		}

		liq_result_destroy(res);
		liq_image_destroy(image);
		liq_attr_destroy(attr);
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace

#endif // HAVE_LIBIMAGEQUANT