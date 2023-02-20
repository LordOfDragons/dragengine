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

#ifndef _DEOGLTRIANGLESORTER_H_
#define _DEOGLTRIANGLESORTER_H_

#include <dragengine/common/math/decMath.h>



/**
 * Triangle Sorter.
 * 
 * Stores a list of triangles that can be sorted by distance.
 */
class deoglTriangleSorter{
private:
	struct sTriangle{
		decVector vertex1;
		decVector vertex2;
		decVector vertex3;
		float distance;
	};
	
	
	
private:
	sTriangle **pTriangles;
	int pTriangleCount;
	int pTriangleSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create triangle sorter. */
	deoglTriangleSorter();
	
	/** Clean up triangle sorter. */
	~deoglTriangleSorter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of triangles. */
	inline int GetTriangleCount() const{ return pTriangleCount; }
	
	/** First vertex of triangle at index. */
	const decVector &GetTriangleVertex1( int triangle ) const;
	
	/** Second vertex of triangle at index. */
	const decVector &GetTriangleVertex2( int triangle ) const;
	
	/** Third vertex of triangle at index. */
	const decVector &GetTriangleVertex3( int triangle ) const;
	
	/** Add triangle. */
	void AddTriangle( const decVector &vertex1, const decVector &vertex2, const decVector &vertex3 );
	
	/** Remove all triangles. */
	void RemoveAllTriangles();
	/*@}*/
	
	
	
	/** \name Sorting */
	/*@{*/
	/** Sort objects linear. */
	void SortLinear( const decVector &position, const decVector &view );
	
	/** Sort objects radial. */
	void SortRadial( const decVector &position );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pQuickSortDistance( int left, int right );
};

#endif
