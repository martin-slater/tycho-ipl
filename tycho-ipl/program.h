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

#ifndef PROGRAM_H_4B1F1E0F_D3A1_47AC_AA7B_A97DB1B2B061
#define PROGRAM_H_4B1F1E0F_D3A1_47AC_AA7B_A97DB1B2B061

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include "key_value.h"
#include "functions/factory.h"

#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{
	class program;

	class declaration
	{
	public:
		enum class TypeModifier
		{
			Invalid = 0,
			Input,
			Output,
			Constant,
			Temp
		};


		/// Constructor
		declaration(ObjectType type, TypeModifier mod, std::string name, value value, int decl_line) :
			Type(type),
			Modifier(mod),
			Name(name),
			Value(value),
			DeclLine(decl_line)
		{}

		/// Constructor
		declaration(ObjectType type, TypeModifier mod, std::string name, value value) :
			Type(type),
			Modifier(mod),
			Name(name),
			Value(value),
			DeclLine(-1)
		{}

		/// Constructor
		declaration(ObjectType type, TypeModifier mod, std::string name) :
			Type(type),
			Modifier(mod),
			Name(name),
			DeclLine(-1)
		{}

		declaration() = default;

		ObjectType		Type;
		TypeModifier	Modifier;
		std::string		Name;
		value			Value;

	private:
		friend program;
		int RefCount = 0;
		int DeclLine;
	};

	enum class StatementList
	{
		Invalid = 0,
		FunctionCall
	};

	
	struct statement
	{
		statement(StatementList type, const char* name, function* func, const kv_dict& args) :
			Type(type),
			Name(name),
			Func(func),
			Arguments(args)
		{}

		statement() = default;

		StatementList Type;
		std::string	  Name;
		function*	  Func;
		kv_dict		  Arguments;
	};

	struct program_error
	{
		std::string Message;
		int Line = -1;
		int Column = -1;
	};


	using declaration_list = std::list < declaration > ;
	using statement_list = std::list < statement > ;

	//----------------------------------------------------------------------------
	// Filter program
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI program
	{
	public:
		/// Default constructor
		program();
		
		/// Creates a program from file.
		/// Allocates a new program of the heap. User is responsible for freeing
		program(const char *path, size_t len, const char* source_path);

		/// Create from a program stored in a file
		static std::unique_ptr<program> create_from_file(const char *path);
		
		/// \returns True if the program contains errors
		bool has_errors() const  { return !m_Errors.empty(); }

		/// Print all errors and warnings to stdout
		void print_messages(runtime::output_interface& output) const;

		/// Return the input of the given name or nullptr if it doesn't exist
		const declaration* get_input(const std::string&name);

		/// Parse a value string
		bool parse_value(const std::string& str, std::vector<value>& out_value);

		/// Check that the value is of the correct type.
		bool type_check_or_coerce(ObjectType to_type, value& from_val);

		/// Check that the value is of the correct type.
		bool type_check_or_coerce(ObjectType to_type, std::vector<value>& from_vals);

		/// Returns the number time the passed function is called
		size_t get_call_count(const char* name) const;

		/// Returns the number time the passed function is called
		template<class T> size_t get_call_count() const
		{
			T t;
			return get_call_count(t.get_name());
		}

		/// Returns true if the program writes to the default destination image
		bool writes_to_default_dest() const;

		/// Returns the number of output images the program generates
		size_t num_output_images() const;

	private:

		using ErrorList = std::vector < program_error > ;
		using symbol_table = std::map < std::string, declaration* >;

		const char* consume_statement(const char* data, const char* end);
		const char* consume_declaration(declaration::TypeModifier type, declaration_list& dst_list, const char* data, const char* end);
		const char* consume_function_call(const char* data, const char* end);
		const char* consume_alpha_word(const char *data, const char* end, char *out_buffer, size_t buffer_len);
		const char* consume_whitespace(const char* data, const char* end);
		const char* consume_to_eol(const char* data, const char* end);
		const char* consume_kvdict(function* func, const char* data, const char* end, kv_dict& parts);
		const char* consume_to_char(const char* data, const char* end, char ch);
		const char* consume_value(const char *data, const char* end, value& out_value);
		const char* consume_value_checked(const char *data, const char* end, value& out_value);
		const char* consume_value_list(const char *data, const char* end, std::vector<value>& out_values);

		void print(runtime::output_interface& output, const char* title, const ErrorList& list) const;
		void log(ErrorList& list, int line, const char* fmt, va_list args);
		void log_error(int line, const char* msg, ...);
		void log_warning(int line, const char* msg, ...);
		void check_for_unused(const declaration_list&);
		void add_declaration(declaration_list& dst_list, const declaration& decl);
		void add_internal_declaration(declaration_list& dst_list, declaration decl);
		void set_function_constants(const function*);
		void add_constant_integer(const char* name, int val);

	private:
		functions::factory m_FunctionFactory;
		symbol_table     m_Symbols;
		declaration_list m_Inputs;
		declaration_list m_Outputs;
		declaration_list m_Constants;
		declaration_list m_Temporaries;
		statement_list   m_Statements;
		ErrorList		m_Errors;
		ErrorList		m_Warnings;

		// statistics
		using call_count_map = std::map < std::string, size_t > ;
		call_count_map m_CallCounts;

		// Parser state
		int m_CurLine = 0;
		const char* m_LastNewLine = nullptr;
		std::string m_SourcePath;

		friend context;
	};

	
} // end namespace
} // end namespace

#endif // PROGRAM_H_4B1F1E0F_D3A1_47AC_AA7B_A97DB1B2B061
