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
#include "session_options.h"
#include "output_interface.h"
#include "../utils.h"

#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include <algorithm>
#include <regex>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{
namespace runtime
{

	namespace detail
	{
		static std::vector<std::string> SupportExtensions = {
			".bmp",
			".dib",
			".jpg",
			".jpeg",
			".jpe",
			".jp2",
			".png",
			".pbm",
			".pgm",
			".sr",
			".ras",
			".tiff",
			".tif"
		};


		bool is_supported_extension(const std::string& path)
		{
			std::string dir, name, ext;
			utils::get_path_parts(path, dir, name, ext);
			std::transform(ext.begin(), ext.end(), ext.begin(), [](char ch) { return static_cast<char>(::tolower(ch)); });

			return std::find(SupportExtensions.begin(), SupportExtensions.end(), ext) != SupportExtensions.end();
		}

		//----------------------------------------------------------------------------

		static bool split_arg(const char *arg, std::string& key, std::string& val)
		{
			const char* eq = strchr(arg, '=');
			if (eq)
			{
				key = std::string(arg, eq);
				val = std::string(eq + 1);
			}
			else
			{
				key = arg;
				val.clear();
			}

			return val.size() > 0;
		}

	} // end namespace


	//----------------------------------------------------------------------------

	session_options::session_options(const std::vector<std::string>& in_args)
	{
		bool is_experiment = false;
		std::vector<std::string> args;

		// output directory defaults to cwd
		OutputDir = utils::current_directory();

		// user can specify a single response file parameter of the form @<file_path>.
		// This file must contain the command line on a single line.
		if (in_args.size() == 1 && in_args[0].length() > 0 && in_args[0][0] == '@')
		{
			std::string path(&in_args[0][1], in_args[0].length() - 1);
			utils::file_handle file(path.c_str(), "rt");
			if (!file.ok())
			{
				throw invalid_response_file(path);
			}

			auto cmd_line = file.read_all();
			args = utils::tokenize(cmd_line, ' ');
		}
		else
		{
			args = in_args;
		}


		// eat arguments starting with '--'. 
		size_t narg = 0;
		for (; narg < args.size(); ++narg)
		{
			const std::string& arg = args[narg];
			std::string key, val;
			bool has_val = detail::split_arg(arg.c_str(), key, val);
			bool is_switch = key.size() >= 2 && key[0] == '-' && key[1] == '-';

			if (!is_switch)
				break;

			key = key.substr(2, std::string::npos);
			if (key == "output_dir")
			{
				if (!has_val)
					throw invalid_parameter("--output_dir : no directory specified");
				OutputDir = val;
			}
			else if (key == "launch")
			{
				LaunchResult = true;
			}
			else if (key == "contact")
			{
				MakeContactSheet = true;
			}
			else if (key == "prefilter")
			{
				if (!has_val)
					throw invalid_parameter("--prefilter : no program specified");

				PrefilterProgram = val;
			}
			else if (key == "functions_md")
			{
				RunAction = action::DumpFunctionTableMarkdown;
			}		
			else if (key == "functions")
			{
				RunAction = action::ShowFunctions;
			}
			else if (key == "help")
			{
				RunAction = action::ShowHelp;
			}
			else if (key == "experiment")
			{
				is_experiment = true;
			}
			else
			{
				UnknownOptions.push_back(arg);
			}
		}

		// stop processing if we are just showing some help
		if (RunAction != action::Invalid)
			return;

		// key=val pairs from here specify program input parameters
		while (narg < args.size())
		{
			std::string key, val;
			if (detail::split_arg(args[narg].c_str(), key, val))
				ProgramInputs.insert(std::make_pair(key, val));
			else
				break;
			++narg;
		}

		// program to execute
		if (narg < args.size())
		{
			Program = args[narg++];

			bool case_sensitive_glob = true;
#ifdef _MSC_VER
			case_sensitive_glob = false;
#endif

			// and lastly the input image file to process
			while (narg < args.size())
			{
				std::vector<std::string> globbed;
#ifdef _MSC_VER
				// on windows convert any forward slashes to back slashes
				auto path = args[narg];
				for (char& ch : path)
				{
					if (ch == '/')
						ch = '\\';
				}
#else
				auto path = args[narg];
#endif
				auto fullpath = utils::get_absolute_path(path);
				if (!utils::glob_expand(fullpath, globbed, case_sensitive_glob))
				{
					fprintf(stderr, "Failed to expand input '%s'\n", args[narg].c_str());
					return;
				}
				for (auto arg : globbed)
				{
					if (detail::is_supported_extension(arg))
						InputFiles.push_back(arg);
					else
						fprintf(stderr, "Ignoring '%s' : unrecognized extension\n", arg.c_str());
				}
				++narg;
			}

			if (InputFiles.size() > 0)
				RunAction = action::Run;
		}


		if (RunAction == action::Run)
		{
			// see if we have any inputs greater than length 1, if so we go into experiment mode
			if(!is_experiment)
			{
				for (auto input : ProgramInputs)
				{
					if (input.first.find(',') != std::string::npos)
					{
						is_experiment = true;
						break;
					}
				}
			}
			if (is_experiment)
				RunAction = action::RunExperiment;
		}

		// check output directory ends in slash
		OutputDir = utils::get_absolute_path(OutputDir);
		if (OutputDir.back() != '\\' && OutputDir.back() != '/')
			OutputDir += "/";
	}

