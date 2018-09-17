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
#include "output_interface.h"
#include <array>


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

	void output_interface::write(const char* str, ...) const
	{
		va_list args;
		va_start(args, str);
		write_aux(false, false, str, args);
		va_end(args);
	}

	//----------------------------------------------------------------------------

	void output_interface::write_ln(const char* str, ...)  const
	{
		va_list args;
		va_start(args, str);
		write_aux(false, true, str, args);
		va_end(args);
	}

	//----------------------------------------------------------------------------

	void output_interface::error(const char* str, ...) const
	{
		va_list args;
		va_start(args, str);
		write_aux(true, false, str, args);
		va_end(args);
	}

	//----------------------------------------------------------------------------

	void output_interface::error_ln(const char* str, ...) const
	{
		va_list args;
		va_start(args, str);
		write_aux(true, true, str, args);
		va_end(args);
	}

	//----------------------------------------------------------------------------

	void output_interface::write_aux(bool error, bool line_end, const char* str, va_list args) const
	{
		std::array<char, 4096> buf;
		int c = vsprintf(&buf[0], str, args);

		if (line_end)
		{
			buf[c] = '\n';
			buf[c + 1] = 0;
		}

		if (error)
			do_error(buf.data());
		else
			do_write(buf.data());
	}


	//----------------------------------------------------------------------------

	std_output_interface::std_output_interface(FILE* out, FILE* err) :
		m_Out(out),
		m_Err(err)
	{}

	//----------------------------------------------------------------------------

	std_output_interface::~std_output_interface()
	{
		m_Err = m_Out = nullptr;
	}

	//----------------------------------------------------------------------------

	void std_output_interface::do_write(const char* str) const
	{
		fputs(str, m_Out);
	}

	//----------------------------------------------------------------------------

	void std_output_interface::do_error(const char* str) const
	{
		fputs(str, m_Err);
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace