/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	decTimeHistory( int initialSize, int cutExtremeValues );
	
	/** \brief Clean up history. */
	~decTimeHistory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Size of history. */
	inline int GetSize() const{ return pSize; }
	
	/** \brief Set size of history. */
	void SetSize( int size );
	
	/** \brief Time at position. */
	float GetAt( int position ) const;
	
	/** \brief Add time. */
	void Add( float time );
	
	/** \brief Clear history. */
	void Clear();
	
	/** \brief History is empty. */
	inline bool Empty() const{ return pCount == 0; }
	
	/** \brief Cut extreme values count. */
	inline int GetCutExtremeValues() const{ return pCutExtremeValues; }
	
	/** \brief Set cut extreme values count. */
	void SetCutExtremeValues( int cutExtremeValues );
	
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
