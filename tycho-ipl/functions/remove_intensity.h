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

#ifndef REMOVEINTENSITY_H_6B03A8A4_9D6F_48EA_A411_9E44036767F0
#define REMOVEINTENSITY_H_6B03A8A4_9D6F_48EA_A411_9E44036767F0

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include "../function.h"
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

	//----------------------------------------------------------------------------
	// Removes intensity from the image depending on the passed image type.
	// 
	// ColorSpace::RGB
	// Normalise in RGB space by normalising each component
	// using C / (R + G + B) with C in [R, G, B]. 
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI remove_intensity : public function
	{
	public:
		remove_intensity();
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs) override;
		void execute(image* in_src, image* in_dst, int cutoff);
	};

	
} // end namespace
} // end namespace
} // end namespace

#endif // REMOVEINTENSITY_H_6B03A8A4_9D6F_48EA_A411_9E44036767F0
