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

#ifndef OUTPUTINTERFACE_H_3604F16C_FDB9_4F77_A209_44498F2C59F4
#define OUTPUTINTERFACE_H_3604F16C_FDB9_4F77_A209_44498F2C59F4

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include <cstdarg>
#include <cstdio>

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
	// Interface for writing text output.
	//----------------------------------------------------------------------------
	class output_interface
	{
	public:
		void write(const char* str, ...) const;
		void write_ln(const char* str, ...)  const;
		void error(const char* str, ...) const;
		void error_ln(const char* str, ...) const;

	protected:
		virtual void do_write(const char* str) const = 0;
		virtual void do_error(const char* str) const = 0;

	private:
		void write_aux(bool error, bool line_end, const char* str, va_list args) const;
	};

	//----------------------------------------------------------------------------
	// Interface for writing text output to FILE* handles. By default these are
	// stdout and stderr.
	//----------------------------------------------------------------------------
	class std_output_interface : public output_interface
	{
	public:
		std_output_interface(FILE* out = stdout, FILE* err = stderr);
		~std_output_interface();
		
	private:
		void do_write(const char* str) const override;
		void do_error(const char* str) const override;

	private:
		FILE* m_Out;
		FILE* m_Err;
	};


	
} // end namespace
} // end namespace
} // end namespace

#endif // OUTPUTINTERFACE_H_3604F16C_FDB9_4F77_A209_44498F2C59F4
