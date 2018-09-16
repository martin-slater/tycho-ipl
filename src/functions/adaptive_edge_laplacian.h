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

#ifndef ADAPTIVEEDGELAPLACIAN_H_3BBF2159_3D1E_4B52_9032_0116E9B60743
#define ADAPTIVEEDGELAPLACIAN_H_3BBF2159_3D1E_4B52_9032_0116E9B60743

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
	// Calculate edges using laplacian filter. This version will search the filter
	// input space to find the result that is closest to supplied edge ratio. This
	// is the number of pixels detected as edges over the image area.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI adaptive_edge_laplacian : public function
	{
	public:
		adaptive_edge_laplacian();
		bool dispatch(context* ctx, const kv_dict& kwargs, kv_dict& outputs) override;
		virtual void execute(
			image* src, image* dst, 
			int edge_percent, int lower_cutoff, 
			bool invert, bool apply_adaptive_cutoff);
	};

	
} // end namespace
} // end namespace
} // end namespace

#endif // ADAPTIVEEDGELAPLACIAN_H_3BBF2159_3D1E_4B52_9032_0116E9B60743
