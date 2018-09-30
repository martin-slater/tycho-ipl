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
#include "gaussian_blur.h"
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
	static const char Name[] = "gaussian_blur";
	static const char Desc[] = "Blur the image using a gaussian filter";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "kernel_size", "Size of the kernel", value::make_integer(3)),
		param_desc(ObjectType::Float, "sigma_x", "Gaussian kernel standard deviation in X direction", value::make_float(0.0f)),
		param_desc(ObjectType::Float, "sigma_y", "Gaussian kernel standard deviation in Y direction", value::make_float(0.0f))
	};

	//----------------------------------------------------------------------------

	gaussian_blur::gaussian_blur() :
		function(Group::Filtering, Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool gaussian_blur::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int kernel_size = inputs.get_integer("kernel_size");
		float sigma_x = inputs.get_float("sigma_x");
		float sigma_y = inputs.get_float("sigma_y");
		image* dst = src->clone();
		execute(src, dst, kernel_size, sigma_x, sigma_y);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------

	void gaussian_blur::execute(image* in_src, image* in_dst, 
		int ksize, float sigma_x, float sigma_y)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		cv::GaussianBlur(src, dst, Size(ksize, ksize), sigma_x, sigma_y);
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
