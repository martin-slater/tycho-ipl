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

#ifndef KEYVALUE_H_2537E56C_F53F_489D_B351_346F4F299856
#define KEYVALUE_H_2537E56C_F53F_489D_B351_346F4F299856

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <stdlib.h>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{

	enum class ObjectType
	{
		Invalid = 0,
		Integer,
		Float,
		Boolean,
		Image,
		Name,
		String
	};

	const char* to_string(ObjectType type);

	class value
	{
	public:
		static const int MaxName = 128;

		static ObjectType deduce_object_type(const std::string&);
		static value from_string(const std::string&);

		value() = default;
		
		static value make_name(const std::string& str)
		{ 
			value_ v;
			strcpy(&v.Name[0], str.c_str()); 
			return value(v, ObjectType::Name);
		}

		static value make_string(const std::string& str)
		{
			value_ v;
			strcpy(&v.Name[0], str.c_str());
			return value(v, ObjectType::String);
		}

#define MAKE_FUNC(_obj_type, _type, _name) \
		static value make_##_name(_type val) \
 		{ \
			value_ v; \
			v._obj_type = val; \
			return value(v, ObjectType::_obj_type); \
		}

		MAKE_FUNC(Integer, int, integer)
		MAKE_FUNC(Boolean, bool, boolean)
		MAKE_FUNC(Image, image*, image)
		MAKE_FUNC(Float, float, float)

#undef MAKE_FUNC

		void set_image(image* img) { m_Value.Image = img; m_Type = ObjectType::Image; }
		void set_float(float val) { m_Value.Float = val; m_Type = ObjectType::Float; }
		void set_name(const char* name) 
		{ 
			strcpy(&m_Value.Name[0], name);
			m_Type = ObjectType::Name;
		}

		void set_string(const char* name)
		{
			strcpy(&m_Value.Name[0], name);
			m_Type = ObjectType::String;
		}

		void set_boolean(bool val) { m_Value.Boolean = val; m_Type = ObjectType::Boolean; }
		void set_integer(int val) { m_Value.Integer = val; m_Type = ObjectType::Integer; }

		ObjectType get_type() const { return m_Type;  }
		bool is_valid() const { return m_Type != ObjectType::Invalid; }

		const image* get_image() const  { IMAGE_PROC_ASSERT(m_Type == ObjectType::Image); return m_Value.Image; }
		image* get_image() { IMAGE_PROC_ASSERT(m_Type == ObjectType::Image); return m_Value.Image; }
		int get_integer() const { IMAGE_PROC_ASSERT(m_Type == ObjectType::Integer); return m_Value.Integer; }
		float get_float() const { IMAGE_PROC_ASSERT(m_Type == ObjectType::Float); return m_Value.Float; }
		bool get_boolean() const { IMAGE_PROC_ASSERT(m_Type == ObjectType::Boolean); return m_Value.Boolean; }
		const char* get_name() const { IMAGE_PROC_ASSERT(m_Type == ObjectType::Name); return m_Value.Name; }
		const char* get_string() const { IMAGE_PROC_ASSERT(m_Type == ObjectType::String); return m_Value.Name; }
		std::string to_string() const;

	private:
		union value_
		{
			image* Image;
			float  Float;
			int    Integer;
			bool   Boolean;
			char   Name[MaxName];
		};

		value(value_ value, ObjectType type) :
			m_Value(value),
			m_Type(type)
		{}

		value_ m_Value;
		ObjectType m_Type = ObjectType::Invalid;
	};

	struct key_value
	{
		key_value(const char* key, const char* val) :
			Key(key),
			Value(val)
		{}

		key_value() = default;

		std::string Key;
		std::string Value;
	};

	using kv_list = std::vector < key_value >;

	class kv_dict
	{
	public:

		bool exists(const std::string& key) const
		{
			return m_dict.find(key) != m_dict.end();
		}

		void set(const std::string& key, const value& value)
		{
			auto it = m_dict.find(key);
			if (it != m_dict.end())
				it->second = value;
			else
				m_dict.insert(std::make_pair(key, value));
		}
		
		bool try_get(const std::string &key, value& value) const 
		{
			auto it = m_dict.find(key);
			if (it == m_dict.end())
				return false;

			value = it->second;
			return true;
		}

#define DEFINE_GET_METHODS(_t0, _t1) \
		bool try_get_##_t0(const std::string& key, _t1& out_value) const { \
			value v; if (try_get(key, v)) { \
				out_value = v.get_##_t0(); \
				return true; } \
			return false; } \
		_t1 get_##_t0(const std::string& key) const { \
			value v; try_get(key, v); return v.get_##_t0();} \
		void set_##_t0(const std::string& name, _t1 val) { \
		    set(name, value::make_##_t0(val)); }

		DEFINE_GET_METHODS(name, std::string);
		DEFINE_GET_METHODS(string, std::string);
		DEFINE_GET_METHODS(float, float);
		DEFINE_GET_METHODS(integer, int);
		DEFINE_GET_METHODS(image, image*);
		DEFINE_GET_METHODS(boolean, bool);

#undef DEFINE_GET_METHODS

	private:
		std::map<std::string, value> m_dict;
	};
	
} // end namespace
} // end namespace

#endif // KEYVALUE_H_2537E56C_F53F_489D_B351_346F4F299856
