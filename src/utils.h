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

#ifndef UTILS_H_A9006198_22B1_40CD_9C42_98A7D819DE82
#define UTILS_H_A9006198_22B1_40CD_9C42_98A7D819DE82

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include <string>
#include <vector>
#include <chrono>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{
namespace utils
{
	//----------------------------------------------------------------------------
	// Simple RAII wrapper around a FILE handle
	//----------------------------------------------------------------------------
	class file_handle 
	{
	public:
		file_handle(const char* path, const char* flags)
		{
			m_file = fopen(path, flags);
		}

		~file_handle()
		{
			if (m_file)
			{
				fclose(m_file);
				m_file = nullptr;
			}
		}

		bool ok() const 
		{
			return m_file != nullptr;
		}

		std::string read_all() const
		{
			fseek(m_file, 0L, SEEK_END);
			size_t len = ftell(m_file);
			fseek(m_file, 0L, SEEK_SET);

			std::string result;
			result.resize(len);
			fread(&result[0], len, 1, m_file);
			return result;
		}
	private:
		FILE* m_file = nullptr;
		void operator=(const file_handle&) = delete;		
	};

	//----------------------------------------------------------------------------
	// Simple timer
	//----------------------------------------------------------------------------
	class timer
	{
	public:
		/// constructor
		timer();

		/// \returns the elapsed time in seconds
		double elapsed() const;

		/// \returns the elapsed time in milliseconds as a string
		std::string elapsed_str_ms() const;
			
	private:
		std::chrono::high_resolution_clock m_clock;
		std::chrono::high_resolution_clock::time_point m_startTime;
	};


	//----------------------------------------------------------------------------
	// Split a file path into directory, name and extension parts.
	//----------------------------------------------------------------------------
	bool get_path_parts(const std::string& path, std::string& dir, 
		std::string& name, std::string& ext);

	//----------------------------------------------------------------------------
	// Get the filename part of a path
	//----------------------------------------------------------------------------
	bool get_filename(const std::string& path, std::string& filename);

	//----------------------------------------------------------------------------
	// Get the filename part of a path without the extension
	//----------------------------------------------------------------------------
	bool get_filename_no_ext(const std::string& path, std::string& filename);

	//----------------------------------------------------------------------------
	// Get the absolute path for the passed path name
	//----------------------------------------------------------------------------
	std::string get_absolute_path(const std::string& path);

	//----------------------------------------------------------------------------
	// Execute the passed file using the windows shell to choose the correct 
	// application
	//----------------------------------------------------------------------------
	void shell_launch(const std::string& path);

	//----------------------------------------------------------------------------
	// Tokenize a string
	//----------------------------------------------------------------------------
	std::vector<std::string> tokenize(const std::string& str, char delim);

	//----------------------------------------------------------------------------
	// Get the current working directory
	//----------------------------------------------------------------------------
	std::string current_directory();

	//----------------------------------------------------------------------------
	// Creates all directories in the given path
	//----------------------------------------------------------------------------
	bool create_directories(const std::string& dir);

	//----------------------------------------------------------------------------
	// Returns the current data and time in the format YYYY_MM_DD_HH_MM_SS
	//----------------------------------------------------------------------------
	std::string get_datetime_now_string();

	//----------------------------------------------------------------------------
	// Returns the number of bits set in the passed value
	//----------------------------------------------------------------------------
	size_t num_bits_set(int x);

	//----------------------------------------------------------------------------
	// Return the base 2 logarithm of an integer
	//----------------------------------------------------------------------------
	int int_log2(int val);

	//----------------------------------------------------------------------------
	// Take a file path and expands and if it contains wildcard's (? or *) will
	// search the directory for all matches. Wildcard's are only supported in the
	// filename. i.e.
	//   /foo/bar/*
	//   /foo/bar/*.*
	//   /foo/bar/*.jpg
	//   /foo/bar/bob.*
	//----------------------------------------------------------------------------
	bool glob_expand(const std::string& path, std::vector<std::string>& results, bool icase);

} // end namespace
} // end namespace
} // end namespace

#endif // UTILS_H_A9006198_22B1_40CD_9C42_98A7D819DE82
