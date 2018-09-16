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
#include "key_value.h"
#include <cstring>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	//----------------------------------------------------------------------------

	value value::from_string(const std::string& str)
	{
		auto type = deduce_object_type(str);

		if (type != ObjectType::Invalid)
		{
			switch (type)
			{
			case ObjectType::Float: { return make_float((float)atof(str.c_str())); }
			case ObjectType::Integer: { return make_integer(atoi(str.c_str())); }
			case ObjectType::Boolean: { return make_boolean((str == "true") ? true : false); }
			case ObjectType::Name: { return make_name(str); }
			case ObjectType::String:{ return make_string(str);  }
			
			default:
				break;
			}
		}
		return value();

	}

	//----------------------------------------------------------------------------

	const char* to_string(ObjectType type)
	{
		switch (type)
		{
		case ObjectType::Invalid: return "invalid";
		case ObjectType::Integer: return "int";
		case ObjectType::Float: return "float";
		case ObjectType::Boolean: return "bool";
		case ObjectType::Image: return "image";
		case ObjectType::Name: return "name";
		case ObjectType::String: return "string";
		}

		return "ERROR";
	};


	//----------------------------------------------------------------------------

	ObjectType value::deduce_object_type(const std::string& str)
	{
		size_t len = str.length();
		if (len == 0)
			return ObjectType::Invalid;

		// check to see if it is a string
		if (len > 2 && str[0] == '"' && str[len - 1] == '"')
			return ObjectType::String;

		// maybe a boolean
		if (strcmp(str.c_str(), "true") == 0 ||
			strcmp(str.c_str(), "false") == 0)
		{
			return ObjectType::Boolean;
		}

		// check for number
		if (str[0] == '+' || str[0] == '-' || isdigit(str[0]))
		{
			// nothing else cant start with '+' or '-' or digit so  must 
			// be a number otherwise invalid
			bool have_dot = false;
			for (size_t i = 1; i < len; ++i)
			{
				char ch = str[i];

				if (isdigit(ch))
					continue;

				else if (ch == '.')
				{
					if (have_dot)
						return ObjectType::Invalid;
					have_dot = true;
				}
				else
				{
					return ObjectType::Invalid;
				}
			}

			if (have_dot)
				return ObjectType::Float;
			else
				return ObjectType::Integer;
		}

		// not any of the primitive types so must be a name
		if (!(isalpha(str[0]) || str[0] == '_'))
			return ObjectType::Invalid;

		for (size_t i = 1; i < str.length(); ++i)
		{
			char ch = str[i];
			if (!(isalnum(ch) || ch == '-' || ch == '_'))
				return ObjectType::Invalid;
		}

		return ObjectType::Name;
	}


	//----------------------------------------------------------------------------

	std::string value::to_string() const
	{
		char buf[64] = { 0 };
		switch (m_Type)
		{
			case ObjectType::Boolean: {
				if (m_Value.Boolean)
					return "true";
				return "false";
 			}
								 
			case ObjectType::Float: {
				sprintf(buf, "%.3f", m_Value.Float);
			} break;

			case ObjectType::Integer: {
				sprintf(buf, "%d", m_Value.Integer);
			} break;
			case ObjectType::Name: {
				return m_Value.Name;
			}
			case ObjectType::Image: {
				sprintf(buf, "%p", m_Value.Image);
			} break;

			default:
				break;
		}
		return buf;
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
