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
#ifdef HAVE_IMAGE_MAGICK
#include "image_magick_utils.h"
#include "functions/common.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace image_magick_utils
{
	//----------------------------------------------------------------------------

	bool convert_to_im(const image& in_src, Magick::Image& dst)
	{
		using namespace cv;

		const int width = in_src.get_width();
		const int height = in_src.get_height();
		const Mat& src = *in_src.get_opencv();
		dst = Magick::Image(Magick::Geometry(width, height), "white");

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const auto& psrc = src.at<Vec3b>(y, x);
				dst.pixelColor(x, y, Magick::Color(psrc[2], psrc[1], psrc[0]));
			}
		}

		return true;
	}


	//----------------------------------------------------------------------------

	bool convert_from_im(const Magick::Image& src, image& in_dst)
	{
		using namespace cv;

		// copy image magick to opencv
		Mat& dst= *in_dst.get_opencv();

		for (int y = 0; y < in_dst.get_height(); ++y)
		{
			for (int x = 0; x < in_dst.get_width(); ++x)
			{
				auto& pdst = dst.at<Vec3b>(y, x);
				const auto& psrc = src.pixelColor(x, y);
				pdst[0] = psrc.quantumBlue(); 
				pdst[1] = psrc.quantumGreen();
				pdst[2] = psrc.quantumRed();
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace

#endif // HAVE_IMAGE_MAGICK