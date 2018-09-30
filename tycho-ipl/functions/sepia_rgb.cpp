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
#include "sepia_rgb.h"
#include "common.h"
#include "../key_value.h"
#include "../context.h"
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

	static const char Name[] = "sepia_rgb";
	static const char Desc[] = "Sepia tone effect calculated in RGB space. Results in a less brown image than sepia_yiq.";

	//----------------------------------------------------------------------------

	sepia_rgb::sepia_rgb() :
		UnaryFunction(Group::Artistic, Name, Desc)
	{
	}


	//----------------------------------------------------------------------------

	void sepia_rgb::execute(image* src, image* dst)
	{
		IMAGE_PROC_ASSERT(src);
		IMAGE_PROC_ASSERT(dst);

		cv::Mat* cv_src = src->get_opencv(); 
		cv::Mat kernel;

		// uses 
		if (cv_src->channels() == 4)
		{
			kernel = (cv::Mat_<float>(4, 4) <<
				0.272, 0.534, 0.131, 0,
				0.349, 0.686, 0.168, 0,
				0.393, 0.769, 0.189, 0,
				0, 0, 0, 1);
		}
		else if (cv_src->channels() == 3)
		{
			kernel = (cv::Mat_<float>(3, 3) <<
				0.272, 0.534, 0.131,
				0.349, 0.686, 0.168,
				0.393, 0.769, 0.189);
		}
		cv::transform(*cv_src, *dst->get_opencv(), kernel);
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
