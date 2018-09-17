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

#ifndef IMAGEFUNCTIONS_H_EDD3C74D_50E3_4D2C_AE2F_919C2B3D20BA
#define IMAGEFUNCTIONS_H_EDD3C74D_50E3_4D2C_AE2F_919C2B3D20BA

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

	/// Load an image from disk
	class image_load : public function
	{
	public:
		image_load();
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs);

	};

	/// Save the image to disk
	class image_save : public function
	{
	public:
		image_save();
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs);
	};

	
	/// Convert from one image format to another
	class image_convert : public function
	{
	public:
		image_convert();
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs);
		void execute(image* in_src, image* in_dst, image::Format format);
	};

	/// Clamp an image to a given size. If the image is less than the given
	/// size on both axis it is left untouched otherwise it will be scaled
	/// so the longest edge is the given size whilst maintaining the correct
	/// aspect ratio.
	class image_clamp_size : public function
	{
	public:
		image_clamp_size();
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs);
		void execute(image* in_src, image* in_dst, int size, bool enlarge);
	};

} // end namespace
} // end namespace
} // end namespace

#endif // IMAGEFUNCTIONS_H_EDD3C74D_50E3_4D2C_AE2F_919C2B3D20BA
