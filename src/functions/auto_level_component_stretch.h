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

#ifndef AUTOLEVELCOMPONENTSTRETCH_H_D8FF38E9_8674_48AD_9DB8_B143F5423688
#define AUTOLEVELCOMPONENTSTRETCH_H_D8FF38E9_8674_48AD_9DB8_B143F5423688

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include "../functions/nary_functions.h"

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
	// Auto levels image lighting by stretch each RGB component to use the full 
	// range.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI auto_level_component_stretch : public UnaryFunction
	{
	public:
		auto_level_component_stretch();
		void execute(image* src, image* dst) override;
	};


} // end namespace
} // end namespace
} // end namespace


#endif // AUTOLEVELCOMPONENTSTRETCH_H_D8FF38E9_8674_48AD_9DB8_B143F5423688
