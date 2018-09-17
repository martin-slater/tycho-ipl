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

#ifndef RESULTSET_H_6AA0F334_FE39_45C4_A3F9_96E155382A81
#define RESULTSET_H_6AA0F334_FE39_45C4_A3F9_96E155382A81

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include <string>
#include <vector>
#include <memory>
#include <stdarg.h>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{

	//----------------------------------------------------------------------------
	// Base of all nodes stored in the result matrix
	//----------------------------------------------------------------------------
	class node_base
	{
	public:
		enum class Type
		{
			Matrix,
			FileList
		};

		node_base(Type t) : m_Type(t) {}
		virtual ~node_base() = default;

		Type get_type() const { return m_Type;  }

		void* set_user_data(void* d)
		{
			void* cur = m_UserData;
			m_UserData = d;
			return cur;
		}

		const void* get_user_data() const
		{
			return m_UserData;
		}

	private:
		Type m_Type;
		void* m_UserData;
	};

	using NodeBasePtr = std::shared_ptr < node_base > ;

	//----------------------------------------------------------------------------
	// Leaf nodes contain a list of file paths
	//----------------------------------------------------------------------------
	class file_list_node : public node_base
	{
	public:
		struct path_entry
		{
			path_entry(const std::string& p, const std::string& a) :
				Path(p),
				Annotation(a)
			{}

			std::string Path;
			std::string Annotation;
		};

		using FileList = std::vector < path_entry > ;

	public:
		file_list_node() : node_base(Type::FileList)
		{}

		void add_path(const std::string& p, const std::string& a)
		{
			m_Paths.push_back(path_entry(p, a));
		}

		const FileList& get_entries() const { return m_Paths; }

	private:
		FileList m_Paths;
	};


	//----------------------------------------------------------------------------
	// Dynamic multi dimensional matrix of nodes. This is used at the root of the tree
	// and below for non leaf nodes.
	//----------------------------------------------------------------------------
	class result_matrix : public node_base
	{
	public:
		using address = std::vector < size_t >;
		using node_type = NodeBasePtr;
		using const_node_type = const node_type;

	public:
		result_matrix() : node_base(Type::Matrix) {}

		result_matrix(const std::vector<size_t>& dimensions, 
					 const std::vector<std::string>& names) :
			node_base(Type::Matrix),
			m_Dimensions(dimensions),
			m_Names(names)
		{
			if (dimensions.size() > 0)
			{
				int num_nodes = dimensions[0];
				for (size_t d = 1; d < dimensions.size(); ++d)
					num_nodes *= dimensions[d];

				m_Elements.resize(num_nodes, nullptr);
			}
		}

		const_node_type get_node(const address& address) const
		{
			size_t linear_addr = linear_address(address);

			IMAGE_PROC_ASSERT(linear_addr < m_Elements.size());

			return m_Elements[linear_addr];
		}

		node_type& get_node(const address& address)
		{
			size_t linear_addr = linear_address(address);

			IMAGE_PROC_ASSERT(linear_addr < m_Elements.size());

			return m_Elements[linear_addr];
		}

		void set_node(const address& address, node_type n)
		{
			get_node(address) = n;
		}

		size_t num_dimensions() const
		{
			return m_Dimensions.size();
		}

		size_t get_dimension_size(size_t d) const
		{
			return m_Dimensions[d];
		}

		size_t get_num_nodes() const
		{
			if (num_dimensions() == 0)
				return 0;
			
			size_t n = m_Dimensions[0];
			for (size_t i = 0; i < num_dimensions(); ++i)
				n *= m_Dimensions[i];

			return n;				
		}

		void trim_single_dimension()
		{
			std::vector<size_t> new_dims;
			for (auto d : m_Dimensions)
			{
				if (d > 1)
					new_dims.push_back(d);
			}
			m_Dimensions = new_dims;
		}

	private:
		size_t linear_address(const address& address) const
		{
			IMAGE_PROC_ASSERT(address.size() == m_Dimensions.size());
			IMAGE_PROC_ASSERT(m_Dimensions.size() > 0);

			size_t addr = address[0];
			for (size_t d = 1; d < m_Dimensions.size(); ++d)
			{
				addr *= m_Dimensions[d];
				addr += address[d];
			}

			return addr;
		}
	private:
		std::vector<node_type> m_Elements;
		std::vector<size_t> m_Dimensions;
		std::vector<std::string> m_Names;
	};


	
} // end namespace
} // end namespace

#endif // RESULTSET_H_6AA0F334_FE39_45C4_A3F9_96E155382A81
