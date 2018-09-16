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

#ifndef EXCEPTION_H_2F4DA3A5_ADD2_4642_9597_1F99529F9DD6
#define EXCEPTION_H_2F4DA3A5_ADD2_4642_9597_1F99529F9DD6

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include "key_value.h"
#include "function.h"
#include <exception>
#include <array>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{

	//----------------------------------------------------------------------------
	// Base class of all library exceptions
	//----------------------------------------------------------------------------
	class exception : public std::exception
	{
	public:
		exception() = default;
	};

	//----------------------------------------------------------------------------
	// Raised during program execution
	//----------------------------------------------------------------------------
	class runtime_exception : public exception
	{
	public:
	};

	//----------------------------------------------------------------------------
	// Symbol required by a function cannot be found
	//----------------------------------------------------------------------------
	class symbol_not_found : public runtime_exception
	{
	public:
		symbol_not_found(const char *name) :
			m_SymbolName(name)
		{}

		const char* what() const noexcept override
		{
			static std::array<char, 256> buffer;
			sprintf(
				&buffer[0],
				"Symbol '%s' cannot be found",
				m_SymbolName.c_str());

			return buffer.data();
		}

	private:
		std::string m_SymbolName;
	};

	//----------------------------------------------------------------------------
	// Invalid parameter passed to a function
	//----------------------------------------------------------------------------
	class invalid_parameter : public runtime_exception
	{
	public:
		invalid_parameter(const function* func, const param_desc& param, const value& value)
		{
			sprintf(
				&m_buffer[0],
				"Param '%s' to Function '%s' cannot be assigned '%s'",
				param.Name.c_str(), func->get_name(), value.to_string().c_str());
		}

		const char* what() const noexcept override
		{
			return m_buffer.data();			
		}

	private:
		std::array<char, 256> m_buffer;
	};
	

	class program_load_error : public runtime_exception
	{
	public:
		program_load_error(const std::string& path) :
			m_Path(path)
		{}

		const char* what() const noexcept override
		{
			static std::array<char, 256> buffer;
			sprintf(
				&buffer[0],
				"Error loading program '%s'",
				m_Path.c_str());
			buffer.data();
			return buffer.data();
		}

	private:
		std::string m_Path;
	};

} // end namespace
} // end namespace

#endif // EXCEPTION_H_2F4DA3A5_ADD2_4642_9597_1F99529F9DD6
