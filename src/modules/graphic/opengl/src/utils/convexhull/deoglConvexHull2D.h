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

#ifndef _DEOGLCONVEXHULL2D_H_
#define _DEOGLCONVEXHULL2D_H_

#include <dragengine/common/math/decMath.h>


/**
 * Clockwise oriented 2D Convex Hull calculator for small numbers of points using gift wrap algorithm.
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
