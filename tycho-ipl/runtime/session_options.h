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

#ifndef SESSIONOPTIONS_H_389D9139_51CE_4BF1_B02D_C8E272F4A154
#define SESSIONOPTIONS_H_389D9139_51CE_4BF1_B02D_C8E272F4A154

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include "../exception.h"

#include <vector>
#include <string>
#include <map>

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
	// Raised when an invalid command line parameter is encountered
	//----------------------------------------------------------------------------
	class invalid_parameter : public runtime_exception
	{
	public:
		explicit invalid_parameter(const char *arg) :
			m_Arg(arg)
		{}

		explicit invalid_parameter(const std::string& arg) :
			m_Arg(arg)
		{}

		const char* what() const noexcept override
		{
			static std::array<char, 128> buf = { 0 };
			snprintf(&buf[0], buf.size(), "Unknown argument '%s'", m_Arg.c_str());
			return buf.data();
		}
		
	private:
		std::string m_Arg;
	};

	class invalid_response_file : public runtime_exception
	{
	public:
		invalid_response_file(const std::string& path) :
			m_path(path)
		{}

		const char* what() const noexcept override
		{
			static std::array<char, 128> buf = { 0 };
			sprintf(&buf[0], "Unable to read response file '%s'", m_path.c_str());
			return buf.data();
		}

	private:
		std::string m_path;
	};

	//----------------------------------------------------------------------------
	//  Runtime session options
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI session_options
	{		
	public:
		enum class action
		{
			Invalid,
			ShowHelp,
			ShowFunctions,
			DumpFunctionTableMarkdown,
			GenerateSphinxDocs,
			Run,
			RunExperiment
		};

	public:
		using file_list = std::vector < std::string > ;
		using input_map = std::map < std::string, std::string > ;

		explicit session_options(const std::vector<std::string>& args);
		session_options() = default;

		void print_help(output_interface& output) const;
		void print_function_list(output_interface& output) const;
		void print_function_list_markdown(output_interface& output) const;

		/**
		 * @brief Generate sphinx document tree for all functions
		 *
		 * This generates a .rst for each function with its description
		 * and function parameters.
		 *
		 * @warning This will overwrite any existing files in the directory.
		 * @param output_dir Directory to output the documentation to.
		 */
		int output_sphinx_function_docs(const std::string& output_dir) const;

		bool		MakeContactSheet = false;
		bool		LaunchResult = false;
		file_list    InputFiles;
		std::string PrefilterProgram;
		std::string SphinxOutputDir;
		std::string Program;
		input_map    ProgramInputs;
		std::string OutputDir;
		action		RunAction = action::Invalid;
		std::vector<std::string> UnknownOptions;
	};

	
} // end namespace
} // end namespace
} // end namespace

#endif // SESSIONOPTIONS_H_389D9139_51CE_4BF1_B02D_C8E272F4A154
