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


#ifdef _MSC_VER
#pragma once
#endif  // _MSC_VER

#ifndef IMCOLORREDUCEMEANSHIFT_H_7020BCD4_4450_4D0C_A2C0_AAF947CF0750
#define IMCOLORREDUCEMEANSHIFT_H_7020BCD4_4450_4D0C_A2C0_AAF947CF0750

#ifdef HAVE_IMAGE_MAGICK

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include "../function.h"

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
	// Colour reduction using ImageMagicks mean shift algorithm
	// http://www.imagemagick.org/discourse-server/viewtopic.php?f=4&t=25504
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI color_reduce_im_mean_shift : public function
	{
	public:
		/// Default constructor
		color_reduce_im_mean_shift();
		bool dispatch(context* ctx, const kv_dict& kwargs, kv_dict& outputs) override;
		virtual void execute(image* src, image* dst, int kernel_size, float color_distance);
	};

	
} // end namespace
} // end namespace
} // end namespace

#endif // HAVE_IMAGE_MAGICK

#endif // IMCOLORREDUCEMEANSHIFT_H_7020BCD4_4450_4D0C_A2C0_AAF947CF0750

