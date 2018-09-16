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

#ifndef UnaryFunction_H_8BE18A2D_FF41_42BD_940B_397374D148EF
#define UnaryFunction_H_8BE18A2D_FF41_42BD_940B_397374D148EF

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
	// Base class for functions that just take single input and output arguments 
	// (src, dst) of image type.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI UnaryFunction : public function
	{
	public:
		UnaryFunction(const char* name, const char* desc) :
			function(name, desc, function::DefaultInputs(), function::DefaultOutputs(), declaration_list())
		{}

		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs) override;
		virtual void execute(image* src, image* dst) = 0;
	};

	//----------------------------------------------------------------------------
	// Base class for functions that just take two inputs input and a single 
	// output arguments (src1, src2, dst) of image type.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI BinaryFunction : public function
	{
	public:
		BinaryFunction(const char* name, const char* desc);
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs) override;
		virtual void execute(image* src1, image* src2, image* dst) = 0;
	};

	//----------------------------------------------------------------------------
	// Base class for functions that just take two inputs input and a single 
	// output arguments (src1 * scale, src2, dst) of image type. 
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI ScaledBinaryFunction : public function
	{
	public:
		ScaledBinaryFunction(const char* name, const char* desc);
		bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs) override;
		virtual void execute(image* src1, float scale, image* src2, image* dst) = 0;
	};

#define IMAG_PROC_DECLARE_UNARY_FUNCTION(Name) \
	class TYCHO_IMAGEPROCESSING_ABI Name : public UnaryFunction \
					{ public: \
		Name (); \
		void execute(image* src, image* dst);	};

#define IMAG_PROC_DEFINE_UNARY_FUNCTION(Name, Desc, ClassName) \
	ClassName :: ClassName() : UnaryFunction(Name, Desc) {} \
	void ClassName::execute(image* src, image* dst)

#define IMAG_PROC_DECLARE_BINARY_FUNCTION(Name) \
	class TYCHO_IMAGEPROCESSING_ABI Name : public BinaryFunction \
			{ public: \
		Name (); \
		void execute(image* src1, image* src2, image* dst);	};

#define IMAG_PROC_DEFINE_BINARY_FUNCTION(Name, Desc, ClassName) \
	ClassName :: ClassName() : BinaryFunction(Name, Desc) {} \
	void ClassName::execute(image* src1, image* src2, image* dst)

#define IMAG_PROC_DECLARE_SCALED_BINARY_FUNCTION(Name) \
	class TYCHO_IMAGEPROCESSING_ABI Name : public ScaledBinaryFunction \
	{ public: \
		Name (); \
		void execute(image* src1, float scale, image* src2, image* dst);	};

#define IMAG_PROC_DEFINE_SCALED_BINARY_FUNCTION(Name, Desc, ClassName) \
	ClassName :: ClassName() : ScaledBinaryFunction(Name, Desc) {} \
	void ClassName::execute(image* src1, float scale, image* src2, image* dst)

} // end namespace
} // end namespace
} // end namespace

#endif // UnaryFunction_H_8BE18A2D_FF41_42BD_940B_397374D148EF
