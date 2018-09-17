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


//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "nary_functions.h"
#include "../context.h"
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
	//----------------------------------------------------------------------------

	bool UnaryFunction::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		image* dst = src->clone();
		execute(src, dst);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	static const std::vector<param_desc> BinaryFuncInputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Image, "src1", "First source image", value()),
		param_desc(ObjectType::Image, "src2", "Second source image", value())
	};


	BinaryFunction::BinaryFunction(const char* name, const char* desc) :
		function(
			name, desc, 
			BinaryFuncInputs, 
			function::DefaultOutputs(), declaration_list())
	{}

	
	//----------------------------------------------------------------------------

	bool BinaryFunction::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src1 = inputs.get_image("src1");
		image* src2 = inputs.get_image("src2");
		image* dst = src1->clone();
		execute(src1, src2, dst);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	static const std::vector<param_desc> ScaledBinaryFuncInputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Image, "src1", "First source image", value()),
		param_desc(ObjectType::Image, "src2", "Second source image", value()),
		param_desc(ObjectType::Float, "scale", "Scale factor applied to src1", value::make_float(1))
	};

	ScaledBinaryFunction::ScaledBinaryFunction(const char* name, const char* desc) :
		function(
		name, desc,
		ScaledBinaryFuncInputs,
		function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool ScaledBinaryFunction::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src1 = inputs.get_image("src1");
		image* src2 = inputs.get_image("src2");
		image* dst = src1->clone();
		float scale = inputs.get_float("scale");
		execute(src1, scale, src2, dst);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
