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
	inline int GetPointCount() const{return pPointCount;}
	
	/** Point by index. */
	const decVector &GetPointAt(int index) const;
	
	/** Add point. */
	void AddPoint(const decVector &point);
	
	/** Remove all points. */
	void RemoveAllPoints();
	
	/** Convex hull indices in groups of 3 per face. */
	inline const decIntList &GetHullIndices() const{return pHullIndices;}
	
	/** Calculate convex hull. */
	void CalculateHull();
	/*@}*/
};

#endif
