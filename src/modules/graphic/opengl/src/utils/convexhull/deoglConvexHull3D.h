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

#ifndef _DEOGLCONVEXHULL3D_H_
#define _DEOGLCONVEXHULL3D_H_

#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>


/**
 * 3D Convex Hull calculator based on 
 */
class deoglConvexHull3D{
private:
	decVector *pPoints;
	int pPointCount;
	int pPointSize;
	
	decIntList pHullIndices;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create 3d convex hull calculator. */
	deoglConvexHull3D();
	
	/** Clean up 3d convex hull calculator. */
	~deoglConvexHull3D();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** Point by index. */
	const decVector &GetPointAt( int index ) const;
	
	/** Add point. */
	void AddPoint( const decVector &point );
	
	/** Remove all points. */
	void RemoveAllPoints();
	
	/** Convex hull indices in groups of 3 per face. */
	inline const decIntList &GetHullIndices() const{ return pHullIndices; }
	
	/** Calculate convex hull. */
	void CalculateHull();
	/*@}*/
};

#endif
