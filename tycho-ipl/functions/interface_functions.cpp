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
#include "interface_functions.h"
#include "common.h"
#include "../image.h"
#include "../context.h"


//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace functions
{

	static const char Name[] = "experiment_add_image";
	static const char Desc[] = "Add an image to the list of images created by this run of an experiment";
	static const std::vector<param_desc> Inputs = {
		function::DefaultInput(),
		param_desc(ObjectType::String, "name", "Image filename", value()),
	};

	//----------------------------------------------------------------------------

	experiment_add_image::experiment_add_image() :
		function(Name, Desc, Inputs, function::NoOutputs(), declaration_list())
	{
	}


	//----------------------------------------------------------------------------

	bool experiment_add_image::dispatch(context* ctx, const kv_dict& inputs, kv_dict& /*outputs*/)
	{
		image* src = inputs.get_image("src");
		std::string name = inputs.get_string("name");
		
		if (ctx->GetExecutionInterface())
			ctx->GetExecutionInterface()->add_experiment_image(src, name);

		return true;
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
