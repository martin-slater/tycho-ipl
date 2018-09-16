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

#ifndef BLENDFUNCTIONS_H_9DE54B7A_921E_45DE_9499_254BC40AC3C6
#define BLENDFUNCTIONS_H_9DE54B7A_921E_45DE_9499_254BC40AC3C6

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

	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_normal);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_lighten);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_darken);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_multiply);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_average);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_add);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_subtract);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_difference);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_negation);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_screen);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_exclusion);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_overlay);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_soft_light);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_hard_light);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_color_dodge);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_color_burn);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_linear_dodge);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_linear_burn);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_linear_light);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_vivid_light);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_pin_light);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_hard_mix);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_reflect);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_glow);
	IMAG_PROC_DECLARE_BINARY_FUNCTION(blend_phoenix);


} // end namespace
} // end namespace
} // end namespace

#endif // BLENDFUNCTIONS_H_9DE54B7A_921E_45DE_9499_254BC40AC3C6
