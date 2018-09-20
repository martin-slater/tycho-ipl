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
#include "color_reduce_median_cut.h"
#include "common.h"
#include "../image.h"
#include "../context.h"


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

	static const char Name[] = "color_reduce_median_cut";
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

	color_reduce_median_cut::color_reduce_median_cut() :
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------

	bool color_reduce_median_cut::dispatch(context * /*ctx*/, const kv_dict & inputs, kv_dict & outputs)
	{
		image* src = inputs.get_image("src");
		int num_colors = inputs.get_integer("num_colors");

		image* dst = src->clone();
		execute(src, dst, num_colors);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------


	static void Partition(
		const Vec3b* pixels, Vec3b* dst_pixels, int* indices, 
		int start, int end, int depth, int max_depth, std::vector<Vec3b>& palette)
	{
		if (depth == max_depth)
		{
			// get the average colour in this partition
			Vec3f sum = Vec3f::all(0);
			for (int p = start; p < end; ++p)
			{
				sum += pixels[indices[p]];
			}

			Vec3b avg;
			auto scale = (float)(end - start);
			avg[0] = saturate_cast<uchar>(sum[0] / scale);
			avg[1] = saturate_cast<uchar>(sum[1] / scale);
			avg[2] = saturate_cast<uchar>(sum[2] / scale);

			palette.push_back(avg);
		}
		else
		{
			// get the min and max values for each component
			Vec3b min(255, 255, 255);
			Vec3b max(0, 0, 0);

			for (int p = start; p < end; ++p)
			{
				Vec3b v = pixels[indices[p]];

				min[0] = std::min(min[0], v[0]);
				min[1] = std::min(min[1], v[1]);
				min[2] = std::min(min[2], v[2]);

				max[0] = std::max(max[0], v[0]);
				max[1] = std::max(max[1], v[1]);
				max[2] = std::max(max[2], v[2]);
			}

			// find which component has the largest range, this will become our 
			// splitting axis
			Vec3b range(max - min);
			int axis = -1;
			if (range[0] > range[1] && range[0] > range[2])
				axis = 0;
			else if (range[1] > range[0] && range[1] > range[2])
				axis = 1;
			else
				axis = 2;

			// sort pixels along the splitting axis 
			std::sort(&indices[start], &indices[end], [&pixels, axis](auto lhs, auto rhs) {
				return pixels[lhs][axis] < pixels[rhs][axis];
			});

			// split partition into 2 equal sized partitions and recurse
			int pivot = (start + end) / 2;
			Partition(pixels, dst_pixels, indices, start, pivot, depth + 1, max_depth, palette);
			Partition(pixels, dst_pixels, indices, pivot, end, depth + 1, max_depth, palette);
		}
	}

	void color_reduce_median_cut::execute(image* in_src, image* in_dst, int num_colors)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		int depth = utils::int_log2(num_colors);

		// array of indices into the source image
		std::vector<int> indices;
		indices.resize(src.rows * src.cols);
		for (int i = 0; i < src.rows * src.cols; ++i)
			indices[i] = i;

		std::vector<Vec3b> palette;
		Partition((Vec3b*)src.data, (Vec3b*)dst.data, indices.data(), 0, indices.size(), 0, depth, palette);
		in_dst->remap_to_palette(&palette[0], palette.size());
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