	//----------------------------------------------------------------------------

	void session_options::print_help(output_interface& output) const
	{
		output.error(
			"Driver program to run Tycho IPL scripts on a set of images for a given range of parameters.\n"
			"You can supply filter script input parameters on the command line as a comma separated list\n"
			"and it will run the script repeatedly for each of the parameters.\n"
			"You can also specify a response file using @<filename> as a single command line parameters and\n"
			"it will read the command line options from there instead\n"
			"\n"
			"Usage : \n"
			"    filter.exe --help             : Show help\n"
			"    filter.exe --functions		   : Show help for all functions\n"
			"    filter.exe --help <function>  : Show help for a specific function\n"
			"    filter.exe <options> <parameters> <input-program> <image-path>\n"
			"\n"
			"Options : \n"
			"    --prefilter=<program>    : Optional program to run on image before running main program\n"
			"    --launch                 : Display the resulting image\n"
			"    --output_dir=<dir>       : Directory to save the result in\n"
			"    --experiment             : Run an experiment\n"
			"    --contact                : Create a contact sheet for result images\n"
			"\n"
			"Parameters : \n"
			"    Filter script parameters can be passed using the form <key>=<value>. The value can be a\n"
			"    single element or a comma delimited list. If it is a list then the script will be execute\n"
			"    once for each element.\n"
			"\nCopyright (c) 2018 Martin A Slater (mslater@hellinc.net)\n"
		);
	}

	//----------------------------------------------------------------------------

	void session_options::print_function_list_markdown(output_interface& output) const
	{
		output.write_ln("| Function | Description |");
		output.write_ln("|----------|-------------|");

		auto fp = [&output](function* func) {
			output.write_ln("|%s|%s|", func->get_name(), func->get_description());
		};

		functions::factory factory;
		factory.enumerate(std::bind(fp, std::placeholders::_1));
	}

	//----------------------------------------------------------------------------

	void session_options::print_function_list(output_interface& output) const
	{
		auto pp = [&output](const char* title, const param_list& params)
		{
			if (params.size())
			{
				for (auto p : params)
					output.write_ln(" <%s>\t%s\t: %s", title, p.Name.c_str(), p.Description.c_str());
			}
		};

		auto fp = [&output, pp](function* func) {
			output.write_ln("----------------------------------------");
			output.write_ln("%s\n", func->get_description());
			output.write_ln("%s", func->get_signature().c_str());
			output.write_ln("----------------------------------------");
			pp("in", func->get_inputs());
			pp("out", func->get_outputs());
			output.write_ln("");
		};

		functions::factory factory;
		factory.enumerate(std::bind(fp, std::placeholders::_1));
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
