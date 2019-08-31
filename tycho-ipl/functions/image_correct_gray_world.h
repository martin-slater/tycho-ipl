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
#endif // _MSC_VER

#ifndef _IMAGE_CORRECT_GRAY_WORLD_H_73364EA8_B8F4_4BEF_9E43_DF22A3F338AC
#define _IMAGE_CORRECT_GRAY_WORLD_H_73364EA8_B8F4_4BEF_9E43_DF22A3F338AC

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
//
//----------------------------------------------------------------------------
class TYCHO_IMAGEPROCESSING_ABI image_correct_gray_world : public function
{
public:
	/// Default constructor
	image_correct_gray_world();

	bool dispatch(context *ctx, const kv_dict &kwargs, kv_dict &outputs) override;
	virtual void execute(image *src, image *dst);
};

} // namespace functions
} // namespace image_processing
} // namespace tycho

#endif // _IMAGE_CORRECT_GRAY_WORLD_H_73364EA8_B8F4_4BEF_9E43_DF22A3F338AC
