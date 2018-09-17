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

#ifndef AUTOLEVELHISTOGRAMCLIP_H_1FF31065_0ABF_4CE2_BDEB_BA6450BA9FB4
#define AUTOLEVELHISTOGRAMCLIP_H_1FF31065_0ABF_4CE2_BDEB_BA6450BA9FB4

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
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
	// Auto levels image lighting base on its intensity histogram. This version
	// also allow clipping of the ends of the histogram and expanding into the 
	// full range. 
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI auto_level_histogram_clip: public function
	{
	public:
		auto_level_histogram_clip();
		bool dispatch(context* ctx, const kv_dict& kwargs, kv_dict& outputs) override;
		virtual void execute(image* src, image* dst, float clipHistPercent);
	};


} // end namespace
} // end namespace
} // end namespace

#endif // AUTOLEVELHISTOGRAMCLIP_H_1FF31065_0ABF_4CE2_BDEB_BA6450BA9FB4
