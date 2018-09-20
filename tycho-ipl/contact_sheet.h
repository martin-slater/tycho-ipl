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

#ifndef CONTACTSHEET_H_3EAF8583_2E89_4867_AF30_CE575CE421CF
#define CONTACTSHEET_H_3EAF8583_2E89_4867_AF30_CE575CE421CF

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include "runtime/session_options.h"
#include <string>
#include <list>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{

	//----------------------------------------------------------------------------
	// Arrange multiple images into one or more contact sheets to more easily
	// visualize parameter spaces and algorithm differences.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI contact_sheet
	{
	public:
		enum class Orientation
		{
			Invalid = 0,
			Portrait,
			Landscape
		};

	public:
		/// Default constructor
		contact_sheet() = default; 		

		/// Automatically construct the contact sheets from the given results
		/// matrix.
		void auto_build(
			const std::string& title, 
			const result_matrix& results, 
			const std::string& output_path,
			const runtime::session_options::file_list& input_files);

		/// Adds a page to the contact sheet
		void add_page(const std::string& title, const result_matrix& results);

		/// Save the contact sheet to disk
		bool write(const std::string &path);

	private:
		struct page
		{
			page(const std::string& title, const std::string& contents) :
				Title(title),
				Contents(contents)
			{}

			std::string Title;
			std::string Contents;
		};

		size_t ImageSize = 512;
		size_t ImageBorder = 5;
		size_t ImageTopBorder = 80;
		size_t InfoRectHeight = 20;
		size_t InfoRectPadding = 5;
		float  FontScale = 0.8f;
		size_t FontThickness = 1;
	};

	
} // end namespace
} // end namespace

#endif // CONTACTSHEET_H_3EAF8583_2E89_4867_AF30_CE575CE421CF
