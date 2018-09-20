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
#include "color_reduce_kmeans_cluster.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
#include "opencv2/core/types_c.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{

	static const char Name[] = "color_reduce_kmeans";
	static const char Desc[] = "Reduces the number of colors in the image to a specified number.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer,
		"num_colors",
		"Number of colors to reduce the image to. If it is 0 then it will automatically determine the number of colors.",
		value::make_integer(256),
		std::bind(validation::positive_integer, std::placeholders::_1, std::numeric_limits<int>::max())),

		param_desc(ObjectType::Integer,
		"num_attempts",
		"Number of iterations to search for a result",
		value::make_integer(1),
		std::bind(validation::positive_integer, std::placeholders::_1, std::numeric_limits<int>::max())),

		param_desc(ObjectType::Integer,
		"term_epsilon",
		"Termination epsilon",
		value::make_integer(50),
		std::bind(validation::positive_integer, std::placeholders::_1, std::numeric_limits<int>::max())),

		param_desc(ObjectType::Integer,
		"term_iterations",
		"Number of iterations to search for a result",
		value::make_integer(4),
		std::bind(validation::positive_integer, std::placeholders::_1, std::numeric_limits<int>::max())),


	};

	//----------------------------------------------------------------------------

	color_reduce_kmeans_cluster::color_reduce_kmeans_cluster() : 
		function(Name, Desc, Inputs, function::DefaultOutputs(), declaration_list()) 
	{}

	//----------------------------------------------------------------------------

	bool color_reduce_kmeans_cluster::dispatch(context * /*ctx*/, const kv_dict & inputs, kv_dict & outputs)
	{
		image* src = inputs.get_image("src");
		int num_colors = inputs.get_integer("num_colors");
		int num_attempts = inputs.get_integer("num_attempts");
		int term_epsilon = inputs.get_integer("term_epsilon");
		int term_iterations = inputs.get_integer("term_iterations");

		image* dst = src->clone();
		execute(src, dst, num_colors, num_attempts, term_epsilon, term_iterations);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void color_reduce_kmeans_cluster::execute(image* in_src, image* in_dst,
		int num_colors, int num_attempts,
		int term_epsilon, int term_iterations)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		// build samples matrix
		Mat samples(src.rows * src.cols, 3, CV_32F);
		for (int y = 0; y < src.rows; y++)
			for (int x = 0; x < src.cols; x++)
				for (int z = 0; z < 3; z++)
					samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y, x)[z];


		// calculate the kmeans for the image
		Mat labels;
		Mat centers;
		kmeans(samples, num_colors, labels, 
			TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, term_iterations, term_epsilon),
			num_attempts, KMEANS_PP_CENTERS, centers);


		// create a new image mapping existing pixels to the reduced clusters
		for (int y = 0; y < src.rows; y++)
		{
			for (int x = 0; x < src.cols; x++)
			{
				int cluster_idx = labels.at<int>(y + x*src.rows, 0);
				dst.at<Vec3b>(y, x)[0] = (uint8_t)centers.at<float>(cluster_idx, 0);
				dst.at<Vec3b>(y, x)[1] = (uint8_t)centers.at<float>(cluster_idx, 1);
				dst.at<Vec3b>(y, x)[2] = (uint8_t)centers.at<float>(cluster_idx, 2);
			}
		}
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
