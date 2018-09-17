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
#include "auto_level_component_stretch.h"
#include "common.h"
#include "../image.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{
	
	static const char Name[] = "auto_level_rgb_stretch";
	static const char Desc[] = "Auto levels image lighting by stretching each RGB channel to use the full range [0,255].";

	//----------------------------------------------------------------------------

	auto_level_component_stretch::auto_level_component_stretch() :
		UnaryFunction(Name, Desc)
	{
	}

	//----------------------------------------------------------------------------

	void auto_level_component_stretch::execute(image* in_src, image* in_dst)
	{
		using namespace cv;

		IMAGE_PROC_ASSERT(in_src);
		IMAGE_PROC_ASSERT(in_dst);

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		// get max and min of the channel
		int r_max, g_max, b_max;
		r_max = g_max = b_max = std::numeric_limits<int>::min();
		int r_min, g_min, b_min;
		r_min = g_min = b_min = std::numeric_limits<int>::max();

		const int width = src.size().width;
		const int height = src.size().height;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Vec3b val = src.at<Vec3b>(Point(x, y));

				r_min = std::min<int>(r_min, val[2]);
				r_max = std::max<int>(r_max, val[2]);

				g_min = std::min<int>(g_min, val[1]);
				g_max = std::max<int>(g_max, val[1]);

				b_min = std::min<int>(b_min, val[0]);
				b_max = std::max<int>(b_max, val[0]);
			}
		}

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Vec3b val = dst.at<Vec3b>(Point(x, y));
				Vec3b val2;
				val2[0] = (unsigned char)(float(val[0] - b_min) / (b_max - b_min) * 255.0f);
				val2[1] = (unsigned char)(float(val[1] - g_min) / (g_max - g_min) * 255.0f);
				val2[2] = (unsigned char)(float(val[2] - r_min) / (r_max - r_min) * 255.0f);
				dst.at<Vec3b>(Point(x, y)) = val2;
			}
		}
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
