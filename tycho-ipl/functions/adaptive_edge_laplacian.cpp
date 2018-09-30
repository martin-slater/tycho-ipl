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
#include "adaptive_edge_laplacian.h"
#include "edge_laplacian.h"
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
	static const char Name[] = "adaptive_edge_laplacian";
	static const char Desc[] = "Find edges using a Laplacian filter. This will " 
		                       "search the filter space for the result that "
							   "contains the closest to the requested percentage "
							   "of edge pixels";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),

		param_desc(ObjectType::Integer,
		"edge_percent",
		"Percentage of edge pixels to target",
		value::make_integer(5)),

		param_desc(ObjectType::Integer,
		"min",
		"Minimum value to be classified as an edge pixels. Pixels >= than this will be forced to 255",
		value::make_integer(64)),

		param_desc(ObjectType::Boolean,
		"invert",
		"Invert the results",
		value::make_boolean(false)),

		param_desc(ObjectType::Boolean,
		"adaptive_cutoff",
		"Refine the best image found searching the filter space by changing the cutoff value",
		value::make_boolean(false))

	};

	//----------------------------------------------------------------------------

	adaptive_edge_laplacian::adaptive_edge_laplacian() :
		function(Group::EdgeDetection, Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------

	bool adaptive_edge_laplacian::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int edge_percent = inputs.get_integer("edge_percent");
		int lower_cutoff = inputs.get_integer("min");
		bool invert = inputs.get_boolean("invert");
		bool apply_adaptive_cutoff = inputs.get_boolean("adaptive_cutoff");
		image* dst = src->clone();
		execute(src, dst, edge_percent, lower_cutoff, invert, apply_adaptive_cutoff);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	int CountPixels(const image* img, int cutoff)
	{
		IMAGE_PROC_ASSERT(img->get_format() == image::Format::Grey);
	
		// count pixels detected as edges 
		int count = 0;
		const cv::Mat& tmp_mat = *img->get_opencv();
		for (int y = 0; y < tmp_mat.rows; y++)
		{
			for (int x = 0; x < tmp_mat.cols; x++)
			{
				if (tmp_mat.at<uchar>(y, x) > cutoff)
					++count;
			}
		}
		
		return count;
	}

	image_ptr FindClosestCuttoffToPercent(image* image, int cutoff, int inc, int steps, int target_pc, float &best_pc)
	{
		int best_cutoff = cutoff;
		for (int i = 1; i < steps + 1; ++i)
		{
			const int nc = cutoff + inc * i;
			const int count = CountPixels(image, nc);
			const float pc = count / float(image->get_width() * image->get_height()) * 100.0f;
			if (std::abs(target_pc - pc) < std::abs(best_pc - pc))
			{
				best_pc = pc;
				best_cutoff = nc;
			}
		}

		image_ptr dst(image->clone());
		cv::threshold(*image->get_opencv(), *dst->get_opencv(), best_cutoff, 255, cv::THRESH_BINARY);
		return dst;
	}

	void adaptive_edge_laplacian::execute(
		image* in_src, image* in_dst, 
		int edge_percent, int lower_cutoff, 
		bool invert, bool apply_adaptive_cutoff)
	{
		edge_laplacian filter;
		
		image_ptr temp(new image(image::Format::Grey, in_dst->get_width(), in_dst->get_height()));
		image_ptr best = nullptr;
		float best_pc = FLT_MAX;
		const auto image_area = float(in_src->get_height() * in_src->get_width());

		// test different kernel sizes and get as close as we can to the target 
		// percentage
 		const int strengths[] = { 3, 5, 7, 9 };
		for (int strength : strengths)
		{
				filter.execute(in_src, temp.get(), strength, false);

			image_ptr gray(new image());
			temp->convert_to(gray.get(), image::Format::Grey);
			
			// count pixels detected as edges 
			int edge_count = CountPixels(gray.get(), lower_cutoff);
			float pc = edge_count / image_area * 100.0f;
			if (std::abs(edge_percent - pc) < std::abs(edge_percent - best_pc))
			{
				best_pc = pc;
				best = gray;
			}
		}

		// now try and refine the result by adjusting the lower cutoff a little	
		if (apply_adaptive_cutoff)
		{
			const int inc_size = 8;
			if (best_pc < edge_percent)
				best = FindClosestCuttoffToPercent(best.get(), lower_cutoff, -inc_size, 4, edge_percent, best_pc);
			else
				best = FindClosestCuttoffToPercent(best.get(), lower_cutoff, inc_size, 4, edge_percent, best_pc);
		}
		else
		{
			image_ptr dst(best->clone());
			cv::threshold(*best->get_opencv(), *dst->get_opencv(), lower_cutoff, 255, cv::THRESH_BINARY);
			best = dst;
		}

		cv::Mat dst;
		if (invert)
			cv::bitwise_not(*best->get_opencv(), dst);
		else
			best->get_opencv()->copyTo(dst);

		in_dst->set_mat(dst, best->get_format());
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
