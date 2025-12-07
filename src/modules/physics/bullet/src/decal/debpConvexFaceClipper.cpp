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

#include <stdio.h>
#include <stdlib.h>

#include "debpConvexFaceClipper.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define EQUALITY_THRESHOLD	1e-4f



// Class debpConvexFaceClipper
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

debpConvexFaceClipper::debpConvexFaceClipper(){
	pVertices = NULL;
	pVertexCount = 0;
}

debpConvexFaceClipper::~debpConvexFaceClipper(){
	if(pVertices) delete [] pVertices;
}



// Management
///////////////

void debpConvexFaceClipper::SetNormal(const decDVector &normal){
	pNormal = normal;
}

const decDVector &debpConvexFaceClipper::GetVertexAt(int position) const{
	if(position < 0 || position >= pVertexCount) DETHROW(deeInvalidParam);
	
	return pVertices[position];
}

bool debpConvexFaceClipper::HasVertex(const decDVector &vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex.IsEqualTo(pVertices[i])) return true;
	}
	
	return false;
}

int debpConvexFaceClipper::IndexOfVertex(const decDVector &vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex.IsEqualTo(pVertices[i])) return i;
	}
	
	return -1;
}

void debpConvexFaceClipper::AddVertex(const decDVector &vertex){
	decDVector *newArray = new decDVector[pVertexCount + 1];
	if(!newArray) DETHROW(deeOutOfMemory);
	
	if(pVertices){
		int i;
		for(i=0; i<pVertexCount; i++) newArray[i] = pVertices[i];
		delete [] pVertices;
	}
	pVertices = newArray;
	
	pVertices[pVertexCount] = vertex;
	pVertexCount++;
}

void debpConvexFaceClipper::RemoveAllVertices(){
	if(pVertices){
		delete [] pVertices;
		pVertices = NULL;
	}
	pVertexCount = 0;
}



void debpConvexFaceClipper::ClipByPlane(const decDVector &planeNormal, const decDVector &planePosition){
	double planeDot = planeNormal * planePosition;
	decDVector edgeDirection, cutVertexPosition;
	decDVector *oldVertices = pVertices;
	int oldVertexCount = pVertexCount;
	int sideFirst, sideSecond;
	double dotFirst, dotSecond;
	int vertexFirst, vertexSecond, v;
	double lambda;
	
	pVertices = NULL;
	pVertexCount = 0;
	
	try{
		for(v=0; v<oldVertexCount; v++){
			// get the first and second vertex of this edge
			vertexFirst = v;
			vertexSecond = (v + 1) % oldVertexCount;
			
			// determine if the vertices are in front of the split plane
			dotFirst = planeNormal * oldVertices[vertexFirst] - planeDot;
			if(dotFirst > EQUALITY_THRESHOLD){
				sideFirst = 1;
				
			}else if(dotFirst < -EQUALITY_THRESHOLD){
				sideFirst = -1;
				
			}else{
				sideFirst = 0;
			}
			
			dotSecond = planeNormal * oldVertices[vertexSecond] - planeDot;
			if(dotSecond > EQUALITY_THRESHOLD){
				sideSecond = 1;
				
			}else if(dotSecond < -EQUALITY_THRESHOLD){
				sideSecond = -1;
				
			}else{
				sideSecond = 0;
			}
			
			// add vertex if front side or on the cut plane
			if(sideFirst != -1){
				AddVertex(oldVertices[vertexFirst]);
			}
			
			// if both are on different sides add cut vertex
			if(sideFirst != 0 && sideSecond != 0 && sideFirst != sideSecond){
				// calculate the cut vertex
				edgeDirection = oldVertices[vertexSecond] - oldVertices[vertexFirst];
				lambda = (planeDot - planeNormal * oldVertices[vertexFirst])
					/ (planeNormal * edgeDirection);
				cutVertexPosition = oldVertices[vertexFirst] + edgeDirection * lambda;
				
				// add vertex
				AddVertex(cutVertexPosition);
			}
		}
		
		// old vertices no more needed
		if(oldVertices) delete [] oldVertices;
		
	}catch(const deException &){
		if(oldVertices) delete [] oldVertices;
		throw;
	}
}

void debpConvexFaceClipper::Transform(const decDMatrix &matrix){
	int v;
	
	for(v=0; v<pVertexCount; v++){
		pVertices[v] = matrix * pVertices[v];
	}
	
	pNormal = matrix.TransformNormal(pNormal);
}
