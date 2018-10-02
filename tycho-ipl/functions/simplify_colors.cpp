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
#include "simplify_colors.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
#include "../program.h"

#include <deque>
#include <stack>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{
	static const char Name[] = "simplify_colors";
	static const char Desc[] = 
		"Simply the colors used in the image. This is not useful for general color reduction "
		"but can be useful when using logo type images that have antialiasing that needs "
		"to be removed to leave only the most dominant colors in the image";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Float,
			"merge_distance",
			"Maximum euclidean distance between colors to consider them for merging",
			value::make_float(2.0f)),
		param_desc(ObjectType::Float,
			"min_coverage",
			"Minimum percentage area to color must cover to be considered for keeping",
			value::make_float(0.5f))
	};


	//----------------------------------------------------------------------------

	simplify_colors::simplify_colors() :
		function(Group::ColorReduction, Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}

	//----------------------------------------------------------------------------


	bool simplify_colors::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		float merge_distance = inputs.get_float("merge_distance");
		float min_coverage = inputs.get_float("min_coverage");
		image* src = inputs.get_image("src");
		image* dst = src->clone();
		execute(src, dst, merge_distance, min_coverage);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------


	void CollectMergedEntries(std::vector<palette_entry>& input, size_t element, std::set<palette_entry*>& output)
	{
		std::stack<int> stack;
		stack.push(element);

		while (stack.size())
		{
			int cur = stack.top();
			stack.pop();

			auto& entry = input[cur];
			if (output.find(&entry) == output.end())
			{
				output.insert(&entry);
				for (auto idx : entry.mappings)
					stack.push(idx);
			}
		}
	}


	void RemoveIslands(image* in_src, image* in_dst, int kernel_size, int min_island_size)
	{
		using namespace cv;

		const int width = in_src->get_width();
		const int height = in_src->get_height();
		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();
		int offset = (kernel_size - 1) / 2;
		struct Pixel
		{
			Vec3b color;
			int count = 0;
		};

		std::vector<Pixel> pixels;
		pixels.resize(kernel_size*kernel_size);
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{				
				if(y < offset || x < offset || y > (height-offset) || x > (width-offset))
				{
					dst.at<Vec3b>(y, x) = src.at<Vec3b>(y, x);
				}
				else
				{
					size_t cur_p = 0;
					for (int ky = -offset; ky < offset; ++ky)
					{
						for (int kx = -offset; kx < offset; ++kx)
						{
							const Vec3b& pixel = src.at<Vec3b>(y + ky, x + kx);

							bool found = false;
							for (size_t p = 0; p < cur_p; ++p)
							{
								if (pixels[p].color == pixel)
								{
									++pixels[p].count;
									found = true;
									break;
								}
							}
							if (!found)
							{
								pixels[cur_p].color = pixel;
								pixels[cur_p].count = 1;
								++cur_p;
							}
						}
					}

					int this_idx = 0;
					Vec3b this_pixel = src.at<Vec3b>(y, x);
					while (pixels[this_idx].color != this_pixel)
						++this_idx;

					if (pixels[this_idx].count <= min_island_size)
					{
						Pixel use_pixel;
						use_pixel.count = -1;
						for (size_t p = 0; p < cur_p; ++p)
						{
							if (pixels[p].count > use_pixel.count)
							{
								use_pixel = pixels[p];
							}
						}

						if (use_pixel.count != -1)
						{
							dst.at<Vec3b>(y, x) = use_pixel.color;
						}
					}
					else
					{
						dst.at<Vec3b>(y, x) = this_pixel;
					}
				}
			}
		}
	}

	void simplify_colors::execute(image* in_src, image* in_dst, float merge_dist, float min_coverage)
	{
		using namespace cv;


		// Reduce the input image size using point sampling.
		image* small_src = in_src->clone();
		small_src->clamp_size(128, false, image::Interpolation::Nearest);

		detailed_palette palette;
		small_src->get_detailed_palette_information(palette);

		// sort by the axis with the largest range
		int rmin = INT_MAX, rmax = -1;
		int gmin = INT_MAX, gmax = -1;
		int bmin = INT_MAX, bmax = -1;

		for (const auto& entry : palette)
		{
			rmin = std::min<int>(entry.color[2], rmin);
			rmax = std::max<int>(entry.color[2], rmax);
			gmin = std::min<int>(entry.color[1], gmin);
			gmax = std::max<int>(entry.color[1], gmax);
			bmin = std::min<int>(entry.color[0], bmin);
			bmax = std::max<int>(entry.color[0], bmax);
		}
		int rrange = rmax - rmin;
		int brange = bmax - bmin;
		int grange = gmax - gmin;
		int max = std::max(std::max(rrange, grange), brange);
		int axis = 0;
		if (max == rrange)
			axis = 2;
		else if (max == brange)
			axis = 1;

		auto pred = [axis](const palette_entry& lhs, const palette_entry& rhs) 
		{
			return lhs.color[axis] < rhs.color[axis];
		};

		std::sort(palette.begin(), palette.end(), pred);
		size_t idx = 0;
		std::for_each(palette.begin(), palette.end(), [&idx](auto& val) {
			val.index = idx++;
		});

		// now run through each palette entry and find colours that are close to it
		float merge_dist_sq = 3 * merge_dist*merge_dist;
		for (auto pit = palette.begin(); pit != palette.end(); ++pit)
		{
			auto& entry = *pit;
			palette_entry ltest, rtest;
			ltest.color = entry.color;
			ltest.color[axis] = cv::saturate_cast<uchar>(ltest.color[axis] -  (int)merge_dist);
			auto lower = std::lower_bound(palette.begin(), palette.end(), ltest, pred);
			
			while (lower != palette.end())
			{	
				auto& ethis = *lower;
				int edist = abs((int)ethis.color[axis] - (int)entry.color[axis]);
				if (edist > merge_dist)
					break;

				// squared distance
  				Vec3b vec;
				vec[0] = static_cast<uint8_t>(abs((int)ethis.color[0] - (int)entry.color[0]));
				vec[1] = static_cast<uint8_t>(abs((int)ethis.color[1] - (int)entry.color[1]));
				vec[2] = static_cast<uint8_t>(abs((int)ethis.color[2] - (int)entry.color[2]));

				auto dist = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
				if (dist < merge_dist_sq)
				{
					entry.mappings.push_back(lower->index);
					lower->mappings.push_back(pit->index);
				}
				++lower;
			}
		}

		// merge the linked list of palette entries
		std::vector<palette_entry> merged_pal;
		for (auto& entry : palette)
		{
			if (!entry.merged)
			{
				std::set<palette_entry*> output;
				CollectMergedEntries(palette, entry.index, output);

				Vec3b merge_clr;
				float coverage = 0;
				size_t num_pixels = 0;

				if (output.size() == 1)
				{
					auto& oit = *output.begin();
					oit->merged = true;
					merge_clr = oit->color;
					coverage = oit->coverage;
					num_pixels = oit->num_pixels;
				}
				else
				{
					int max_pop = -1;
					for (auto out : output)
					{
						coverage += out->coverage;
						num_pixels += out->num_pixels;
						out->merged = true;
						if ((int)out->num_pixels > max_pop)
						{
							max_pop = out->num_pixels;
							merge_clr = out->color;
						}
					}
					IMAGE_PROC_ASSERT(max_pop != -1);
				}
				palette_entry new_entry;
				new_entry.color = merge_clr;
				new_entry.coverage = coverage;
				new_entry.num_pixels = num_pixels;
				merged_pal.push_back(new_entry);
			}
		}

		// sort colors by coverage
		std::sort(merged_pal.begin(), merged_pal.end(), [](const palette_entry& lhs, const palette_entry& rhs) {
			return lhs.coverage < rhs.coverage;
		});

		// keep any region with over N% coverage
		std::vector<Vec3b> new_palette;
		for (const auto& entry : merged_pal)
		{
			if (entry.coverage > min_coverage)
				new_palette.push_back(entry.color);
		}

		image* tmp = in_src->clone();
		tmp->remap_to_palette(&new_palette[0], new_palette.size());
		RemoveIslands(tmp, in_dst, 5, 4);
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
