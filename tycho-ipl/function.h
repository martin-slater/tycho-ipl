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

#ifndef FUNCTION_H_0C41B4A0_6096_4F30_B5D6_24AE9889848D
#define FUNCTION_H_0C41B4A0_6096_4F30_B5D6_24AE9889848D

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include "key_value.h"
#include "program.h"
#include "utils.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	namespace validation
	{
		using ValidationFunc = std::function < bool(const value&) > ;

		/// Default validation function that just accepts all values
		inline bool no_validation(const value&) { return true; }

		inline bool odd_only_integer(const value& val, int max)
		{
			int v = val.get_integer();

			if (v == 0)
				return false;

			if ((v & 1) == 0)
				return false;

			if (v > max)
				return false;

			return true;
		}

		inline bool unit_float(const value& val)
		{
			float v = val.get_float();

			if (v < 0)
				return false;
			if (v > 1)
				return false;
			return true;
		}

		inline bool positive_integer(const value& val, int max = std::numeric_limits<int>::max())
		{
			int v = val.get_integer();
			
			if (v > max)
				return false;

			if (v < 0)
				return false;

			return true;
		}

		inline bool pow2_integer(const value& val)
		{
			int v = val.get_integer();

			return utils::num_bits_set(v) == 1;
		}

	}



	struct param_desc
	{
		param_desc() = default;

		param_desc(
			ObjectType type,
			const std::string& name, 
			const std::string& desc, 
			const value& def,
			validation::ValidationFunc vfunc = std::bind(validation::no_validation, std::placeholders::_1)) :
			Type(type),
			Name(name),
			Description(desc),
			DefaultVal(def),
			ValidationFunction(vfunc)
		{
		}

		param_desc(
			ObjectType type,
			const std::string& name,
			const std::string& desc) :
			Type(type),
			Name(name),
			Description(desc),
			ValidationFunction(std::bind(validation::no_validation, std::placeholders::_1))
		{
		}

		bool HasDefault() const { return DefaultVal.get_type() != ObjectType::Invalid;  }

		ObjectType Type;
		std::string Name;
		std::string Description;
		value		DefaultVal;
		validation::ValidationFunc ValidationFunction;
	};

	using param_list = std::vector < param_desc > ;

	//----------------------------------------------------------------------------
	// Function base class. 
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI function
	{
	public:
		static const param_desc DefaultInput();
		static const param_desc DefaultOutput();
		static const std::vector<param_desc> DefaultInputs();
		static const std::vector<param_desc> DefaultOutputs();
		static const std::vector<param_desc> NoOutputs();

		enum class Group
		{
			Support,
			Arithmetic,
			Filtering,
			Blending,
			EdgeDetection,
			ColorReduction,
			Structural,
			Leveling,
			Artistic
		};

	public:
		/// Constructor
		function(Group group, const char* name, const char* desc,
			const param_list& inputs,
			const param_list& outputs,
			const declaration_list& constants) :
			m_group(group),
			m_Name(name),
			m_Desc(desc),
			m_Inputs(inputs),
			m_Outputs(outputs),
			m_Constants(constants)
		{}


		/// Destructor
		virtual ~function() = default;
		
		virtual bool dispatch(context* ctx, const kv_dict& inputs, kv_dict& outputs) = 0;

		const param_list& get_inputs() const;
		const param_list& get_outputs() const;
		const declaration_list& get_constants() const;

		bool has_constants() const;
		bool has_input(const char*) const;
		bool has_output(const char*) const;

		bool has_param(const char*) const;
		bool get_param(const std::string& key, param_desc&);

		Group       get_group() const { return m_group; }
		std::string get_signature() const;
		std::string get_simple_signature() const;
		const char* get_name() const { return m_Name; }
		const char* get_description() const { return m_Desc; }

	protected:
		const Group m_group;
		const char* m_Name = nullptr;
		const char* m_Desc = nullptr;
		const param_list m_Inputs;
		const param_list m_Outputs;
		const declaration_list m_Constants;
	};
	
	
} // end namespace
} // end namespace

#endif // FUNCTION_H_0C41B4A0_6096_4F30_B5D6_24AE9889848D
