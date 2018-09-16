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

#ifndef MORPHOLOGICAL_H_87265B14_597C_49BB_9242_E17BFD9E8B24
#define MORPHOLOGICAL_H_87265B14_597C_49BB_9242_E17BFD9E8B24

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
	// Base for morphological operation
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI morphological_function : public function
	{
	public:
		morphological_function(const char* name, const char* desc);
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs);
		virtual void execute(image* in_src, image* in_dst, int elem, int size) = 0;
	};


	//----------------------------------------------------------------------------
	// Dilate the image
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI dilate : public morphological_function
	{
	public:
		dilate();
		void execute(image* in_src, image* in_dst, int elem, int size) override;
	};


	//----------------------------------------------------------------------------
	// Erode the image
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI erode : public morphological_function
	{
	public:
		erode();
		void execute(image* in_src, image* in_dst, int elem, int size) override;
	};


} // end namespace
} // end namespace
} // end namespace

#endif // MORPHOLOGICAL_H_87265B14_597C_49BB_9242_E17BFD9E8B24
