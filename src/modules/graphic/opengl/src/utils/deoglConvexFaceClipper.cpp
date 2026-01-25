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

#include "deoglConvexFaceClipper.h"

#include <dragengine/common/exceptions.h>


// Definitions
////////////////

#define EQUALITY_THRESHOLD	1e-4f


// Class deoglConvexFaceClipper
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglConvexFaceClipper::deoglConvexFaceClipper() = default;
deoglConvexFaceClipper::~deoglConvexFaceClipper() = default;


// Management
///////////////

void deoglConvexFaceClipper::SetNormal(const decDVector &normal){
	pNormal = normal;
}



void deoglConvexFaceClipper::ClipByPlane(const decDVector &planeNormal, const decDVector &planePosition){
	const double planeDot = planeNormal * planePosition;
	int sideFirst, sideSecond;
	
	pOldVertices = pVertices;
	pVertices.SetCountDiscard(0);
	
	const decDVector * const oldVertices = pOldVertices.GetArrayPointer();
	const int oldVertexCount = pOldVertices.GetCount();
	
	int i;
	for(i=0; i<oldVertexCount; i++){
		// get the first and second vertex of this edge
		const int vertexFirst = i;
		const int vertexSecond = (i + 1) % oldVertexCount;
		
		// determine if the vertices are in front of the split plane
		const double dotFirst = planeNormal * oldVertices[vertexFirst] - planeDot;
		if(dotFirst > EQUALITY_THRESHOLD){
			sideFirst = 1;
			
		}else if(dotFirst < -EQUALITY_THRESHOLD){
			sideFirst = -1;
			
		}else{
			sideFirst = 0;
		}
		
		const double dotSecond = planeNormal * oldVertices[vertexSecond] - planeDot;
		if(dotSecond > EQUALITY_THRESHOLD){
			sideSecond = 1;
			
		}else if(dotSecond < -EQUALITY_THRESHOLD){
			sideSecond = -1;
			
		}else{
			sideSecond = 0;
		}
		
		// add vertex if front side or on the cut plane
		if(sideFirst != -1){
			pVertices.Add(oldVertices[vertexFirst]);
		}
		
		// if both are on different sides add cut vertex
		if(sideFirst != 0 && sideSecond != 0 && sideFirst != sideSecond){
			// calculate the cut vertex
			const decDVector edgeDirection(oldVertices[vertexSecond] - oldVertices[vertexFirst]);
			const double lambda = (planeDot - planeNormal * oldVertices[vertexFirst])
				/ (planeNormal * edgeDirection);
			const decDVector cutVertexPosition(oldVertices[vertexFirst] + edgeDirection * lambda);
			
			// add vertex
			pVertices.Add(cutVertexPosition);
		}
	}
}

void deoglConvexFaceClipper::Transform(const decDMatrix &matrix){
	pVertices.Visit([&](decDVector &vertex){
		vertex = matrix * vertex;
	});
	
	pNormal = matrix.TransformNormal(pNormal);
}
