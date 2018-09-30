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
#include "auto_level_histogram_clip.h"
#include "common.h"
#include "../image.h"
#include "../context.h"
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
	static const char Name[] = "auto_level_histogram_clip";
	static const char Desc[] = "Auto levels image lighting based on the image histogram.";

	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Float, "clip_percent", "percentage to clip from histogram", value::make_float(1.0f))
	};

	//----------------------------------------------------------------------------

	auto_level_histogram_clip::auto_level_histogram_clip() :
		function(Group::Leveling, Name, Desc, Inputs, function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool auto_level_histogram_clip::dispatch(context* /*ctx*/, const kv_dict& inputs,  kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		float clip_percent = inputs.get_float("clip_percent");
		image* dst = src->clone();
		execute(src, dst, clip_percent);
		outputs.set_image("dst", dst);
		return true;
	}
	//----------------------------------------------------------------------------
	
	void auto_level_histogram_clip::execute(image* in_src, image* in_dst, float clipHistPercent)
	{
		using namespace cv;

		Mat& src = *in_src->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		IMAGE_PROC_ASSERT(clipHistPercent >= 0);
		IMAGE_PROC_ASSERT((src.type() == CV_8UC1) || (src.type() == CV_8UC3) || (src.type() == CV_8UC4));

		int histSize = 256;
		double alpha, beta;
		double minGray = 0, maxGray = 0;

		//to calculate grayscale histogram
		cv::Mat gray;
		if (src.type() == CV_8UC1) gray = src;
		else if (src.type() == CV_8UC3) cvtColor(src, gray, CV_BGR2GRAY);
		else if (src.type() == CV_8UC4) cvtColor(src, gray, CV_BGRA2GRAY);
		if (clipHistPercent == 0)
		{
			// keep full available range
			cv::minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			cv::Mat hist; //the grayscale histogram

			float range[] = { 0, 256 };
			const float* histRange = { range };
			bool uniform = true;
			bool accumulate = false;
			calcHist(&gray, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

			// calculate cumulative distribution from the histogram
			std::vector<float> accumulator(histSize);
			accumulator[0] = hist.at<float>(0);
			for (int i = 1; i < histSize; i++)
			{
				accumulator[i] = accumulator[i - 1] + hist.at<float>(i);
			}

			// locate points that cuts at required value
			float max = accumulator.back();
			clipHistPercent *= (max / 100.0f); //make percent as absolute
			clipHistPercent /= 2.0; // left and right wings
			// locate left cut
			minGray = 0;
			while (accumulator[(int)minGray] < clipHistPercent)
				minGray++;

			// locate right cut
			maxGray = histSize - 1;
			while (accumulator[(int)maxGray] >= (max - clipHistPercent))
				maxGray--;
		}

		// current range
		double inputRange = maxGray - minGray;

		alpha = (histSize - 1) / inputRange;   // alpha expands current range to histsize range
		beta = -minGray * alpha;             // beta shifts current range so that minGray will go to 0

		// Apply brightness and contrast normalization
		// convertTo operates with saurate_cast
		src.convertTo(dst, -1, alpha, beta);

		// restore alpha channel from source 
		if (dst.type() == CV_8UC4)
		{
			int from_to[] = { 3, 3 };
			cv::mixChannels(&src, 4, &dst, 1, from_to, 1);
		}
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
