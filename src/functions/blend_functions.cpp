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
#include "blend_functions.h"
#include "common.h"
#include "../image.h"
#include "../context.h"

#include <algorithm>
#include <cmath>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{
	using namespace cv;

	template<typename Func>
	void blend(image *in_src1, image* in_src2, image* in_dst, Func f)
	{
		Mat& src1 = *in_src1->get_opencv();
		Mat& src2 = *in_src2->get_opencv();
		Mat& dst = *in_dst->get_opencv();

		int width = std::min(
			src1.size().width,
			std::min(src2.size().width, dst.size().width));

		int height = std::min(
			src1.size().height,
			std::min(src2.size().height, dst.size().height));

		std::vector<cv::Mat> c1, c2, cdst;
		cv::split(src1, c1);
		cv::split(src2, c2);
		cv::split(dst, cdst);

		int cnum = std::min(
			src1.channels(),
			std::min(src2.channels(), dst.channels()));

		for (int c = 0; c < cnum; ++c)
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					auto ch1 = c1[c].at<uchar>(y, x);
					auto ch2 = c2[c].at<uchar>(y, x);

					cdst[c].at<uchar>(y, x) = cv::saturate_cast<uchar>(f(ch1, ch2));
				}
			}
		}

		cv::merge(cdst, dst);
	}

	uchar normal(uchar B, uchar)		{ return B; }
	uchar lighten(uchar B, uchar L)		{ return (((L > B) ? L : B)); }
	uchar darken(uchar B, uchar L)		{ return (((L > B) ? B : L)); }
	uchar multiply(uchar B, uchar L)	{ return static_cast<uchar>(((((int)B * L) / 255))); }
	uchar average(uchar B, uchar L)		{ return static_cast<uchar>(((((int)B + L) / 2))); }
	uchar add(uchar B, uchar L)			{ return static_cast<uchar>(((std::min<int>(255, ((int)B + L))))); }
	uchar subtract(uchar B, uchar L)	{ return static_cast<uchar>(((((int)B + L < 255) ? 0 : ((int)B + L - 255)))); }
	uchar difference(uchar B, uchar L)	{ return static_cast<uchar>(std::abs((int)B - L)); }
	uchar negation(uchar B, uchar L)	{ return static_cast<uchar>(((255 - std::abs(255 - B - L)))); }
	uchar screen(uchar B, uchar L)		{ return ((255 - (((255 - B) * (255 - L)) >> 8))); }
	uchar exclusion(uchar B, uchar L)	{ return ((B + L - 2 * B * L / 255)); }
	uchar overlay(uchar B, uchar L)		{ return (((L < 128) ? (2 * B * L / 255) : (255 - 2 * (255 - B) * (255 - L) / 255))); }
	uchar soft_light(uchar B, uchar L)	{ return static_cast<uchar>((int)(((L < 128) ? (2 * ((B >> 1) + 64)) * ((float)L / 255) : (255 - (2 * (255 - ((B >> 1) + 64)) * (float)(255 - L) / 255))))); }
	uchar hard_light(uchar B, uchar L)	{ return (overlay(L, B)); }
	uchar color_dodge(uchar B, uchar L)	{ return static_cast<uchar>((((L == 255) ? L : std::min(255, ((B << 8) / (255 - L)))))); }
	uchar color_burn(uchar B, uchar L)	{ return static_cast<uchar>((((L == 0) ? L : std::max(0, (255 - ((255 - B) << 8) / L))))); }
	uchar linear_dodge(uchar B, uchar L) { return B + L; }
	uchar linear_burn(uchar B, uchar L)	{ return B - L; }
	uchar linear_light(uchar B, uchar L) { return ((L < 128) ? linear_burn(B, (2 * L)) : linear_dodge(B, (2 * (L - 128)))); }
	uchar vivid_light(uchar B, uchar L)	{ return ((L < 128) ? color_burn(B, (2 * L)) : color_dodge(B, (2 * (L - 128)))); }
	uchar pin_light(uchar B, uchar L)	{ return ((L < 128) ? darken(B, (2 * L)) : lighten(B, (2 * (L - 128)))); }
	uchar hard_mix(uchar B, uchar L)		{ return (((vivid_light(B, L) < 128) ? 0 : 255)); }
	uchar reflect(uchar B, uchar L)		{ return static_cast<uchar>((((L == 255) ? L : std::min(255, (B * B / (255 - L)))))); }
	uchar glow(uchar B, uchar L)		{ return (reflect(L, B)); }
	uchar phoenix(uchar B, uchar L)		{ return ((std::min(B, L) - std::max(B, L) + 255)); }

	#define	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(ClassName, Name, Desc) \
		IMAG_PROC_DEFINE_BINARY_FUNCTION(Name, "Photoshop " #ClassName " blend function.", blend_##ClassName) \
		{ return blend(src1, src2, dst, ClassName); }

	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(normal, "blend_normal", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(lighten, "blend_lighten", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(darken, "blend_darken", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(multiply, "blend_multiply", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(average, "blend_average", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(add, "blend_add", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(subtract, "blend_sub", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(difference, "blend_difference", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(negation, "blend_negation", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(screen, "blend_screen", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(exclusion, "blend_exclusion", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(overlay, "blend_overlay", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(soft_light, "blend_soft_light", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(hard_light, "blend_hard_light", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(color_dodge, "blend_color_dodge", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(color_burn, "blend_color_burn", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(linear_dodge, "blend_linear_dodge", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(linear_burn, "blend_linear_burn", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(linear_light, "blend_linear_light", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(vivid_light, "blend_vivid_lights", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(pin_light, "blend_pin_light", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(hard_mix, "blend_hard_max", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(reflect, "blend_reflect", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(glow, "blend_glow", "");
	IMAG_PROC_DEFINE_BINARY_BLEND_FUNCTION(phoenix, "blend_phoenix", "");

} // end namespace
} // end namespace
} // end namespace
