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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "decConvexVolume.h"
#include "decConvexVolumeFace.h"
#include "../exceptions.h"



// Definitions
////////////////


#define EQUALITY_THRESHOLD	1e-3f



// Class decConvexVolumeFace
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

decConvexVolumeFace::decConvexVolumeFace(){
	pVertices = NULL;
	pVertexCount = 0;
	pMarker = 0;
}

decConvexVolumeFace::~decConvexVolumeFace(){
	if(pVertices) delete [] pVertices;
}



// Management
///////////////

void decConvexVolumeFace::SetNormal(const decVector &normal){
	pNormal = normal;
}

void decConvexVolumeFace::SetMarker(int marker){
	pMarker = marker;
}

int decConvexVolumeFace::GetVertexAt(int position) const{
	if(position < 0 || position >= pVertexCount) DETHROW(deeInvalidParam);
	return pVertices[position];
}

bool decConvexVolumeFace::HasVertex(int vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex == pVertices[i]) return true;
	}
	
	return false;
}

int decConvexVolumeFace::IndexOfVertex(int vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex == pVertices[i]) return i;
	}
	
	return -1;
}

void decConvexVolumeFace::AddVertex(int vertex){
	int *newArray = new int[pVertexCount + 1];
	if(pVertices){
		int i;
		for(i=0; i<pVertexCount; i++) newArray[i] = pVertices[i];
		delete [] pVertices;
	}
	pVertices = newArray;
	pVertices[pVertexCount] = vertex;
	pVertexCount++;
}

void decConvexVolumeFace::RemoveAllVertices(){
	if(pVertices){
		delete [] pVertices;
		pVertices = NULL;
	}
	pVertexCount = 0;
}



