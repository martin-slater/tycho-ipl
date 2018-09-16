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

#ifndef IMAGEPROCESSING_ABI_H_8E19A66E_4220_443B_A8B7_7CD19C3F8032
#define IMAGEPROCESSING_ABI_H_8E19A66E_4220_443B_A8B7_7CD19C3F8032

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "forward_decls.h"
#include <cassert>

#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#endif


#define IMAGE_PROC_SAFE_DELETE(_ptr) if(_ptr) { delete _ptr; _ptr = nullptr; }
#define IMAGE_PROC_ASSERT(_expr) assert(_expr)

#ifdef _MSC_VER

// DLL interface
#ifdef TYCHO_SHARED_LIB
#	ifdef TYCHO_IMAGEPROCESSING_EXPORTS
#		define TYCHO_IMAGEPROCESSING_ABI __declspec(dllexport)
#	else
#		define TYCHO_IMAGEPROCESSING_ABI __declspec(dllimport)
#	endif 
#else
#	define TYCHO_IMAGEPROCESSING_ABI
#endif

// disable a few warnings globally. should move these into specific cpp's to avoid polluting
// user header files
#pragma warning(disable : 4251) // class needs to have dll-interface to be used by clients of class ''
#pragma warning(disable : 4355) // 'this' : used in base member initializer list

#else // _WIN32

#define TYCHO_IMAGEPROCESSING_ABI

#endif // WIN32

#ifndef HAVE_LIBIMAGEQUANT
#define HAVE_LIBIMAGEQUANT 1
#endif

#endif // IMAGEPROCESSING_ABI_H_8E19A66E_4220_443B_A8B7_7CD19C3F8032
