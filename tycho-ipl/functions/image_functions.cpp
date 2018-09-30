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
#include "image_functions.h"
#include "common.h"
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
	// ImageLoad
	//----------------------------------------------------------------------------

	static const std::vector<param_desc> LoadInputs = {
		param_desc(ObjectType::String, "path", "Path to image file.")
	};

	//----------------------------------------------------------------------------

	image_load::image_load() :
		function(
		Group::Support,
		"load_image",
		"Load an image to disk",
		LoadInputs, function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool image_load::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		std::string path = inputs.get_string("path");
		auto* dst = new image(path.c_str());
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------
	// ImageSave
	//----------------------------------------------------------------------------

	static const std::vector<param_desc> SaveInputs = {
		function::DefaultInput(),
		param_desc(ObjectType::String, "path", "Path to write image to.")
	};

	//----------------------------------------------------------------------------

	image_save::image_save() :
		function(
		Group::Support,
		"save_image",
		"Write image to disk",
		SaveInputs, function::NoOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool image_save::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict&)
	{
		image* src = inputs.get_image("src");
		std::string path = inputs.get_string("path");
		src->write_to_file(path.c_str());
		return true;
	}


	//----------------------------------------------------------------------------
	// ImageConvert
	//----------------------------------------------------------------------------

	static const std::vector<param_desc> ConvertInputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "format", "One of FORMAT_RGB, FORMAT_LAB, FORMAT_HSV, FORMAT_HLS, FORMAT_GREY"),
	};

	//----------------------------------------------------------------------------

	image_convert::image_convert() :
		function(
			Group::Support,
			"convert_image",
			"Convert an image to a different format",
			ConvertInputs, function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool image_convert::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int format = inputs.get_integer("format");
		image* dst = src->clone();
		execute(src, dst, (image::Format)format);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void image_convert::execute(image* in_src, image* in_dst, image::Format format)
	{
		IMAGE_PROC_ASSERT(in_src);
		IMAGE_PROC_ASSERT(in_dst);

		in_src->convert_to(in_dst, format);
	}

	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// ImageClampSize
	//----------------------------------------------------------------------------

	static const std::vector<param_desc> ClampSizeInputs = {
		function::DefaultInput(),
		param_desc(ObjectType::Integer, "size", "Size in pixels to clamp longest edge to."),
		param_desc(ObjectType::Boolean, "enlarge", "True to scale smaller images to the given size.", value::make_boolean(false))
	};

	//----------------------------------------------------------------------------

	image_clamp_size::image_clamp_size() :
		function(
		Group::Support,
		"clamp_image_size",
		"Clamp an image to a given size whilst maintaining its aspect ratio",
		ClampSizeInputs, function::DefaultOutputs(), declaration_list())
	{}


	//----------------------------------------------------------------------------

	bool image_clamp_size::dispatch(context* /*ctx*/, const kv_dict& inputs, kv_dict& outputs)
	{
		image* src = inputs.get_image("src");
		int size = inputs.get_integer("size");
		bool enlarge = inputs.get_boolean("enlarge");
		image* dst = src->clone();
		execute(src, dst, size, enlarge);
		outputs.set_image("dst", dst);
		return true;
	}

	//----------------------------------------------------------------------------

	void image_clamp_size::execute(image* in_src, image* in_dst, int max_size, bool enlarge)
	{
		using namespace cv;

		IMAGE_PROC_ASSERT(in_src);
		IMAGE_PROC_ASSERT(in_dst);

		in_src->clamp_size(in_dst, max_size, enlarge, image::Interpolation::Cubic);
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
