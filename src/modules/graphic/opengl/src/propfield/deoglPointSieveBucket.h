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

// include only once
#ifndef _DEOGLPOINTSIEVEBUCKET_H_
#define _DEOGLPOINTSIEVEBUCKET_H_

// includes

// predefinitions



/**
 * @brief Point Sieve Bucket.
 *
 * Bucket in a point sieve storing indices of objects dropped into it.
 */
class deoglPointSieveBucket{
private:
	int *pIndices;
	int pIndexCount;
	int pIndexSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new bucket. */
	deoglPointSieveBucket();
	/** Cleans up the bucket. */
	~deoglPointSieveBucket();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of indices. */
	inline int GetIndexCount() const{ return pIndexCount; }
	/** Retrieves the index at the given position. */
	int GetIndexAt( int position ) const;
	/** Adds an index. */
	void AddIndex( int index );
	/** Removes all the indices. */
	void RemoveAllIndices();
	/*@}*/
};

// end of include only once
#endif
