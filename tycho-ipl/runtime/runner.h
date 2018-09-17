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
#ifdef MSC_VER
#pragma once
#endif  // _MSC_VER

#ifndef RUNNER_H_EE0C8C00_F366_4ED7_9D93_98D0B89D478D
#define RUNNER_H_EE0C8C00_F366_4ED7_9D93_98D0B89D478D

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include "../context.h"
#include "../image.h"
#include "../runtime/session_options.h"
#include "../runtime/output_interface.h"


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
	// Base runner
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI runner : public execution_interface
	{
	public:
		struct image_result
		{
			image_result(image_ptr image, const std::string& ann) :
				Image(image),
				Annotation(ann)
			{}
				
			image_ptr Image;
			std::string Annotation;
		};

		using image_result_list = std::list<image_result>;

	public:
		/// Constructor
		runner(const session_options& options, output_interface* output);

		/// Destructor
		~runner();

		/// Execute
		virtual void run() = 0;

	protected:
		std::unique_ptr<program> load_program(const std::string& path) const;
		image_ptr load_image(const std::string& path) const;
		
		const program* get_program() const { return m_Program.get(); }
		program* get_program() { return m_Program.get(); }

		const output_interface* get_output() const { return m_Output; }

		const session_options::file_list& get_input_files() const { return m_Options.InputFiles; }
		bool launch_result() const { return m_Options.LaunchResult;  }
 
		void run(const program* program, image* source,
			const kv_dict& inputs,
			image_result_list& outputs);

		// ExecutionInterface
		void add_experiment_image(image* image, const std::string& name) override;


	private:
		session_options			 m_Options;
		std::unique_ptr<program> m_Program;
		std::unique_ptr<program> m_PrefilterProgram;
		output_interface*		 m_Output;
		image_result_list*		 m_CurOutputs;
	};

	
} // end namespace
} // end namespace
} // end namespace

#endif // RUNNER_H_EE0C8C00_F366_4ED7_9D93_98D0B89D478D
