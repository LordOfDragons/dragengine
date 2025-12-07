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

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "deoglConvexHull3D.h"

#define CONVHULL_3D_ENABLE
#define CONVHULL_3D_USE_FLOAT_PRECISION
#include "../../../convexhull3d/convhull_3d.h"

#include <dragengine/common/exceptions.h>



// Class deoglConvexHull3D
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConvexHull3D::deoglConvexHull3D(){
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
}

deoglConvexHull3D::~deoglConvexHull3D(){
	if(pPoints){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector &deoglConvexHull3D::GetPointAt(int index) const{
	if(index < 0 || index >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	return pPoints[index];
}

void deoglConvexHull3D::AddPoint(const decVector &point){
	if(pPointCount == pPointSize){
		const int newSize = pPointSize + 10;
		decVector * const newArray = new decVector[newSize];
		
		if(pPoints){
			memcpy(newArray, pPoints, sizeof(decVector) * pPointCount);
			delete [] pPoints;
		}
		pPoints = newArray;
		
		pPointSize = newSize;
	}
	
	pPoints[pPointCount++] = point;
}

void deoglConvexHull3D::RemoveAllPoints(){
	pPointCount = 0;
}



void deoglConvexHull3D::CalculateHull(){
	pHullIndices.RemoveAll();
	
	if(pPointCount < 3){
		return;
	}
	
	ch_vertex * const vertices = new ch_vertex[pPointCount];
	int i;
	for(i=0; i<pPointCount; i++){
		vertices[i].x = pPoints[i].x;
		vertices[i].y = pPoints[i].y;
		vertices[i].z = pPoints[i].z;
	}
	
	int *faceIndices = NULL;
	int nFaces;
	convhull_3d_build(vertices, pPointCount, &faceIndices, &nFaces);
	
	for(i=0; i<nFaces*3; i++){
		pHullIndices.Add(faceIndices[i]);
	}
	
	free(faceIndices);
	delete [] vertices;
}
