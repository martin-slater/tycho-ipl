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
#include "program.h"
#include "functions/common.h"
#include "function.h"
#include "image.h"
#include "runtime/output_interface.h"
#include "functions/interface_functions.h"

#include <cstdio>
#include <cassert>
#include <cstdarg>
#include <array>
#include <memory>

#define CHECK_UNEXPECTED_END() \
	if(data == end) { \
		log_error(m_CurLine, "Unexpected end of file"); \
		return data; \
	}

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	//----------------------------------------------------------------------------

	std::unique_ptr<program> program::create_from_file(const char* path)
	{
		FILE *file = fopen(path, "rb");
		std::unique_ptr<program> p;

		if (file)
		{
			fseek(file, 0L, SEEK_END);
			size_t len = ftell(file);
			fseek(file, 0L, SEEK_SET);
			std::vector<char> data;
			data.resize(len);
			if (fread(&data[0], len, 1, file))
				p = std::make_unique<program>(&data[0], len, path);
			fclose(file);
		}
		return p;
	}

	//----------------------------------------------------------------------------

	program::program(const char *str, size_t len, const char* source_path) :
		m_SourcePath(source_path)
	{
		// setup implicit constants
		add_declaration(m_Inputs, declaration(ObjectType::Image, declaration::TypeModifier::Input, "__src__"));
		add_declaration(m_Outputs, declaration(ObjectType::Image, declaration::TypeModifier::Output, "__dst__"));
		add_internal_declaration(m_Constants, declaration(ObjectType::Integer, declaration::TypeModifier::Constant, "__width__"));
		add_internal_declaration(m_Constants, declaration(ObjectType::Integer, declaration::TypeModifier::Constant, "__height__"));

		// add function constants
		m_FunctionFactory.enumerate(
			std::bind(&program::SetFunctionConstants, this, std::placeholders::_1));

		// add some built-in constants
		add_constant_integer("FORMAT_RGB", (int)image::Format::RGB);
		add_constant_integer("FORMAT_LAB", (int)image::Format::Lab);
		add_constant_integer("FORMAT_HLS", (int)image::Format::HLS);
		add_constant_integer("FORMAT_HSV", (int)image::Format::HSV);
		add_constant_integer("FORMAT_GREY", (int)image::Format::Grey);

		// morphological structuring elements
		add_constant_integer("MORPH_RECT", cv::MORPH_RECT);
		add_constant_integer("MORPH_CROSS", cv::MORPH_CROSS);
		add_constant_integer("MORPH_ELLIPSE", cv::MORPH_ELLIPSE);


		// configure call count map
		m_FunctionFactory.enumerate(
			[&](const function*f) 
			{ 
				m_CallCounts.insert(std::make_pair(f->get_name(), 0)); 
			});

		// Parse the actual program
		const char* cur_pos = str;
		const char* end = cur_pos + len;
		m_LastNewLine = cur_pos;
		while (cur_pos < end && *cur_pos)
		{
			cur_pos = consume_whitespace(cur_pos, end);
			if (cur_pos < end && *cur_pos)
				cur_pos = consume_statement(cur_pos, end);
		}

		// Report unused declarations
		check_for_unused(m_Inputs);
		check_for_unused(m_Outputs);
		check_for_unused(m_Constants);
		check_for_unused(m_Temporaries);
	}

	//----------------------------------------------------------------------------

	bool program::parse_value(const std::string& str, std::vector<value>& out_values)
	{
		consume_value_list(&str[0], &str[str.length()], out_values);

		for (auto v : out_values)
		{
			if (v.get_type() == ObjectType::Invalid)
				return false;
		}
		return true;
	}

	//----------------------------------------------------------------------------

	void program::add_internal_declaration(declaration_list& dst_list, declaration decl)
	{
		// increase ref count of internal declarations so they do not get reported as unused
		++decl.RefCount;
		add_declaration(dst_list, decl);
	}

	//----------------------------------------------------------------------------

	void program::add_declaration(declaration_list& dst_list, const declaration& decl)
	{
		dst_list.push_back(decl);
		m_Symbols.insert(std::make_pair(decl.Name, &dst_list.back()));
	}

	//----------------------------------------------------------------------------

	void program::check_for_unused(const declaration_list& list)
	{
		for (auto decl : list)
		{
			if (decl.RefCount == 0)
			{
				log_warning(decl.DeclLine, "Declaration '%s' is unused", decl.Name.c_str());
			}
		}
	}

	//----------------------------------------------------------------------------

	const declaration* program::get_input(const std::string& name)
	{
		auto it = m_Symbols.find(name);
		if (it != m_Symbols.end() &&
			it->second->Modifier == declaration::TypeModifier::Input)
		{
			return it->second;
		}
		return nullptr;
	}



	//----------------------------------------------------------------------------

	const char* program::consume_value_list(
		const char *data, const char* end, std::vector<value>& out_values)
	{
		data = consume_whitespace(data, end);

		while (1)
		{
			value value;
			data = consume_value(data, end, value);
			out_values.push_back(value);
			data = consume_whitespace(data, end);
			if (*data == ',')
			{
				++data;
				data = consume_whitespace(data, end);
			}
			else
			{
				break;
			}
		}

		return data;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_value(
		const char *data, const char* end, value& out_value)
	{
		std::array<char, 128> buffer;

		char ch = *data;
		if (ch == '\"')
		{
			// string
			++data;
			auto it = buffer.begin();
			while (data < end && *data != '\"' && it < buffer.end())
				*it++ = *data++;

			if (*data != '\"')
				log_error(m_CurLine, "String not terminated");
			else
			{
				*it = 0;
				out_value = value::make_string(buffer.data());
				++data;
			}
		}
		else if ((ch >= '0' && ch <= '9') || ch == '-' || ch == '+')
		{
			// nothing else cant start with '+' or '-' or digit so must 
			// be a number otherwise invalid
			auto it = buffer.begin();
			*it++ = *data++;
			bool have_dot = false;
			bool have_digit = isdigit(ch) != 0;
			while (data < end)
			{
				char ach = *data;

				if (ach == '.')
				{
					if (!have_digit)
					{
						log_error(m_CurLine, "Number must start with a digit");
						out_value = value();
						return ++data;
					}
					else if (have_dot)
					{
						log_error(m_CurLine, "Invalid character '.' parsing number");
						out_value = value();
						return ++data;
					}
					else
					{
						have_dot = true;
						*it++ = ach;
						++data;
					}
				}
				else if (isdigit(ach))
				{
					*it++ = ach;
					have_digit = true;
					++data;
				}
				else
				{
					break;
				}
			}

			// terminate the string
			*it = 0;

			if (have_dot)
				out_value = value::make_float((float)atof(buffer.data()));
			else
				out_value = value::make_integer(atoi(buffer.data()));
		}
		else if (isalpha(ch) || ch == '_' || ch == '-' || ch == '.')
		{
			// name or boolean
			data = consume_alpha_word(data, end, &buffer[0], buffer.size());

			if (strcmp(&buffer[0], "true") == 0)
			{
				out_value = value::make_boolean(true);
			}
			else if (strcmp(&buffer[0], "false") == 0)
			{
				out_value = value::make_boolean(false);
			}
			else
			{
				out_value = value::make_name(buffer.data());
			}
		}

		return data;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_value_checked(
		const char *data, const char* end, value& out_value)
	{
		data = consume_value(data, end, out_value);
		if (out_value.get_type() == ObjectType::Name)
		{
			auto it = m_Symbols.find(out_value.get_name());
			if (it == m_Symbols.end())
			{
				log_error(m_CurLine, "Use of undefined name '%s'", out_value.get_name());
				out_value = value();
				return data;
			}

			++it->second->RefCount;
		}

		return data;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_alpha_word(
		const char *data, const char* end, char *out_buffer, size_t buffer_len)
	{
		size_t cur = 0;
		while (data < end)
		{
			char c = *data;
			if (isalnum(c) || c == '-' || c == '_' || c == '.')
			{
				out_buffer[cur++] = c;
				assert(cur < buffer_len);
				++data;
			}
			else
				break;
		}

		// null terminate
		out_buffer[cur] = 0;

		return consume_whitespace(data, end);
	}

	//----------------------------------------------------------------------------

	const char* program::consume_whitespace(const char* data, const char* end)
	{
		while (data < end)
		{
			if (*data == '\n')
			{
				++data;
				m_LastNewLine = data;
				++m_CurLine;
			}
			else if (isspace(*data))
				++data;
			else if (*data == '#')
				data = consume_to_eol(data, end);
			else
				break;
		}
		return data;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_to_eol(const char* data, const char* end)
	{
		while (data < end && *data != '\n')
			++data;

		return data;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_statement(const char* data, const char* end)
	{
		// check first word for the statement type
		std::array<char, 64> type = { 0 };
		data = consume_whitespace(data, end);
		data = consume_alpha_word(data, end, &type[0], type.size());

		if (strcmp(type.data(), "const") == 0)
			data = consume_declaration(declaration::TypeModifier::Constant, m_Constants, data, end);
		else if (strcmp(type.data(), "input") == 0)
			data = consume_declaration(declaration::TypeModifier::Input, m_Inputs, data, end);
		else if (strcmp(type.data(), "output") == 0)
			data = consume_declaration(declaration::TypeModifier::Output, m_Outputs, data, end);
		else if (strcmp(type.data(), "temp") == 0)
			data = consume_declaration(declaration::TypeModifier::Temp, m_Temporaries, data, end);
		else if (strcmp(type.data(), "call") == 0)
			data = consume_function_call(data, end);
		else
		{
			log_error(m_CurLine, "Invalid statement '%s'", type.data());
			return consume_to_char(data, end, ';') + 1;
		}

		data = consume_whitespace(data, end);

		if (*data != ';')
		{
			log_error(m_CurLine, "Statement not terminated");
			return consume_to_char(data, end, ';') + 1;
		}
		++data;

		return data;
	}

	//----------------------------------------------------------------------------

	bool program::type_check_or_coerce(ObjectType to_type, std::vector<value>& from_vals)
	{
		bool ok = true;
		for (auto& v : from_vals)
		{
			ok = type_check_or_coerce(to_type, v) && ok;
		}

		return ok;
	}

	//----------------------------------------------------------------------------

	bool program::type_check_or_coerce(ObjectType to_type, value& from_val)
	{
		auto from_type = from_val.get_type();
		if (from_type != to_type)
		{
			// names can convert to anything 
			if (from_type == ObjectType::Name)
			{
				// ok
			}
			// coerce ints to floats.
			else if (to_type == ObjectType::Float && from_type == ObjectType::Integer)
			{
				from_val = value::make_float((float)from_val.get_integer());
			}
			else
			{
				log_error(m_CurLine, "Incorrect type, cannot convert '%s' to '%s'", to_string(from_type), to_string(to_type));
				return  false;
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_kvdict(
		function* func,
		const char* data,
		const char* end,
		kv_dict& parts)

	{
		// all args are of the form
		//	  key=value

		std::array<char, 128> key = { 0 };
		std::array<char, 128> value_str = { 0 };
		while (data < end)
		{
			// get key 
			data = consume_whitespace(data, end);
			data = consume_alpha_word(data, end, &key[0], key.size());

			if (*data != '=')
			{
				// Error, log and try to eat until next comma or semi-colon 
				// so we can carry on parsing and collect more errors
				log_error(m_CurLine, "Unassigned keyword '%s'", key.data());
				while (data < end)
				{
					if (*data == ';')
						return data;

					if (*data == ',')
					{
						++data;
						break;
					}

					++data;
				}
				continue;
			}

			// consume '='
			++data;

			// consume value
			data = consume_whitespace(data, end);

			value value;
			data = consume_value_checked(data, end, value);

			// check key is used by the function
			param_desc desc;
			if (!func->get_param(key.data(), desc))
			{
				log_error(m_CurLine, "Function '%s' does not take argument '%s'", func->get_name(), key.data());
			}
			else if (value.is_valid())
			{
				// type checking
				if (type_check_or_coerce(desc.Type, value))
					parts.set(key.data(), value);
			}

			if (*data == ';')
				return data;

			if (*data == ')')
			{
				++data;
				return data;
			}
			if (*data != ',')
			{
				log_error(m_CurLine, "Syntax error. Unexpected '%c'", *data);
				return consume_to_char(data, end, ';');
			}
			++data;
		}
		return data;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_to_char(const char* data, const char* end, char ch)
	{
		while (data < end && *data != ch)
			++data;
		return data;
	}

	//----------------------------------------------------------------------------

	const char* program::consume_function_call(const char* data, const char* end)
	{
		data = consume_whitespace(data, end);
		std::array<char, 64> name = { 0 };
		data = consume_alpha_word(data, end, &name[0], name.size());

		if (*data == ';')
		{
			log_error(m_CurLine, "Function '%s' specifies no parameters", name.data());
			return data;
		}
		if (*data != '(')
		{
			log_error(m_CurLine, "Expected '('. Function has no argument list.");
			return consume_to_char(data, end, ';');
		}
		++data;
		data = consume_whitespace(data, end);


		auto func = m_FunctionFactory.create(name.data());
		if (func)
		{
			kv_dict args;
			data = consume_kvdict(func, data, end, args);
			data = consume_whitespace(data, end);

			if (*data != ';')
			{
				log_error(m_CurLine, "Statement not terminated");
				return data;
			}
			m_Statements.push_back(statement(StatementList::FunctionCall, name.data(), func, args));
			m_CallCounts[func->get_name()]++;
		}
		else
		{

			log_error(m_CurLine, "Undefined function '%s'", name.data());
			data = consume_to_char(data, end, ';');
		}

		return data;
	}


	//----------------------------------------------------------------------------

	const char* program::consume_declaration(
		declaration::TypeModifier type_mod,  declaration_list& dst_list, 
		const char* data, const char* end)
	{
		std::array<char, 128> type = { 0 };

		data = consume_whitespace(data, end);
		data = consume_alpha_word(data, end, type.data(), type.size());

		ObjectType obj_type;

		if (strcmp(type.data(), "int") == 0)
			obj_type = ObjectType::Integer;
		else if (strcmp(type.data(), "float") == 0)
			obj_type = ObjectType::Float;
		else if (strcmp(type.data(), "string") == 0)
			obj_type = ObjectType::String;
		else if (strcmp(type.data(), "image") == 0)
			obj_type = ObjectType::Image;
		else if (strcmp(type.data(), "int") == 0)
			obj_type = ObjectType::Integer;
		else
		{
			log_error(m_CurLine, "Invalid type '%s'", type.data());
			return data;
		}

		// may be multiple comma delimited declarations
		do
		{
			std::array<char, 128> name = { 0 };
			data = consume_alpha_word(data, end, name.data(), name.size());

			value value;
			if (*data == '=')
			{
				++data;
				data = consume_whitespace(data, end);
				std::array<char, 128> value_str = { 0 };
				data = consume_value_checked(data, end, value);

				// type check the value matches the declaration type 
				type_check_or_coerce(obj_type, value);
			}


			if (m_Symbols.find(name.data()) != m_Symbols.end())
			{
				log_error(m_CurLine, "Multiple definitions of '%s'", name.data());
				return data;
			}
			else
			{
				add_declaration(dst_list, declaration(obj_type, type_mod, name.data(), value, m_CurLine));
			}

			data = consume_whitespace(data, end);
			CHECK_UNEXPECTED_END();
			if (*data == ';')
				break;
			else if (*data == ',')
			{
				++data;
				data = consume_whitespace(data, end);
				continue;
			}
			else
			{
				log_error(m_CurLine, "Syntax error. Encountered unexpected character '%c'", *data);
				return consume_to_char(data, end, ';');
			}

		} while (1);

		return data;
	}

	//----------------------------------------------------------------------------

	void program::log_warning(int line, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		log(m_Warnings, line, fmt, args);
		va_end(args);
	}

	void program::log_error(int line, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		log(m_Errors, line, fmt, args);
		va_end(args);
	}

	void program::log(ErrorList& list, int line, const char* fmt, va_list args )
	{
		program_error error;
		error.Line = line;
		std::array<char, 512> buffer = { 0 };
		vsnprintf(buffer.data(), buffer.size(), fmt, args);
		error.Message = buffer.data();
		list.push_back(error);
	}


	
	//----------------------------------------------------------------------------

	void program::print_messages(runtime::output_interface& output) const
	{
		print(output, "Error", m_Errors);
		print(output, "Warning", m_Warnings);

		output.write_ln("%d Errors, %d Warnings", (int)m_Errors.size(), (int)m_Warnings.size());
	}

	//----------------------------------------------------------------------------

	void program::print(runtime::output_interface& output, const char* title, const ErrorList& list) const
	{
		for (auto error : list)
		{
			output.error_ln(
				"%s : %s(%d) : %s\n",
				title,
				m_SourcePath.c_str(), 
				error.Line + 1, error.Message.c_str());
		}
	}

	//----------------------------------------------------------------------------

	void program::SetFunctionConstants(const function* func)
	{
		if (func->has_constants())
		{
			for (auto c : func->get_constants())
			{
				add_internal_declaration(m_Constants, c);
			}
		}
	}

	//----------------------------------------------------------------------------

	void program::add_constant_integer(const char* name, int val)
	{
		add_internal_declaration(m_Constants,
			declaration(ObjectType::Integer, 
			declaration::TypeModifier::Constant, 
			name, value::make_integer(val)));

	}

	//----------------------------------------------------------------------------

	size_t program::get_call_count(const char* name) const
	{
		size_t n = 0;
		auto it = m_CallCounts.find(name);
		if (it != m_CallCounts.end())
			n = it->second;
		return n;
	}

	//----------------------------------------------------------------------------

	bool program::writes_to_default_dest() const
	{
		for (auto decl : m_Outputs)
		{
			if (decl.Name == "__dst__" && decl.RefCount > 0)
				return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------

	size_t program::num_output_images() const
	{
		size_t num_outputs = get_call_count<functions::experiment_add_image>();
		if (writes_to_default_dest())
			++num_outputs;

		return num_outputs;
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
