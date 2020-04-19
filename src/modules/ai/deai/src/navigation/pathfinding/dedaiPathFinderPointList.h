/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAIPATHFINDERPOINTLIST_H_
#define _DEDAIPATHFINDERPOINTLIST_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Path Finder Point List.
 */
class dedaiPathFinderPointList{
private:
	decVector *pPoints;
	int pPointCount;
	int pPointSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new path finder point list. */
	dedaiPathFinderPointList();
	/** Cleans up the path finder point list. */
	~dedaiPathFinderPointList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of points. */
	inline int GetCount() const{ return pPointCount; }
	/** Determines if the list is empty. */
	inline bool IsEmpty() const{ return pPointCount == 0; }
	/** Retrieves the top point. */
	const decVector &GetFirst() const;
	/** Retrieves the given point from the start of the list. */
	const decVector &GetAt( int index ) const;
	/** Add a point to the end of the list. */
	void Add( const decVector &point );
	/** Removes the first point. */
	void RemoveFirst();
	/** Remove the given number of points from the start of the list. */
	void RemoveFirst( int count );
	/** Removes the given point from the start of the list. */
	void RemoveFrom( int index );
	/** Removes all points. */
	void RemoveAll();
	
	/** Transform point list by a matrix. */
	void Transform( const decMatrix &matrix );
	/*@}*/
};

#endif
