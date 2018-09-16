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
#include "runner.h"
#include "../image.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace runtime
{

	//----------------------------------------------------------------------------

	runner::runner(const session_options& options, output_interface* output) :
		m_Options(options),
		m_Output(output)
	{
		
		if (options.PrefilterProgram.length())
			m_PrefilterProgram = load_program(options.PrefilterProgram);
			
		if (options.Program.length())
			m_Program = load_program(options.Program);
			
	}

	//----------------------------------------------------------------------------

	runner::~runner()
	{
		m_Output = nullptr;
	}

	//----------------------------------------------------------------------------

	std::unique_ptr<program> runner::load_program(const std::string& path) const
	{
		auto program = program::create_from_file(path.c_str());

		if (!program)
			throw program_load_error(path);

		program->print_messages(*m_Output);

		if (program->has_errors())
			throw program_load_error(path);

		return program;
	}

	//----------------------------------------------------------------------------

	image_ptr runner::load_image(const std::string& path) const
	{
		image_ptr img(new image(path.c_str()));

		if (m_PrefilterProgram)
		{
			context context(m_PrefilterProgram.get(), nullptr);
			image* dst = nullptr;
			context.execute(img.get(), dst, kv_dict());
			img = image_ptr(dst);
		}
		return img;
	}

	//----------------------------------------------------------------------------

	void runner::run(
		const program* program, image* source,
		const kv_dict& inputs, image_result_list& outputs)
	{
		context context(program, this);
		image* dst = nullptr;
		m_CurOutputs = &outputs;
		if (context.execute(source, dst, inputs) && dst)
		{
			m_CurOutputs->push_back(image_result(image_ptr(dst), ""));
		}
		m_CurOutputs = nullptr;
	}

	//----------------------------------------------------------------------------

	void runner::add_experiment_image(image* image, const std::string& name)
	{
		image_ptr copy(image->clone());
		m_CurOutputs->push_back(image_result(copy, name));
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
