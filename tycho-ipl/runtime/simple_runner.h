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

#ifndef SIMPLERUNNER_H_ACDE07AF_6025_40A9_9368_382BCF4901AA
#define SIMPLERUNNER_H_ACDE07AF_6025_40A9_9368_382BCF4901AA

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "../image_processing_abi.h"
#include "../runtime/runner.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{
namespace runtime
{

	//----------------------------------------------------------------------------
	// Run the specified program over all input images.
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI simple_runner : public runner
	{
	public:
		/// Default constructor
		simple_runner(const session_options& options, output_interface* output);
				
		/// Process the images
		void run() override;
	};

	
} // end namespace
} // end namespace
} // end namespace

#endif // SIMPLERUNNER_H_ACDE07AF_6025_40A9_9368_382BCF4901AA
