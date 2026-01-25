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

#include "deoglConvexHull2D.h"

#include <dragengine/common/exceptions.h>



// Class deoglConvexHull2D
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConvexHull2D::deoglConvexHull2D() = default;
deoglConvexHull2D::~deoglConvexHull2D() = default;


// Management
///////////////

void deoglConvexHull2D::AddPoint(const decVector2 &point){
	pPoints.Add(point);
}

void deoglConvexHull2D::AddPoint(float x, float y){
	pPoints.Add({x, y});
}

void deoglConvexHull2D::RemoveAllPoints(){
	pPoints.SetCountDiscard(0);
}


const decVector2 &deoglConvexHull2D::GetHullPointVectorAt(int index) const{
	return pPoints[pHullPoints[index]];
}


void deoglConvexHull2D::CalculateHull(){
	pHullPoints.SetCountDiscard(0);
	
	if(pPoints.GetCount() < 3){
		return;
	}
	
	pHullPoints.EnlargeCapacity(pPoints.GetCount());
	
	int testIndex;
	int i, j, bestIndex;
	decVector2 edge;
	float bestValue;
	
	// init the hull points list
	for(i=0; i<pPoints.GetCount(); i++){
		pHullPoints.Add(i);
	}
	
	// find the starting point. this is the point with the smallest x coordinate. this point is
	// added to the convex hull and moved to the front position to avoid testing it again
	const decVector2 * const points = pPoints.GetArrayPointer();
	int * const hullPoints = pHullPoints.GetArrayPointer();
	
	bestValue = pPoints[0].x;
	bestIndex = 0;
	
	for(i=1; i<pPoints.GetCount(); i++){
		if(points[i].x < bestValue){
			bestValue = points[i].x;
			bestIndex = i;
		}
	}
	
	// find the rest of the hull points. for each loop the point is searched for which all other
	// points are on the right side of the line running from the last found point to the point in
	// question. for testing the sideness of points the triangle area calculation is
	// used. every found point is added to the hull points and moved to the location of the first
	// remaining point. this way they are not tested again in the coming loops. the test stops
	// if there is no more point with all points on the right side. this is the case for the last
	// point in the hull before closing up with the first point
	
	// the loop looks a bit strange the but idea is the following. if the loop variable i is equal
	// to pPointCount then we found no point anymore having all other points on the right side.
	// instead of using an if-condition to either update the hull list or to break from the loop at
	// the end of the main loop we can move this all at the beginning and into the for-condition.
	// to get this working the variable i is set to bestIndex for the first run of the loop. this
	// way the moving of the initial point to the start of the list is done before the first
	// testing is conducted. furthermore exchanges happen only if in the previous loop a new point
	// has been found. it is a sort of delayed loop
	int hpnext = 0;
	
	for(i=bestIndex; i<pPoints.GetCount(); hpnext++){
		std::swap(hullPoints[i], hullPoints[hpnext]);
		
		const decVector2 &lineStart = points[hullPoints[hpnext]];
		
		for(i=hpnext+1; i<pPoints.GetCount(); i++){
			// for the calculation of the triangle area the following fomula can be used:
			// 0.5 * ( ( bx - ax )*( cy - ay ) - ( cx - ax )*( by - ay ) ) . for testing the sideness
			// the scaling by 0.5 is not important. this gives the simpler version:
			// ( bx - ax )*( cy - ay ) - ( cx - ax )*( by - ay ). this can be further converted to a
			// ( direct if-condition test (a point on the left side is positive, fails the test):
			// ( bx - ax )*( cy - ay ) > ( cx - ax )*( by - ay ). furthermore the lineStart and lineEnd
			// relative only value can be pre-computed for each loop
			
			testIndex = hullPoints[i];
			
			const decVector2 &lineEnd = points[testIndex];
			
			edge = lineEnd - lineStart;
			
			for(j=0; j<pPoints.GetCount(); j++){
				if(j == testIndex){
					continue;
				}
				
				const decVector2 &lineTest = pPoints[j];
				
				if((lineTest.y - lineStart.y) * edge.x > (lineTest.x - lineStart.x) * edge.y){
					break;
				}
			}
			
			if(j == pPoints.GetCount()){
				break;
			}
		}
	}
	
	pHullPoints.SetCountDiscard(hpnext); // set count to the actual amount of hull points
}
