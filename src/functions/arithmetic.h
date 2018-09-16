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

#ifndef ADD_H_B0D7F0A9_AA66_4157_A941_B49817CCC283
#define ADD_H_B0D7F0A9_AA66_4157_A941_B49817CCC283

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


	IMAG_PROC_DECLARE_BINARY_FUNCTION(add)
	IMAG_PROC_DECLARE_BINARY_FUNCTION(subtract)
	IMAG_PROC_DECLARE_BINARY_FUNCTION(multiply)
	IMAG_PROC_DECLARE_BINARY_FUNCTION(divide)
	IMAG_PROC_DECLARE_SCALED_BINARY_FUNCTION(scaled_multiply)
	IMAG_PROC_DECLARE_SCALED_BINARY_FUNCTION(scaled_add)
	IMAG_PROC_DECLARE_BINARY_FUNCTION(bitwise_and)
	IMAG_PROC_DECLARE_BINARY_FUNCTION(bitwise_or)
	IMAG_PROC_DECLARE_BINARY_FUNCTION(bitwise_xor)
	IMAG_PROC_DECLARE_UNARY_FUNCTION(max)
	IMAG_PROC_DECLARE_UNARY_FUNCTION(min)

} // end namespace
} // end namespace
} // end namespace

#endif // ADD_H_B0D7F0A9_AA66_4157_A941_B49817CCC283
