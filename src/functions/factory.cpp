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
#include "factory.h"
#include "greyscale.h"
#include "sepia_rgb.h"
#include "sepia_yiq.h"
#include "auto_level_open_cv.h"
#include "auto_level_histogram_clip.h"
#include "auto_level_component_stretch.h"
#include "denoise.h"
#include "gaussian_blur.h"
#include "edge_canny.h"
#include "edge_laplacian.h"
#include "threshold.h"
#include "resize.h"
#include "rescale.h"
#include "bilateral.h"
#include "copy.h"
#include "image_adjust.h"
#include "gamma_correct.h"
#include "arithmetic.h"
#include "blend_functions.h"
#include "image_functions.h"
#include "remove_intensity.h"
#include "interface_functions.h"
#include "adaptive_edge_laplacian.h"
#include "adaptive_edge_canny.h"
#include "morphological.h"
#include "oil_painting.h"
#include "edge_sobel.h"
#include "kuwahara.h"
#include "color_reduce_kmeans_cluster.h"
#include "color_reduce_median_cut.h"
#include "color_reduce_im_mean_shift.h"
#include "color_reduce_im_quantize.h"
#include "color_reduce_lib_image_quant.h"
#include "visualize_palette.h"
#include "simplify_colors.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{
	//----------------------------------------------------------------------------

	factory::factory()
	{

#define ADD_FUNC(_type) add_func<_type>();
#define ADD_FUNCN(_type, _name) add_func<_type>(_name);


		// default choices when there are several algorithms to choose from
		ADD_FUNCN(functions::sepia_yiq, "sepia");
		ADD_FUNCN(functions::auto_level_histogram_clip, "auto_level");
		
		// image functions
		ADD_FUNC(functions::image_convert);
		ADD_FUNC(functions::image_load);
		ADD_FUNC(functions::image_save);
		ADD_FUNC(functions::image_clamp_size);

		// execution interface functions
		ADD_FUNC(functions::experiment_add_image);

		// all algorithms
		ADD_FUNC(functions::greyscale);
		ADD_FUNC(functions::sepia_rgb);
		ADD_FUNC(functions::sepia_yiq);
		ADD_FUNC(functions::auto_level_open_cv);
		ADD_FUNC(functions::auto_level_component_stretch);
		ADD_FUNC(functions::auto_level_histogram_clip);
		ADD_FUNC(functions::denoise);
		ADD_FUNC(functions::gaussian_blur);
		ADD_FUNC(functions::edge_canny);
		ADD_FUNC(functions::edge_laplacian);
		ADD_FUNC(functions::edge_sobel);
		ADD_FUNC(functions::adaptive_edge_laplacian);
		ADD_FUNC(functions::resize);
		ADD_FUNC(functions::rescale);
		ADD_FUNC(functions::threshold);
		ADD_FUNC(functions::bilateral);
		ADD_FUNC(functions::copy);
		ADD_FUNC(functions::gaussian_blur);
		ADD_FUNC(functions::image_adjust);
		ADD_FUNC(functions::gamma_correct);
		ADD_FUNC(functions::add);
		ADD_FUNC(functions::subtract);
		ADD_FUNC(functions::multiply);
		ADD_FUNC(functions::divide);
		ADD_FUNC(functions::bitwise_and);
		ADD_FUNC(functions::bitwise_or);
		ADD_FUNC(functions::bitwise_xor);
		ADD_FUNC(functions::scaled_add);
		ADD_FUNC(functions::scaled_multiply);
		ADD_FUNC(functions::remove_intensity);
		ADD_FUNC(functions::max);
		ADD_FUNC(functions::min);
		ADD_FUNC(functions::oil_painting);
		ADD_FUNC(functions::kuwahara);
		ADD_FUNC(functions::color_reduce_kmeans_cluster);
		ADD_FUNC(functions::color_reduce_median_cut);

#ifdef HAVE_IMAGE_MAGICK
		ADD_FUNC(functions::color_reduce_im_mean_shift);
		ADD_FUNC(functions::color_reduce_im_quantize);
#endif // HAVE_IMAGE_MAGICK

#ifdef HAVE_LIBIMAGEQUANT
		ADD_FUNC(functions::color_reduce_lib_image_quant);
#endif // HAVE_LIBIMAGEQUANT

		ADD_FUNC(functions::visualize_palette);
		ADD_FUNC(functions::simplify_colors);

		// morphological functions
		ADD_FUNC(functions::erode);
		ADD_FUNC(functions::dilate);

		// photoshop blend functions
		ADD_FUNC(functions::blend_normal);
		ADD_FUNC(functions::blend_lighten);
		ADD_FUNC(functions::blend_darken);
		ADD_FUNC(functions::blend_multiply);
		ADD_FUNC(functions::blend_average);
		ADD_FUNC(functions::blend_add);
		ADD_FUNC(functions::blend_subtract);
		ADD_FUNC(functions::blend_difference);
		ADD_FUNC(functions::blend_negation);
		ADD_FUNC(functions::blend_screen);
		ADD_FUNC(functions::blend_exclusion);
		ADD_FUNC(functions::blend_overlay);
		ADD_FUNC(functions::blend_soft_light);
		ADD_FUNC(functions::blend_hard_light);
		ADD_FUNC(functions::blend_color_dodge);
		ADD_FUNC(functions::blend_color_burn);
		ADD_FUNC(functions::blend_linear_dodge);
		ADD_FUNC(functions::blend_linear_burn);
		ADD_FUNC(functions::blend_linear_light);
		ADD_FUNC(functions::blend_vivid_light);
		ADD_FUNC(functions::blend_pin_light);
		ADD_FUNC(functions::blend_hard_mix);
		ADD_FUNC(functions::blend_reflect);
		ADD_FUNC(functions::blend_glow);
		ADD_FUNC(functions::blend_phoenix);

#undef ADD_FUNC
	}

	//----------------------------------------------------------------------------
	
	factory::~factory()
	{
		for (auto it : m_functions)
		{
			delete it.second;
		}
		m_functions.clear();
	}

	//----------------------------------------------------------------------------

	function* factory::create(const std::string& name) const
	{
		auto it = m_functions.find(name);
		if (it != m_functions.end())
			return it->second;
		return nullptr;
	}

	//----------------------------------------------------------------------------

	bool factory::exists(const std::string& name) const
	{
		return m_functions.find(name) != m_functions.end();
	}

	//----------------------------------------------------------------------------

	void factory::enumerate(std::function<void(function*)> func)
	{
		for (auto f : m_functions)
			func(f.second);
	}

	//----------------------------------------------------------------------------

	void factory::enumerate(std::function<void(const function*)> func) const
	{
		for (auto f : m_functions)
			func(f.second);
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
