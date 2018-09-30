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
#include "utils.h"
#include "image.h"
#include <array>
#include <ctime>
#include <time.h>
#include <sstream>
#include <regex>

#ifdef _WIN32
#include <windows.h>
#include <ShlObj.h>
#endif // WIN32

#ifdef _MSC_VER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif 

namespace std_filesystem = std::experimental::filesystem;

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

	timer::timer()
	{
		m_startTime = m_clock.now();
	}

	//----------------------------------------------------------------------------
	
	double timer::elapsed() const
	{
		auto now = m_clock.now();
		return (double) std::chrono::duration_cast<std::chrono::seconds>(now - m_startTime).count();
	}

	//----------------------------------------------------------------------------

	std::string timer::elapsed_str_ms() const
	{
		auto elapsedMS = static_cast<int>(elapsed() * 1000);
		char buf[128];
		sprintf(buf, "%d", elapsedMS);
		return buf;
	}

	//----------------------------------------------------------------------------

	bool get_path_parts(const std::string& path, std::string& dir,
		std::string& name, std::string& ext)
	{
		auto cpath = std_filesystem::absolute(path);
		name = cpath.stem().string();
		dir = cpath.parent_path().string();		
		ext = cpath.extension().string();

		return true;
	}

	//----------------------------------------------------------------------------

	bool get_filename(const std::string& path, std::string& filename)
	{
		std::string dir, name, ext;
		if (get_path_parts(path, dir, name, ext))
		{
			filename = name;
			if (ext.length())
			{
				filename.append(ext);
			}
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	bool get_filename_no_ext(const std::string& path, std::string& filename)
	{
		std::string dir, name, ext;
		if (get_path_parts(path, dir, name, ext))
		{
			filename = name;
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	std::string get_absolute_path(const std::string& path)
	{
		std::string dir, name, ext;
		get_path_parts(path, dir, name, ext);
		auto res_path = std_filesystem::canonical(std_filesystem::absolute(dir));
		return (res_path / (name + ext)).string();
	}

	//----------------------------------------------------------------------------

	void shell_launch(const std::string& path)
	{
#ifdef _WIN32
		::ShellExecuteA(0, 0, path.c_str(), 0, 0, SW_SHOW);
#endif
	}

	//----------------------------------------------------------------------------

	std::string current_directory()
	{
		return std_filesystem::current_path().string();
	}

	//----------------------------------------------------------------------------

	bool create_directories(const std::string& dir)
	{
		std::error_code result;
		return std_filesystem::create_directories(dir, result);
	}

	//----------------------------------------------------------------------------

	std::string get_datetime_now_string()
	{
		time_t     now = time(nullptr);
		tm*  tstruct;
		char       buf[80];
		tstruct = localtime(&now);

		// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// for more information about date/time format
		std::strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S", tstruct);
		return buf;
	}

	//----------------------------------------------------------------------------

	std::vector<std::string> tokenize(const std::string &s, char delim)
	{
		std::stringstream ss(s);
		std::string item;
		std::vector<std::string> tokens;

		while (getline(ss, item, delim)) 
		{
			if(item.length() > 0)
				tokens.push_back(item);
		}

		return tokens;
	}

	//----------------------------------------------------------------------------

	size_t num_bits_set(int x)
	{
		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0F0F0F0F;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return x & 0x0000003F;
	}

	//----------------------------------------------------------------------------

	int int_log2(int val)
	{
		if (val == 0) return UINT_MAX;
		if (val == 1) return 0;

		int ret = 0;
		while (val > 1) {
			val >>= 1;
			ret++;
		}
		return ret;
	}

	//----------------------------------------------------------------------------

	bool glob_expand(const std::string& in_path, std::vector<std::string>& results, bool icase)
	{
		// escape all regex special chars
		std::string pattern;
		bool has_wildcard = false;
		for (auto& ch : in_path) {
			switch (ch)
			{
			case '\\': pattern.append("\\\\"); break;
			case '^': pattern.append("\\^"); break;
			case '.': pattern.append("\\."); break;
			case '$': pattern.append("\\$"); break;
			case '|': pattern.append("\\|"); break;
			case '(': pattern.append("\\("); break;
			case ')': pattern.append("\\)"); break;
			case '[': pattern.append("\\["); break;
			case ']': pattern.append("\\]"); break;
			case '*': pattern.append(".*"); has_wildcard = true; break;
			case '+': pattern.append("\\+"); break;
			case '?': pattern.append("."); has_wildcard = true; break;
			case '/': pattern.append("\\/"); break;
			case '{': pattern.append("\\{"); break;
			case '}': pattern.append("\\}"); break;

			default: 
				pattern.push_back(ch);
			}
		}
		
		if (!has_wildcard)
		{
			results.push_back(in_path);
			return true;
		}

		std::string dir, name, ext;
		if (!utils::get_path_parts(in_path, dir, name, ext))
		{
			return false;
		}
		std::regex::flag_type flags = std::regex_constants::ECMAScript;
		if (icase)
			flags |= std::regex_constants::icase;
		std::regex regex(pattern, flags);

		if (!std_filesystem::exists(dir))
		{
			return false;
		}

		for(auto& p: std_filesystem::directory_iterator(dir))
		{ 
			const std_filesystem::path& path = p;
			if (std_filesystem::is_regular_file(p))
			{
				if (std::regex_match(path.string(), regex))
				{
					results.push_back(path.string());
				}
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace

