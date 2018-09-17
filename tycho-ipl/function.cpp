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
#include "function.h"


//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	const param_desc function::DefaultInput()
	{
		param_desc def_in(ObjectType::Image, "src", "Source image", value());
		return def_in;
	}

	const param_desc function::DefaultOutput()
	{
		param_desc def_out(ObjectType::Image, "dst", "Destination image", value());
		return def_out;
	}

	const std::vector<param_desc> function::DefaultInputs()
	{
		param_list def_inputs = { DefaultInput() };
		return def_inputs;
	};

	const std::vector<param_desc> function::DefaultOutputs()
	{
		param_list def_outputs = { DefaultOutput() };
		return def_outputs;
	};

	const std::vector<param_desc> function::NoOutputs()
	{
		param_list no = {};
		return no;
	}

	//----------------------------------------------------------------------------

	std::string function::get_signature() const
	{
		std::string result(m_Name);

		result += "(";
		auto inputs = get_inputs();
		auto outputs = get_outputs();
		bool first = true;
		for (auto p : inputs)
		{
			if (!first)
				result += ", ";
			result += p.Name + "=" + p.DefaultVal.to_string();
			first = false;
		}
		result += ")";

		return result;
	}

	//----------------------------------------------------------------------------

	bool function::has_input(const char *name) const
	{
		for (auto p : get_inputs())
		{
			if (p.Name == name)
				return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	bool function::has_output(const char* name) const
	{
		for (auto p : get_outputs())
		{
			if (p.Name == name)
				return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	bool function::has_param(const char* name) const
	{
		return has_input(name) || has_output(name);
	}

	//----------------------------------------------------------------------------

	const declaration_list& function::get_constants() const
	{
		return m_Constants;
	}

	//----------------------------------------------------------------------------

	bool function::has_constants() const
	{
		return m_Constants.size() > 0;
	}

	//----------------------------------------------------------------------------

	const param_list& function::get_inputs() const
	{
		return m_Inputs;
	}

	//----------------------------------------------------------------------------

	const param_list& function::get_outputs() const
	{
		return m_Outputs;
	}

	//----------------------------------------------------------------------------

	bool function::get_param(const std::string& key, param_desc& desc)
	{
		auto& inputs = get_inputs();
		for (auto p : inputs)
		{
			if (p.Name == key)
			{
				desc = p;
				return true;
			}
		}
		auto& outputs = get_outputs();
		for (auto p : outputs)
		{
			if (p.Name == key)
			{
				desc = p;
				return true;
			}
		}
		return false;
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
