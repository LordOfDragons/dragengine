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
	
	decTList<ch_vertex> vertices(pPoints.GetCount());
	int i;
	for(i=0; i<pPoints.GetCount(); i++){
		vertices.Add({pPoints[i].x, pPoints[i].y, pPoints[i].z});
	}
	
	int *faceIndices = nullptr;
	int nFaces;
	convhull_3d_build(vertices.GetArrayPointer(), pPoints.GetCount(), &faceIndices, &nFaces);
	
	pHullIndices.EnlargeCapacity(nFaces * 3);
	for(i=0; i<nFaces*3; i++){
		pHullIndices.Add(faceIndices[i]);
	}
	
	free(faceIndices);
}
