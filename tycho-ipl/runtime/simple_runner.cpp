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
#include "simple_runner.h"
#include "../image.h"
#include "../utils.h"

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

	simple_runner::simple_runner(const session_options& options, output_interface* output) :
		runner(options, output)
	{

	}

	//----------------------------------------------------------------------------
	
	void simple_runner::run()
	{
		auto program = get_program();
		for (auto image_path : get_input_files())
		{
			get_output()->write_ln("Processing : %s", image_path.c_str());
			image_ptr image = load_image(image_path);

			if (image)
			{
				image_result_list outputs;
				runner::run(program, image.get(), kv_dict() /* m_Options.ProgramInputs*/, outputs);

				// write the outputs to disk
				std::string dir, name, ext;
				utils::get_path_parts(image_path, dir, name, ext);
				int idx = 0;
				for (auto img : outputs)
				{
					std::array<char, 128> dst_path;
					sprintf(
						&dst_path[0],
						"%s_%s.png", name.c_str(), img.Annotation.c_str());

					img.Image->write_to_file(dst_path.data());

					if (launch_result())
						utils::shell_launch(dst_path.data());

					++idx;
				}
			}
		}
	}

	//----------------------------------------------------------------------------


} // end namespace
} // end namespace
} // end namespace
