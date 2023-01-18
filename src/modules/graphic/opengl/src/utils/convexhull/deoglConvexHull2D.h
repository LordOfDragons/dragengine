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

#ifndef _DEOGLCONVEXHULL2D_H_
#define _DEOGLCONVEXHULL2D_H_

#include <dragengine/common/math/decMath.h>


/**
 * @brief Clockwise oriented 2D Convex Hull calculator for small numbers of points using gift wrap algorithm.
 */
class deoglConvexHull2D{
private:
	decVector2 *pPoints;
	int pPointCount;
	int pPointSize;
	
	int *pHullPoints;
	int pHullPointCount;
	int pHullPointSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new 2d convex hull calculator. */
	deoglConvexHull2D();
	/** Cleans up the 2d convex hull calculator. */
	~deoglConvexHull2D();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	/** Retrieves a point by index. */
	const decVector2 &GetPointAt( int index ) const;
	/** Adds a point. */
	void AddPoint( const decVector2 &point );
	void AddPoint( float x, float y );
	/** Removes all points. */
	void RemoveAllPoints();
	
	/** Retrieves the number of hull points. */
	inline int GetHullPointCount() const{ return pHullPointCount; }
	/** Retrieves a hull point by index. */
	int GetHullPointAt( int index ) const;
	/** Retrieves the point using a hull point by index. */
	const decVector2 &GetHullPointVectorAt( int index ) const;
	
	/** Calculate convex hull. */
	void CalculateHull();
	/*@}*/
};

#endif
