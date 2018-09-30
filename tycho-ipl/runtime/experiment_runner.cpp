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
#include "experiment_runner.h"
#include "../variation_iterator.h"
#include "../image.h"
#include "../utils.h"
#include "../contact_sheet.h"
#include "../functions/interface_functions.h"

namespace std_filesystem = std::experimental::filesystem;

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

	experiment_runner::experiment_runner(const session_options& options, output_interface* output) :
		runner(options, output),
		m_OutputDir(options.OutputDir)
	{
		namespace fs = std_filesystem;

		build_input_matrix(options.ProgramInputs);

		// calculate the number of images the program will write
		size_t num_outputs = get_program()->num_output_images();

		// create the result matrix, first dimension is input image followed by the
		// input arguments
		std::vector<size_t> output_matrix_dims;
		std::vector<std::string> output_dims_names;
		output_matrix_dims.push_back(options.InputFiles.size());
		output_dims_names.emplace_back("Images");
		for (auto input : m_InputMatrix)
		{
			output_matrix_dims.push_back(input.Values.size());
		}
		output_matrix_dims.push_back(num_outputs);
		
		m_OutputMatrix = result_matrix(output_matrix_dims, output_dims_names);

		// setup directories
		std::string program_name;
		utils::get_filename_no_ext(options.Program, program_name);
		auto now_str = utils::get_datetime_now_string();
		auto root_dir = m_OutputDir;
		m_OutputDir = m_OutputDir + program_name + "-" + now_str + "/";
		fs::create_directory(m_OutputDir, root_dir);
		m_ImageRootDir = m_OutputDir + "Images/";
		fs::create_directory(m_ImageRootDir, m_OutputDir);

		// create contact sheet name
		{
			std::string sheet_path(m_OutputDir);
			sheet_path.append(program_name);
			sheet_path.append("-contact_sheet-");
			sheet_path.append(utils::get_datetime_now_string());
			sheet_path.append(".jpg");

			m_ContactSheetName = sheet_path;
		}
	}

	//----------------------------------------------------------------------------

	void experiment_runner::build_input_matrix(const session_options::input_map& raw_inputs)
	{
		// now have a program convert raw inputs into typed inputs	
		auto program = get_program();
		for (auto raw : raw_inputs)
		{
			auto decl = program->get_input(raw.first);
			if (decl)
			{
				value_list values;
				program->parse_value(raw.second, values);
				if (program->type_check_or_coerce(decl->Type, values))
				{
					m_InputMatrix.push_back(input_dim(raw.first, values));
				}
				else
				{
					get_output()->error("Input '%s' takes type '%s' but was passed '%s'\n",
						raw.first.c_str(),
						to_string(decl->Type),
						raw.second.c_str());
				}
			}
			else
			{
				get_output()->error("Warning : Param '%s' is not used\n", raw.first.c_str());
			}
		}
	}

	//----------------------------------------------------------------------------

	void experiment_runner::run()
	{
		auto program = get_program();
		int image_idx = 0;
		for (auto image_path : get_input_files())
		{
			get_output()->write_ln("Processing : %s", image_path.c_str());
			image_ptr image = load_image(image_path);
			if (image)
			{
				// create output directory for this image 
				std::string filename;
				utils::get_filename(image_path, filename);
				for(char& ch : filename) 
				{
					if (ch == '.')
						ch = '_';
				}
				std_filesystem::path out_dir(m_ImageRootDir);
				out_dir /= filename;		 
				std_filesystem::create_directory(out_dir, m_ImageRootDir);

				// run the experiment
				image_ptr copy(image->clone());
				run(out_dir, program, copy.get(), image_idx++);
			}	
		}

		if(m_OutputMatrix.num_dimensions() > 2)
			m_OutputMatrix.trim_single_dimension();

		// write out contact sheets 
		contact_sheet sheet;
		std::string title("Title");
		sheet.auto_build(title, m_OutputMatrix, m_ContactSheetName, get_input_files());

		if (launch_result())
			utils::shell_launch(m_ContactSheetName.data());
	}


	//----------------------------------------------------------------------------

	void experiment_runner::run(const std::experimental::filesystem::path& out_dir, program* program,  image* source, int image_idx)
	{
		// split source path into parts
		std::string dir, name, ext;
		utils::get_path_parts(source->get_source_path(), dir, name, ext);

		// and loop over all variations of the inputs
		if (m_InputMatrix.size())
		{
			std::vector<int> depths;
			for (auto vlist : m_InputMatrix)
				depths.push_back(vlist.Values.size());
			variation_iterator input_it(depths);

			std::vector<int> cur_state;
			while (input_it.next(cur_state))
			{
				// build the input state for this iteration
				kv_dict inputs;
				int cur_param = 0;
				std::string input_str("(");
				for (auto vlist : m_InputMatrix)
				{
					const value& v = vlist.Values[cur_state[cur_param]];
					inputs.set(vlist.Name, v);
					if (cur_param)
						input_str.append(", ");
					input_str += vlist.Name + std::string("=") + v.to_string();
					++cur_param;
				}
				input_str.append(")");

				get_output()->write("%s ... ", input_str.c_str());

				image_result_list outputs;
				utils::timer timer;
				runner::run(program, source, inputs, outputs);
				input_str.append(std::string(" in ") + timer.elapsed_str_ms() + "ms");
				get_output()->write("done\n");

				// write all output images to disk
				result_matrix::address base_addr;
				base_addr.push_back(image_idx);
				base_addr.insert(base_addr.end(), cur_state.begin(), cur_state.end());
				process_outputs(outputs, out_dir, base_addr, input_str, name);
			}
		}
		else
		{
			image_result_list outputs;
			runner::run(program, source, kv_dict(), outputs);

			// write all output images to disk
			result_matrix::address base_addr;
			base_addr.push_back(image_idx);
			process_outputs(outputs, out_dir, base_addr, "", name);
		}
	}

	//----------------------------------------------------------------------------

	void experiment_runner::process_outputs(image_result_list& outputs, 
		const std::experimental::filesystem::path& out_dir, const result_matrix::address& base_addr,
		const std::string& input_str, const std::string& base_name)
	{
		// write all output images to disk and add to result matrix
		size_t cur_output = 0;
		for (auto image : outputs)
		{
			std_filesystem::path dst_path(out_dir);
			std::string filename = (base_name + "_" + input_str + "_" + image.Annotation);
			for(auto& ch : filename)
			{
				if(!(std::isalpha(ch) || std::isdigit(ch)))
					ch = '_';
			}
			filename += ".png";
			dst_path /= filename;
			image.Image->write_to_file(dst_path.string());

			std::shared_ptr<file_list_node> file_list = std::make_shared<file_list_node>();
			file_list->add_path(dst_path.string(), input_str + image.Annotation);

			// add to the output matrix
			result_matrix::address node_addr(base_addr);
			node_addr.push_back(cur_output);
			m_OutputMatrix.get_node(node_addr) = file_list;

			++cur_output;
		}
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
