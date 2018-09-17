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

#ifndef VISUALIZEPALETTE_H_93D42593_FE5D_4AD4_B4F8_D454689E1710
#define VISUALIZEPALETTE_H_93D42593_FE5D_4AD4_B4F8_D454689E1710

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
	// Creates a visualization of the colours used within the image. It will also
	// optionally compare the palette generated against a user supplied list of 
	// palette colours.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI visualize_palette : public function
	{
	public:
		visualize_palette();
		bool dispatch(context* ctx, const kv_dict& kwargs, kv_dict& outputs) override;
		virtual void execute(image* src, image* dst);
	};


} // end namespace
} // end namespace
} // end namespace


#endif // VISUALIZEPALETTE_H_93D42593_FE5D_4AD4_B4F8_D454689E1710
