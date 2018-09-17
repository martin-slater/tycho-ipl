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

#ifndef COLORREDUCEIMQUANTIZE_H_4E8E1CF8_4EA0_4682_9AA6_0AE9AC848CAA
#define COLORREDUCEIMQUANTIZE_H_4E8E1CF8_4EA0_4682_9AA6_0AE9AC848CAA

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
	// Colour reduction using ImageMagicks color quantization algorithm
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI color_reduce_im_quantize : public function
	{
	public:
		/// Default constructor
		color_reduce_im_quantize();
		bool dispatch(context* ctx, const kv_dict& kwargs, kv_dict& outputs) override;
		virtual void execute(image* src, image* dst, int num_colors);
	};


} // end namespace
} // end namespace
} // end namespace

#endif // HAVE_IMAGE_MAGICK

#endif // COLORREDUCEIMQUANTIZE_H_4E8E1CF8_4EA0_4682_9AA6_0AE9AC848CAA
