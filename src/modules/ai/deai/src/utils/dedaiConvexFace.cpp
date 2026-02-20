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

#include "dedaiConvexFace.h"
#include "dedaiConvexFaceList.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define EQUALITY_THRESHOLD 1e-3f



// Class dedaiConvexFace
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiConvexFace::dedaiConvexFace() :
pMarker(0){
}

dedaiConvexFace::dedaiConvexFace(const dedaiConvexFace &face) :
pVertices(face.pVertices),
pNormal(face.pNormal),
pMarker(face.pMarker){
}

dedaiConvexFace::dedaiConvexFace(int marker) :
pMarker(marker){
}

dedaiConvexFace::~dedaiConvexFace(){
}



// Management
///////////////

void dedaiConvexFace::SetNormal(const decVector &normal){
	pNormal = normal;
}

void dedaiConvexFace::SetMarker(int marker){
	pMarker = marker;
}

int dedaiConvexFace::GetVertexAt(int index) const{
	return pVertices.GetAt(index);
}

void dedaiConvexFace::SetVertexAt(int index, int vertex){
	pVertices.SetAt(index, vertex);
}

bool dedaiConvexFace::HasVertex(int vertex) const{
	return pVertices.Has(vertex);
}

int dedaiConvexFace::IndexOfVertex(int vertex) const{
	return pVertices.IndexOf(vertex);
}

void dedaiConvexFace::AddVertex(int vertex){
	pVertices.Add(vertex);
}

void dedaiConvexFace::InsertVertex(int index, int vertex){
	pVertices.Insert(vertex, index);
}

void dedaiConvexFace::RemoveVertexFrom(int index){
	pVertices.RemoveFrom(index);
}

void dedaiConvexFace::RemoveAllVertices(){
	pVertices.RemoveAll();
}



decVector dedaiConvexFace::CalculateCenter(const dedaiConvexFaceList &convexFaceList) const{
	decVector center;
	if(pVertices.IsNotEmpty()){
		pVertices.Visit([&](int index){
			center += convexFaceList.GetVertexAt(index);
		});
		center /= (float)pVertices.GetCount();
	}
	return center;
}

void dedaiConvexFace::SortVertices(const dedaiConvexFaceList &convexFaceList){
	// sort vertices clockwise around the face normal. for this the dot product of all vertices is
	// calculated with the direction from the center to the first vertex. the vertex with the
	// largest dot product wins. this is the vertex with the smallest angle relative to the vector
	// from the center to the first vertex. for this test only vertices along the clockwise
	// direction are considered. these are all vertices that have a positive dot product between
	// the direction from the center to the vertex in question and the cross product of the face
	// normal and the vector from the center to the first vertex.
	// 
	// it is important to normalize the vectors. this is unfortunately required since otherwise
	// the result can be falsified by vertices located nearly at the same location in space
	// causing an additional loop around the face resulting in an incorrect ordering.
	if(pVertices.GetCount() < 3){
		return;
	}
	
	int i, j;
	
	const decVector center = CalculateCenter(convexFaceList);
	
	const decVector referenceDirection = (convexFaceList.GetVertexAt(pVertices.First()) - center).Normalized();
	const decVector windingDirection = pNormal % referenceDirection;
	const float windingDot = windingDirection * center - FLOAT_SAFE_EPSILON;
	
	for(i=1; i<pVertices.GetCount()-1; i++){
		int nextVertex = -1;
		float nextDot = 0.0f;
		
		for(j=i; j<pVertices.GetCount(); j++){
			const decVector &testVertex = convexFaceList.GetVertexAt(pVertices[j]);
			
			float testDot = (testVertex - center).Normalized() * referenceDirection;
			if(testVertex * windingDirection < windingDot){
				testDot = -2.0f - testDot;
			}
			
			if(nextVertex == -1 || testDot > nextDot){
				nextVertex = j;
				nextDot = testDot;
			}
		}
		
		if(nextVertex != -1){
			const int exchange = pVertices[i];
			pVertices[i] = pVertices[nextVertex];
			pVertices[nextVertex] = exchange;
		}
	}
}

bool dedaiConvexFace::IsTooSmall(const dedaiConvexFaceList &convexFaceList) const{
	if(pVertices.GetCount() < 3){
		return true;
	}
	
	const decVector center = CalculateCenter(convexFaceList);
	return pVertices.HasMatching([&](int index){
		return (convexFaceList.GetVertexAt(index) - center).Length() < FLOAT_SAFE_EPSILON;
	});
}



// Operators
//////////////

dedaiConvexFace &dedaiConvexFace::operator=(const dedaiConvexFace &face){
	pVertices = face.pVertices;
	pMarker = face.pMarker;
	pNormal = face.pNormal;
	return *this;
}
