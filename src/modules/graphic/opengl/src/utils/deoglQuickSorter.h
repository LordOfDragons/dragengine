/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLQUICKSORTER_H_
#define _DEOGLQUICKSORTER_H_



/**
 * @brief Quick Sorter.
 */
class deoglQuickSorter{
private:
	void **pElements;
	float *pScores;
	int pCount;
	int pSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new quick sorter. */
	deoglQuickSorter();
	/** Cleans up the quick sorter. */
	~deoglQuickSorter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of elements. */
	inline int GetElementCount() const{ return pCount; }
	/** Retrieves the list of elements. */
	inline void **GetElements() const{ return pElements; }
	/** Retrieves an element by index. */
	void *GetElementAt( int index ) const;
	/** Removes all elements. */
	void RemoveAllElements();
	/** Adds an element. */
	void AddElement( void *element, float score );
	/** Sort elements in ascending order. */
	void SortAscending();
	/** Sort elements in descending order. */
	void SortDescending();
	
	/** Test the implementation. Throws an exception if the implementation fails. */
	static void TestImplementation();
	/*@}*/
	
private:
	void pSortAscendingStep( int left, int right );
	void pSortDescendingStep( int left, int right );
};

#endif
