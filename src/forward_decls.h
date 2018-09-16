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

#ifndef FORWARD_DECLS_H_1DCD0825_9D20_454A_9619_02817B47CD13
#define FORWARD_DECLS_H_1DCD0825_9D20_454A_9619_02817B47CD13

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	class image;
	class function;
	class program;
	class context;
	struct key_value;
	struct statement;
	class declaration;
	class execution_interface;
	class ContactSheeet;
	class result_matrix;

	namespace functions
	{
		class factory;
		class greyscale;
	}

	namespace runtime
	{
		class output_interface;
		class runner;
		class experiment_runner;
		class simple_runner;
		class session_options;
	}

} // end namespace
} // end namespace


namespace cv
{

	class Mat;
	template<typename T> class Scalar_;
	typedef Scalar_<double> Scalar;
	template<typename T, int N> class Vec;
	typedef Vec<unsigned char, 3> Vec3b;

} // end namespace

#endif // FORWARD_DECLS_H_1DCD0825_9D20_454A_9619_02817B47CD13
