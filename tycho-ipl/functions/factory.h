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

#ifndef FACTORY_H_9E06E49D_1825_4391_B129_0105F305462B
#define FACTORY_H_9E06E49D_1825_4391_B129_0105F305462B

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include <map>
#include <functional>

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
	// Factory for creating function object by name
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI factory
	{
	public:
		/// Default constructor
		factory();

		/// Destructor
		~factory();

		/// Create a new function object
		function* create(const std::string& name) const;

		/// Returns true if a function of the passed name exists
		bool exists(const std::string& name) const;

		// Enumerate functions
		void enumerate(std::function<void(function*)>);

		// Enumerate functions
		void enumerate(std::function<void(const function*)>) const;

	private:
		using FMap = std::map < std::string, function* > ;

		FMap m_functions;

		factory operator=(const factory&) = delete;

		template<class T> void add_func()
		{
			auto* t = new T();
			m_functions.insert(std::make_pair(t->get_name(), t));
		}

		template<class T> void add_func(const char* name)
		{
			auto* t = new T();
			m_functions.insert(std::make_pair(name, t));
		}
	};
	
} // end namespace
} // end namespace
} // end namespace

#endif // FACTORY_H_9E06E49D_1825_4391_B129_0105F305462B
