/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DECTIMEHISTORY_H_
#define _DECTIMEHISTORY_H_

#include "../string/decString.h"


/**
 * \brief History of elapsed times for timing handling.
 * \version 1.6
 * 
 * Stores a list of N time entries using two rotating pointers. Also supports calculating
 * the minimum, maximum and average time.
 */
class DE_DLL_EXPORT decTimeHistory{
private:
	float *pEntries;
	float *pSorted;
	int pSize;
	
	int pHead;
	int pTail;
	int pCount;
	
	int pCutExtremeValues;
	bool pHasMetrics;
	float pMinimum;
	float pMaximum;
	float pAverage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create history with initial size. */
	decTimeHistory(int initialSize, int cutExtremeValues);
	
	/** \brief Clean up history. */
	~decTimeHistory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Size of history. */
	inline int GetSize() const{ return pSize; }
	
	/** \brief Set size of history. */
	void SetSize(int size);
	
	/** \brief Time at position. */
	float GetAt(int position) const;
	
	/** \brief Add time. */
	void Add(float time);
	
	/** \brief Clear history. */
	void Clear();
	
	/** \brief History is empty. */
	inline bool Empty() const{ return pCount == 0; }
	
	/** \brief Cut extreme values count. */
	inline int GetCutExtremeValues() const{ return pCutExtremeValues; }
	
	/** \brief Set cut extreme values count. */
	void SetCutExtremeValues(int cutExtremeValues);
	
	/**
	 * \brief Metrics are valid.
	 * \details Metrics are valid if enough entries exist in the history after cutting extreme values.
	 */
	inline bool HasMetrics() const{ return pHasMetrics; }
	
	/**
	 * \brief Smallest time value in the history.
	 * \details Valid if history is not empty.
	 */
	inline float GetMinimum() const{ return pMinimum; }
	
	/**
	 * \brief Largest time value in the history.
	 * \details Valid if history is not empty.
	 */
	inline float GetMaximum() const{ return pMaximum; }
	
	/**
	 * \brief Average time value in the history.
	 * \details Valid if history is not empty.
	 */
	inline float GetAverage() const{ return pAverage; }
	
	
	
	/** \brief Debug print string. */
	decString DebugPrint() const;
	/*@}*/
	
	
	
private:
	void pCalculateMetrics();
};

#endif
