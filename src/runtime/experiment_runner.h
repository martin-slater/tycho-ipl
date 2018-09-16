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
#pragma once

#ifndef EXPERIMENTRUNNER_H_95877D65_95FB_4B3C_B626_091ABE7E6EA5
#define EXPERIMENTRUNNER_H_95877D65_95FB_4B3C_B626_091ABE7E6EA5

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include "../runtime/runner.h"
#include "../result_set.h"
#include "../image.h"

#include <vector>

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
	// Given a vector of input images and a matrix of input parameters generate
	// the matrix of output images.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI experiment_runner : public runner
	{
	public:
		/// Constructor
		experiment_runner(const session_options& options, output_interface* output);

		/// Conduct the experiment
		void run() override;
		
	private:
		void build_input_matrix(const session_options::input_map& raw_inputs);
		void run(const std::string& out_dir, program* program, image* source, int image_idx);
		void process_outputs(image_result_list& outputs, const std::string& out_dir, 
			const result_matrix::address& base_addr, const std::string& input_str, 
			const std::string& base_name);

	private:
		using value_list = std::vector < value > ;

		struct input_dim
		{
			input_dim(const std::string& name, const value_list& values) :
				Name(name),
				Values(values)
			{}

			std::string Name;
			value_list   Values;
		};

		//using InputDimList = std::vector < InputDim > ;
		using input_matrix = std::vector < input_dim > ;

		input_matrix m_InputMatrix;
		result_matrix m_OutputMatrix;
		std::string m_OutputDir;
		std::string m_ImageRootDir;
		std::string m_ContactSheetName;
	};

	
} // end namespace
} // end namespace
} // end namespace

#endif // EXPERIMENTRUNNER_H_95877D65_95FB_4B3C_B626_091ABE7E6EA5
