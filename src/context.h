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

#ifndef CONTEXT_H_BC367D1D_A5C4_4C35_8B0E_7249287E4C9F
#define CONTEXT_H_BC367D1D_A5C4_4C35_8B0E_7249287E4C9F

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include "key_value.h"
#include <string>
#include <map>
#include <set>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{


	//----------------------------------------------------------------------------
	// Users derive from this class to handle interface functions which are 
	// called by client scripts during execution. 
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI execution_interface
	{
	public:
		/// Called by \ref functions::ExperimentAddImage
		virtual void add_experiment_image(image* image, const std::string& name) = 0;
	};

	//----------------------------------------------------------------------------
	// Execution context when running a filter program
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI context
	{
	public:
		/// Constructor
		context(const program*, execution_interface*);
		
		/// Destructor
		~context();

		/// Run the program
		bool execute(image* src, image*& dst, const kv_dict& inputs);

		/// Get the execution interface for this context
		execution_interface* GetExecutionInterface()
		{
			return m_Interface;
		}


	private:
		execution_interface* m_Interface;
		const program*	m_Program;
		kv_dict m_SymbolTable;
		std::set<image*> m_Allocated;

		// noncopyable
		context& operator=(const context&) = delete;
	};

	
} // end namespace
} // end namespace

#endif // CONTEXT_H_BC367D1D_A5C4_4C35_8B0E_7249287E4C9F
