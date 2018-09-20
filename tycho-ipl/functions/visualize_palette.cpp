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
#include "visualize_palette.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
#include "../program.h"


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

	static const char Name[] = "visualize_palette";
	static const char Desc[] = "Create a visualization of the colours used in the image";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput()
	};

	//----------------------------------------------------------------------------

	visualize_palette::visualize_palette() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------

	bool visualize_palette::dispatch(context * /*ctx*/, const kv_dict &inputs, kv_dict & outputs)
	{
		image* src = inputs.get_image("src");
		image* dst = src->clone();
		execute(src, dst);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void visualize_palette::execute(image *in_src, image *in_dst)
	{
		std::set<uint32_t> unique_clrs;

		Mat& src = *in_src->get_opencv();

		const int src_width = src.size().width;
		const int src_height = src.size().height;

		for (int y = 0; y < src_height; ++y)
		{
			for (int x = 0; x < src_width; ++x)
			{
				Vec3b val = src.at<Vec3b>(Point(x, y));
				uint32_t key = val[0] | (val[1] << 8) | (val[2] << 16);
					unique_clrs.insert(key);
			}
		}
		const int num_colors = (int)unique_clrs.size();

		// layout options
		float clr_step;
		int   txt_col_width = 196;
		int   clr_col_width = 110;
		int   header_height = 48;
		float   max_clr_height = 64;
		float   min_clr_height = 20;
		float clr_col_height;
		bool  show_color_values = false;
		int   padding = 2;
		float font_scale = 1.5f;
		int   palette_height = src_height - header_height + padding * 2;

		if (num_colors > 32 && num_colors > (palette_height/min_clr_height))
		{
			clr_step = float(num_colors) / palette_height;
			clr_col_height = 1;
			show_color_values = false;
			txt_col_width = 0;
			clr_col_width = int(clr_col_width * 1.5f);
		}
		else
		{
			clr_step = 1;
			clr_col_height = float(palette_height) / num_colors;
			if (clr_col_height > max_clr_height)
				clr_col_height = max_clr_height;
			if (clr_col_height < min_clr_height)
				clr_col_height = min_clr_height;
			if (clr_col_height > 16)
				show_color_values = true;
			else
			{
				txt_col_width = 0;
				clr_col_width = int(clr_col_width * 1.5f);
			}
		}
		int total_width = txt_col_width + clr_col_width + padding * 2;
		auto min_dst_height = (int)(floor(num_colors / clr_step) * clr_col_height);

		// increase image size 
		in_dst->resize(src_width + total_width, std::max(src_height, min_dst_height));
		in_dst->draw_filled_rect(0, 0, in_dst->get_width(), in_dst->get_height(), Scalar(66, 66, 66, 0));
		in_dst->draw(in_src, 0, 0, src_width, src_height);
		
		// draw header
		char header_txt[256] = { 0 };
		snprintf(&header_txt[0], 255, "%d clrs", (int)num_colors);
		in_dst->draw_string(
			header_txt, 
			src_width + padding + total_width / 2, 
			padding + header_height, 
			font_scale, 2, image::Anchor::Center);

		// draw colour blocks
		int cur_y = header_height + padding;
		auto palette_it = unique_clrs.begin();
		for (float fidx = 0; fidx < num_colors && palette_it != unique_clrs.end();)
		{
			int set_clr = *palette_it;
			Vec3b clr;
			clr[0] = static_cast<uint8_t>(set_clr & 0x0000ff);
			clr[1] = static_cast<uint8_t>((set_clr & 0x00ff00) >> 8);
			clr[2] = static_cast<uint8_t>((set_clr & 0xff0000) >> 16);

			int cur = (int)floor(fidx);
			fidx += clr_step;
			int next = (int)floor(fidx);
			while (cur < next && palette_it != unique_clrs.end())
			{
				++palette_it;
				++cur;
			}

			if (show_color_values)
			{
				char buf[128] = { 0 };
				snprintf(buf, 127, "%d,%d,%d", clr[2], clr[1], clr[0]);
				in_dst->draw_string(buf, src_width + padding, int(cur_y + clr_col_height / 4), font_scale, 2, image::Anchor::TopLeft);
			}
			in_dst->draw_filled_rect(
				src_width + padding + txt_col_width,
				cur_y,
				clr_col_width,
				(int)clr_col_height, Scalar(clr[0], clr[1], clr[2], 0));

			cur_y += (int)clr_col_height;
		}
	}

	//----------------------------------------------------------------------------
	

} // end namespace
} // end namespace
} // end namespace
