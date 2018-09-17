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
#include "context.h"
#include "program.h"
#include "function.h"
#include "image.h"
#include "exception.h"
#include <functional>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	//----------------------------------------------------------------------------

	context::context(const program* p, execution_interface* i) :
		m_Interface(i),
		m_Program(p)
	{

	}

	//----------------------------------------------------------------------------

	context::~context()
	{
		m_Program = nullptr;
		m_Interface = nullptr;
	}

	//----------------------------------------------------------------------------

	bool context::execute(image* src, image*& dst, const kv_dict& cinputs)
	{
		// set program constants
		for (auto c : m_Program->m_Constants)
		{
			m_SymbolTable.set(c.Name, c.Value);
		}

		// set program inputs
		for (auto c : m_Program->m_Inputs)
		{
			// check to see if this input default has been overridden
			value val;
			if (!cinputs.try_get(c.Name, val))
				val = c.Value;
			m_SymbolTable.set(c.Name, val);
		}

		// set global constants
		m_SymbolTable.set("__src__", value::make_image(src));
		m_SymbolTable.set("__width__", value::make_integer(src->get_width()));
		m_SymbolTable.set("__height__", value::make_integer(src->get_height()));

		// execute each statement in the program
		for (auto stmt : m_Program->m_Statements)
		{
			// resolve arguments
			kv_dict inputs, outputs;
			for (auto p : stmt.Func->get_inputs())
			{
				value val;
				if (stmt.Arguments.try_get(p.Name, val))
				{
					if (val.get_type() == ObjectType::Name)
					{
						value ref;
						if (m_SymbolTable.try_get(val.get_name(), ref))
						{
							// parameter validation
							if (!p.ValidationFunction(ref))
							{
								throw invalid_parameter(stmt.Func, p, ref);
							}
							inputs.set(p.Name, ref);
						}
						else
						{
							throw symbol_not_found(val.get_name());
						}
					}
					else
					{
						inputs.set(p.Name, val);
					}
				}
				else
				{
					// use default value
					inputs.set(p.Name, p.DefaultVal);
				}
			}

			stmt.Func->dispatch(this, inputs, outputs);

			for (auto p : stmt.Func->get_outputs())
			{
				value dst_arg;
				if (stmt.Arguments.try_get(p.Name, dst_arg))
				{
					value res;
					if (outputs.try_get(p.Name, res))
					{
						m_SymbolTable.set(dst_arg.get_name(), res);

						// track allocated images so we can clean up at the end
						if (res.get_type() == ObjectType::Image)
						{
							image* new_img = res.get_image();
							if (m_Allocated.find(new_img) == m_Allocated.end())
								m_Allocated.insert(new_img);
						}
					}
					else
						return false;
				}
				else
					return false;
			}
		}


		value dst_val;
		dst = nullptr;
		if (m_SymbolTable.try_get("__dst__", dst_val) && dst_val.get_type() == ObjectType::Image)
		{
			dst = dst_val.get_image();
		}

		// clean up
		for (auto img : m_Allocated)
		{
			if (img != dst)
				delete img;
		}
		m_Allocated.clear();


		return dst != nullptr;
	}

	//----------------------------------------------------------------------------

	
} // end namespace
} // end namespace