void decConvexVolumeFace::SortVertices(const decConvexVolume &volume){
#if 1
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
	if(pVertexCount > 2){
		decVector center, referenceDirection, windingDirection;
		float windingDot, nextDot, testDot;
		int i, j, nextVertex, exchange;
		//printf( "sort vertices in %p\n", this );
		
		for(i=0; i<pVertexCount; i++){
			//printf( "vertex %i (%+f,%+f,%+f)\n", pVertices[ i ], volume.GetVertexAt( pVertices[ i ] ).x, volume.GetVertexAt( pVertices[ i ] ).y, volume.GetVertexAt( pVertices[ i ] ).z );
			center += volume.GetVertexAt(pVertices[i]);
		}
		center /= (float)pVertexCount;
		
		//printf( "normal (%+g,%+g,%+g)\n", pNormal.x, pNormal.y, pNormal.z );
		//printf( "center (%+f,%+f,%+f)\n", center.x, center.y, center.z );
		
		referenceDirection = (volume.GetVertexAt(pVertices[0]) - center).Normalized();
		windingDirection = pNormal % referenceDirection;
		windingDot = windingDirection * center - FLOAT_SAFE_EPSILON;
		
		for(i=1; i<pVertexCount-1; i++){
			nextVertex = -1;
			nextDot = 0.0f;
			
			for(j=i; j<pVertexCount; j++){
				const decVector &testVertex = volume.GetVertexAt(pVertices[j]);
				
				testDot = (testVertex - center).Normalized() * referenceDirection;
				if(testVertex * windingDirection < windingDot){
					testDot = -2.0f - testDot;
				}
				
				if(nextVertex == -1 || testDot > nextDot){
					nextVertex = j;
					nextDot = testDot;
				}
			}
			
			if(nextVertex != -1){
				exchange = pVertices[i];
				pVertices[i] = pVertices[nextVertex];
				pVertices[nextVertex] = exchange;
			}
		}
		
		//for( i=0; i<pVertexCount; i++ ){
		//	printf( "sorted vertex %i (%+f,%+f,%+f)\n", pVertices[ i ], volume.GetVertexAt( pVertices[ i ] ).x, volume.GetVertexAt( pVertices[ i ] ).y, volume.GetVertexAt( pVertices[ i ] ).z );
		//}
	}
#endif
#if 0
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
	if(pVertexCount > 2){
		decVector center, referenceDirection, windingDirection, testDirection;
		float windingDot, nextDot, testDot;
		int i, j, nextVertex, exchange;
		printf("sort vertices in %p\n", this);
		
		for(i=0; i<pVertexCount; i++){
			printf("vertex %i (%+f,%+f,%+f)\n", pVertices[i], volume.GetVertexAt(pVertices[i]).x, volume.GetVertexAt(pVertices[i]).y, volume.GetVertexAt(pVertices[i]).z);
			center += volume.GetVertexAt(pVertices[i]);
		}
		center /= (float)pVertexCount;
		
		printf("normal (%+g,%+g,%+g)\n", pNormal.x, pNormal.y, pNormal.z);
		printf("center (%+f,%+f,%+f)\n", center.x, center.y, center.z);
		
		for(i=1; i<pVertexCount-1; i++){
			referenceDirection = (volume.GetVertexAt(pVertices[i - 1]) - center).Normalized();
			windingDirection = pNormal % referenceDirection;
			windingDot = windingDirection * center - FLOAT_SAFE_EPSILON;
			
			nextVertex = -1;
			nextDot = -1.0f;
			
			for(j=i; j<pVertexCount; j++){
				const decVector &testVertex = volume.GetVertexAt(pVertices[j]);
				
				if(testVertex * windingDirection >= windingDot){
					testDot = (testVertex - center).Normalized() * referenceDirection;
					
					if(nextVertex == -1 || testDot > nextDot){
						nextVertex = j;
						nextDot = testDot;
					}
				}
			}
			
			if(nextVertex != -1){
				exchange = pVertices[i];
				pVertices[i] = pVertices[nextVertex];
				pVertices[nextVertex] = exchange;
			}
		}
		
		for(i=0; i<pVertexCount; i++){
			printf("sorted vertex %i (%+f,%+f,%+f)\n", pVertices[i], volume.GetVertexAt(pVertices[i]).x, volume.GetVertexAt(pVertices[i]).y, volume.GetVertexAt(pVertices[i]).z);
		}
	}
#endif
#if 0
	decVector pivotVertex, testNormal, curVertex;
	int i, j, testIndex, temporary;
	
	// sorting is only required with 3 or more vertices
	if(pVertexCount < 3) return;
	
	// sort vertices using an inside-outside test
	for(i=1; i<pVertexCount-1; i++){
		// we use this vertex as the test vertex to start with
		pivotVertex = volume.GetVertexAt(pVertices[i - 1]);
		testIndex = i;
		testNormal = pNormal % (volume.GetVertexAt(pVertices[i]) - pivotVertex);
		
		// test against all the remaining vertices if one is outside
		for(j=i+1; j<pVertexCount; j++){
			// if the vertex is outside it is a better vertex and is promoted
			// to be the test vertex for the remaining tests
			curVertex = volume.GetVertexAt(pVertices[j]);
			if(testNormal * (curVertex - pivotVertex) < 0.0f){
				testIndex = j;
				testNormal = pNormal % (curVertex - pivotVertex);
			}
		}
		
		// if the test vertex is not this vertex then a better one has been found.
		// in this case the two vertices are exchanged
		if(testIndex != i){
			temporary = pVertices[i];
			pVertices[i] = pVertices[testIndex];
			pVertices[testIndex] = temporary;
		}
	}
#endif
}

bool decConvexVolumeFace::IsTooSmall(const decConvexVolume &volume) const{
	if(pVertexCount > 2){
		decVector center;
		int i;
		
		for(i=0; i<pVertexCount; i++){
			center += volume.GetVertexAt(pVertices[i]);
		}
		center /= (float)pVertexCount;
		
		for(i=0; i<pVertexCount; i++){
			if((volume.GetVertexAt(pVertices[i]) - center).Length() < FLOAT_SAFE_EPSILON){
				return true;
			}
		}
		
		/*
		int i;
		
		for(i=0; i<pVertexCount; i++){
			const decVector &p1 = volume.GetVertexAt(pVertices[i]);
			const decVector &p2 = volume.GetVertexAt(pVertices[(i + 1) % pVertexCount]);
			
			if((p2 - p1).Length() < FLOAT_SAFE_EPSILON){
				return true;
			}
		}
		*/
		
		return false;
	}
	
	return true;
}

/*
bool decConvexVolumeFace::IsValid() const{
	decVector testNormal;
	int i, v1, v2, v3;
	
	// normal has to be non-null
	if(pNormal.Length() < 1e-5f) return false;
	
	// there have to be 3 or more vertices
	if(pVertexCount < 3) return false;
	
	// vertices have to be in clockwise order
	for(i=0; i<pVertexCount; i++){
		v1 = i;
		v2 = (i + 1) % pVertexCount;
		v3 = (i + 2) % pVertexCount;
		testNormal = pNormal % (pVertices[v2] - pVertices[v1]);
		if(testNormal * (pVertices[v3] - pVertices[v1]) < 0.0f) return false;
	}
	
	// all tests passes
	return true;
}
*/
