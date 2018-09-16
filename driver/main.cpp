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


#include <stdio.h>

#include "runtime/session_options.h"
#include "runtime/output_interface.h"
#include "runtime/simple_runner.h"
#include "runtime/experiment_runner.h"

#if defined(_DEBUG) && defined(_WIN32)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace tycho::image_processing::runtime;

int main(int argc, char* argv[])
{
#if defined(_DEBUG) && defined(_WIN32)
	_CrtMemState mem_start_state;
	_CrtMemCheckpoint(&mem_start_state);
#endif
	
	int result = EXIT_FAILURE;

	{
		std_output_interface output;
		std::vector<std::string> args;
		for (int i = 1; i < argc; ++i)
			args.push_back(argv[i]);

		session_options options(args);

		if (options.UnknownOptions.size() > 0)
		{
			for (auto opt : options.UnknownOptions)
			{
				fprintf(stderr, "Unknown option '%s'\n", opt.c_str());
			}
			return EXIT_FAILURE;
		}

		if (options.RunAction == session_options::action::ShowHelp)
		{
			options.print_help(output);
			result = EXIT_SUCCESS;
		}
		else if (options.RunAction == session_options::action::Invalid)
		{
			options.print_help(output);
			result = EXIT_FAILURE;
		}
		else if (options.RunAction == session_options::action::DumpFunctionTableMarkdown)
		{
			options.print_function_list_markdown(output);
			return EXIT_SUCCESS;
		}
		else if (options.RunAction == session_options::action::ShowFunctions)
		{
			options.print_function_list(output);
			result = EXIT_SUCCESS;
		}
		else
		{
			try
			{
				std::shared_ptr<runner> runner;
				if (options.RunAction == session_options::action::RunExperiment)
					runner = std::make_shared<experiment_runner>(options, &output);
				else
					runner = std::make_shared<simple_runner>(options, &output);
				runner->run();
				result = EXIT_SUCCESS;
			}
			catch (const std::exception& ex)
			{
				fprintf(stderr, "Error : %s\n", ex.what());
				result = EXIT_FAILURE;
			}
		}
	}

#if defined(_DEBUG) && defined(_WIN32)
	_CrtMemState mem_end_state;
	_CrtMemCheckpoint(&mem_end_state);
	_CrtMemState mem_diff;
	_CrtMemDifference(&mem_diff, &mem_start_state, &mem_end_state);
	_CrtMemDumpStatistics(&mem_diff);
#endif

	return result;
}

