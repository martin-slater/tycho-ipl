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
#include "auto_level_open_cv.h"
#include "common.h"
#include "../image.h"
#include "opencv2/imgproc/types_c.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{
	static const char Name[] = "auto_level_opencv";
	static const char Desc[] = "Auto levels image lighting using OpenCV's built in algorithm";

	//----------------------------------------------------------------------------

	auto_level_open_cv::auto_level_open_cv() :
		UnaryFunction(Group::Leveling, Name, Desc)
	{
	}

	//----------------------------------------------------------------------------
	
	void auto_level_open_cv::execute(image* src, image* dst)
	{
		using namespace cv;

		IMAGE_PROC_ASSERT(src);
		IMAGE_PROC_ASSERT(dst);

		std::vector<Mat> channels;
		Mat img_hist_equalized;

		// convert from BGR -> YCrCb
		cvtColor(*src->get_opencv(), img_hist_equalized, CV_BGR2YCrCb);

		// split the image into its component channels
		split(img_hist_equalized, channels);

		// equalize the Y (intensity) channel
		equalizeHist(channels[0], channels[0]);

		// interleave all channels again
		merge(channels, img_hist_equalized);

		// convert back from YCrCb -> BGR
		cvtColor(img_hist_equalized, *dst->get_opencv(), CV_YCrCb2BGR);
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
