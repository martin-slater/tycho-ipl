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
#include "sepia_yiq.h"
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

	static const char Name[] = "sepia_yiq";
	static const char Desc[] = "Sepia tone effect calculated in YIQ space. Results in a deeper brown colour than sepia_rgb.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "offset", "Colour offset to move around the default sepia tone."),
	};

	//----------------------------------------------------------------------------

	sepia_yiq::sepia_yiq() :
		function(
		Name, Desc,
		Inputs, function::DefaultOutputs(), declaration_list())
	{
	}


	//----------------------------------------------------------------------------

	bool sepia_yiq::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int size = inputs.get_integer("offset");
		image* dst = src->clone();
		execute(src, dst, size);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void sepia_yiq::execute(image* src, image* dst, int offset)
	{
		// Converts into YIQ space using the following formula
		//		Y = 0.299 * R + 0.587 * G + 0.114 * B
		//		I = 0.596 * R - 0.274 * G - 0.322 * B
		//		Q = 0.212 * R - 0.523 * G + 0.311 * B
		// 
		// The set all pixels
		//		I = 51
		//		Q = 0
		//
		// Then converts back to RGB
		//
		//		B = 1.0 * Y - 1.105 * I + 1.702 * Q
		//		G = 1.0 * Y - 0.272 * I - 0.647 * Q
		//		R = 1.0 * Y + 0.956 * I + 0.621 * Q
		//
		using namespace cv;

		IMAGE_PROC_ASSERT(src);
		IMAGE_PROC_ASSERT(dst);

		// image in BGR format
		cv::Mat rgb2yiq_mat;
		cv::Mat yiq2rgb_mat;
		cv::Mat *cv_src = src->get_opencv();

		if (cv_src->channels() == 3)
		{
			rgb2yiq_mat = (cv::Mat_<float>(3, 3) <<
				0.114, 0.587, 0.299,
				-0.322, -0.274, 0.596,
				0.311, -0.523, 0.212
				);

			yiq2rgb_mat = (cv::Mat_<float>(3, 3) <<
				1.0, -1.105, 1.702,
				1.0, -0.272, -0.647,
				1.0, 0.956, 0.621
				);
		}
		cv::Mat yiq = cv_src->clone();
		cv::transform(*cv_src, yiq, rgb2yiq_mat);

		for (int y = 0; y < cv_src->size().height; ++y)
		{
			for (int x = 0; x < cv_src->size().width; ++x)
			{
				Vec3b val = yiq.at<Vec3b>(Point(x, y));
				val[1] = saturate_cast<uchar>(51 + offset);
				val[2] = 0;
				yiq.at<Vec3b>(Point(x, y)) = val;
			}
		}

		
		cv::transform(yiq, *dst->get_opencv(), yiq2rgb_mat);
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
