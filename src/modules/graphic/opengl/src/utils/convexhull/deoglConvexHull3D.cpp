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

#include "deoglConvexHull3D.h"

#define CONVHULL_3D_ENABLE
#define CONVHULL_3D_USE_FLOAT_PRECISION
#include "../../../convexhull3d/convhull_3d.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>


// Class deoglConvexHull3D
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConvexHull3D::deoglConvexHull3D() = default;
deoglConvexHull3D::~deoglConvexHull3D() = default;


// Management
///////////////

void deoglConvexHull3D::AddPoint(const decVector &point){
	pPoints.Add(point);
}

void deoglConvexHull3D::RemoveAllPoints(){
	pPoints.SetCountDiscard(0);
}


void deoglConvexHull3D::CalculateHull(){
	pHullIndices.SetCountDiscard(0);
	
	if(pPoints.GetCount() < 3){
		return;
	}
	
	// find non-duplicate points
	decTList<decVector> uniquePoints(pPoints.GetCount());
	pPoints.Visit([&](const decVector &point){
		if(!uniquePoints.HasMatching([&](const decVector &p){
			return p.IsEqualTo(point);
		})){
			uniquePoints.Add(point);
		}
	});
	
	// check for degenerate cases (all collinear or coplanar)
	if(pArePointsCoplanar(uniquePoints)){
		return;
	}
	
	decTList<ch_vertex> vertices(uniquePoints.GetCount());
	uniquePoints.Visit([&](const decVector &point){
		vertices.Add({point.x, point.y, point.z});
	});
	
	int *faceIndices = nullptr;
	int nFaces = 0;
	convhull_3d_build(vertices.GetArrayPointer(), vertices.GetCount(), &faceIndices, &nFaces);
	
	pHullIndices.EnlargeCapacity(nFaces * 3);
	for(int i=0; i<nFaces*3; i++){
		pHullIndices.Add(faceIndices[i]);
	}
	
	free(faceIndices);
}


// Private Functions
//////////////////////

bool deoglConvexHull3D::pArePointsCoplanar(const decTList<decVector> &points) const{
	if(points.GetCount() < 4){
		return true; // always coplanar
	}
	
	// use first 3 non-collinear points to define a plane
	const float tolerance = 1e-4f;
	decVector p0(points[0]);
	
	// find second point that is not too close to the first one
	decVector p1(p0);
	int i;
	
	for(i=1; i<points.GetCount(); i++){
		if(!p0.IsEqualTo(points[i])){
			p1 = points[i];
			break;
		}
	}
	if(i >= points.GetCount()){
		return true; // all points are duplicates
	}
	
	// find third point that is not collinear with first two points
	const decVector edge1(p1 - p0);
	decVector p2(p0);
	
	for(i=i+1; i<points.GetCount(); i++){
		if((edge1 % (points[i] - p0)).Length() > tolerance){
			p2 = points[i];
			break;
		}
	}
	if(i >= points.GetCount()){
		return true; // all points are collinear
	}
	
	// check if all remaining points are coplanar with p0, p1 and p2
	const decVector edge2(p2 - p0);
	decVector normal(edge1 % edge2);
	
	if(normal.Length() < tolerance){
		return true; // plane normal is degenerated
	}
	
	normal.Normalize();
	const float d = normal * p0;
	
	for(int j=3; j < points.GetCount(); j++){
		if(fabsf((normal * points[j]) - d) > tolerance){
			return false; // foundpoint is not on the plane
		}
	}
	
	return true;
}
