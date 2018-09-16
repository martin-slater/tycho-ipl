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

#ifndef _VARIATIONITERATOR_H_0F36D766_9A0D_4FEA_97B2_762E9E7A7970
#define _VARIATIONITERATOR_H_0F36D766_9A0D_4FEA_97B2_762E9E7A7970

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include <vector>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{

	//----------------------------------------------------------------------------
	// Given a vector of integers defining the dimensions of a search space allow
	// the user to iterate over all possible combinations.
	//----------------------------------------------------------------------------
	class variation_iterator
	{
	public:
		variation_iterator(const std::vector<int>& values) :
			m_Values(values),
			m_Complete(false)
		{
			m_State.resize(m_Values.size(), 0);
		}

		bool next(std::vector<int>& state)
		{
			if (m_Complete || m_State.size() == 0)
				return false;

			// return user the current state
			state = m_State;

			// and calculate next state
			for (int d = m_State.size() - 1; d >= 0; --d)
			{
				++m_State[d];

				if (m_State[d] >= m_Values[d])
				{
					if (d == 0)
					{
						m_Complete = true;
						break;
					}
					m_State[d] = 0;
				}
				else
				{
					break;
				}
			}

			return true;
		}

	private:
		std::vector<int> m_Values;
		std::vector<int> m_State;
		bool m_Complete;
	};
	
} // end namespace
} // end namespace

#endif // _VARIATIONITERATOR_H_0F36D766_9A0D_4FEA_97B2_762E9E7A7970
