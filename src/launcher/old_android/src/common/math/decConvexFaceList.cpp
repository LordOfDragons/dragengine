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
#include <string.h>

#include "decConvexFace.h"
#include "decConvexFaceList.h"
#include "decConvexVolume.h"
#include "decConvexVolumeFace.h"
#include "../exceptions.h"



// Definitions
////////////////

#define TEST_NONE			0
#define TEST_SPLIT			1
#define TEST_REMOVE			2

#define SPLIT_FRONT_BACK	0
#define SPLIT_FRONT			1
#define SPLIT_BACK			2
#define SPLIT_NONE			3

#define THRESHOLD_EQUAL		0.0001f

#define THRESHOLD_ON_PLANE	0.0001f

#define PLANE_SIDE(dist)	(((dist) > THRESHOLD_ON_PLANE) ? 1 : (((dist) < -THRESHOLD_ON_PLANE) ? -1 : 0))



// Class decConvexFaceList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

decConvexFaceList::decConvexFaceList(){
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
}

decConvexFaceList::decConvexFaceList(const decConvexFaceList &list){
	const int vertexCount = list.GetVertexCount();
	const int faceCount = list.GetFaceCount();
	decConvexFace *face = NULL;
	int i;
	
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
	
	try{
		for(i=0; i<vertexCount; i++){
			AddVertex(list.GetVertexAt(i));
		}
		for(i=0; i<faceCount; i++){
			face = CreateFace(list.GetFaceAt(i));
			*face = *list.GetFaceAt( i ); // CreateFace does not create a copy just a new face
			AddFace(face);
			face = NULL;
		}
		
	}catch(const deException &){
		RemoveAllFaces();
		if(pVertices){
			delete [] pVertices;
		}
		throw;
	}
}

decConvexFaceList::~decConvexFaceList(){
	RemoveAllFaces();
	if(pVertices){
		delete [] pVertices;
	}
}



// Vertices
/////////////

int decConvexFaceList::GetVertexCount() const{
	return pVertexCount;
}

const decVector &decConvexFaceList::GetVertexAt(int index) const{
	if(index < 0 || index >= pVertexCount){
		DETHROW(deeInvalidParam);
	}
	return pVertices[index];
}

bool decConvexFaceList::HasVertex(const decVector &vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex.IsEqualTo(pVertices[i], THRESHOLD_EQUAL)){
			return true;
		}
	}
	
	return false;
}

int decConvexFaceList::IndexOfVertex(const decVector &vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex.IsEqualTo(pVertices[i], THRESHOLD_EQUAL)){
			return i;
		}
	}
	
	return -1;
}

void decConvexFaceList::AddVertex(const decVector &vertex){
// 	if( HasVertex( vertex ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	if(pVertexCount == pVertexSize){
		const int newSize = pVertexSize * 3 / 2 + 1;
		decVector * const newArray = new decVector[newSize];
		
		if(pVertices){
			memcpy(newArray, pVertices, sizeof(decVector) * pVertexCount);
			delete [] pVertices;
		}
		
		pVertices = newArray;
		pVertexSize = newSize;
	}
	
	pVertices[pVertexCount] = vertex;
	pVertexCount++;
}

void decConvexFaceList::RemoveVertex(int index){
	if(index < 0 || index >= pVertexCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=index+1; i<pVertexCount; i++){
		pVertices[i - 1] = pVertices[i];
	}
	pVertexCount--;
}

void decConvexFaceList::RemoveAllVertices(){
	pVertexCount = 0;
}




// Faces
//////////

int decConvexFaceList::GetFaceCount() const{
	return pFaces.GetCount();
}

decConvexFace *decConvexFaceList::GetFaceAt(int index) const{
	return (decConvexFace*)pFaces.GetAt(index);
}

bool decConvexFaceList::HasFace(decConvexFace *face) const{
	return pFaces.Has(face);
}

int decConvexFaceList::IndexOfFace(decConvexFace *face) const{
	return pFaces.IndexOf(face);
}

void decConvexFaceList::AddFace(decConvexFace *face){
	// no pFaces.Has(face) check. with larger number of faces this becomes very slow
	if(!face){
		DETHROW(deeInvalidParam);
	}
	pFaces.Add(face);
}

void decConvexFaceList::RemoveFaceAt(int index){
	decConvexFace * const face = (decConvexFace*)pFaces.GetAt(index);
	pFaces.RemoveFrom(index);
	delete face;
}

void decConvexFaceList::RemoveFace(decConvexFace *face){
	ExtractFace(face);
	delete face;
}

void decConvexFaceList::ExtractFaceAt(int index){
	pFaces.RemoveFrom(index);
}

void decConvexFaceList::ExtractFace(decConvexFace *face){
	const int index = pFaces.IndexOf(face);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	pFaces.RemoveFrom(index);
}

void decConvexFaceList::RemoveAllFaces(){
	const int count = pFaces.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (decConvexFace*)pFaces.GetAt(i);
	}
	pFaces.RemoveAll();
}



// Management
///////////////

void decConvexFaceList::Move(const decVector &direction){
	int i;
	for(i=0; i<pVertexCount; i++){
		pVertices[i] += direction;
	}
}

void decConvexFaceList::SplitByFace(const decConvexFaceList &splitterFaceList, int splitterFaceIndex){
	const decConvexFace &splitterFace = *splitterFaceList.GetFaceAt(splitterFaceIndex);
	const int splitVertexCount = splitterFace.GetVertexCount();
	
	// if the splitter face does not have at least 3 vertices do not split at all
	if(splitVertexCount < 3){
		return;
	}
	
	int faceCount = pFaces.GetCount();
	int i, j;
	
	// process each face in the list one by one. this is required since faces are allowed
	// to be located in different planes. the face count is kept constant to avoid testing
	// newly created faces again
	for(i=0; i<faceCount; i++){
//		if( splitterFaceIndex > 2 ) continue;
		const decConvexFace &testFace = *((decConvexFace*)pFaces.GetAt(i));
		
		// if the face does not have at least 3 vertices do not split but remove it
		if(testFace.GetVertexCount() < 3){
			RemoveFaceAt(i);
			i--;
			faceCount--;
			continue;
		}
		
		const decVector &planeNormal = testFace.GetNormal();
		const decVector &planePosition = pVertices[testFace.GetVertexAt(0)];
		const float planeDist = planeNormal * planePosition;
		
		// determine the split edge. for this find the two edges piercing the face plane
		// if existing. if the face is not convex due to a mistake by the user use the
		// first two found vertices as split edge. a better solution can be done later on
		bool hasEdgeFrom = false;
		bool hasEdgeTo = false;
		decVector edgeFrom;
		decVector edgeTo;
		
		for(j=0; j<splitVertexCount; j++){
			const decVector &edgeVertex1 = splitterFaceList.GetVertexAt(splitterFace.GetVertexAt(j));
			const decVector &edgeVertex2 = splitterFaceList.GetVertexAt(splitterFace.GetVertexAt((j + 1) % splitVertexCount));
			const decVector edgeDirection = edgeVertex2 - edgeVertex1;
			
			const float numer = planeNormal * edgeVertex1 - planeDist;
			const float denom = planeNormal * edgeDirection;
			
			// if the edge is parallel to plane ignore it. the edges right before and after this
			// edge are adding the edge start and end point already
			if(fabsf(denom) < THRESHOLD_ON_PLANE){
				continue;
			}
			
			// if the hit point is on the edge the edge contributes a cut edge point
			const float dist = -numer / denom;
			if(dist >= 0.0f && dist <= 1.0f){
				const decVector hitPoint = edgeVertex1 + edgeDirection * dist;
				
				if(!hasEdgeFrom){
					edgeFrom = hitPoint;
					hasEdgeFrom = true;
					
				}else if(!hasEdgeTo){
					edgeTo = hitPoint;
					hasEdgeTo = true;
					
				}else{
					printf("more than 2 cut points found. ignoring the new cut point.");
				}
			}
		}
		
		// if there is a split edge split the plan
		if(hasEdgeFrom && hasEdgeTo){
			pSplitFaceByEdge(i, edgeFrom, edgeTo);
			
			// if the face becomes degenerated or empty remove it
			if(testFace.GetVertexCount() < 3){
				RemoveFaceAt(i);
				i--;
				faceCount--;
			}
		}
	}
}

void decConvexFaceList::SplitByFaceList(const decConvexFaceList &splitterFaceList){
	const int count = splitterFaceList.GetFaceCount();
	int i;
	
	for(i=0; i<count; i++){
		SplitByFace(splitterFaceList, i);
	}
}

void decConvexFaceList::SplitByVolume(const decConvexVolume &volume){
	// convert the volume first into a convex face list
	const int volumeVertexCount = volume.GetVertexCount();
	const int volumeFaceCount = volume.GetFaceCount();
	decConvexFaceList list;
	int i, j;
	
	for(i=0; i<volumeVertexCount; i++){
		list.AddVertex(volume.GetVertexAt(i));
	}
	
	decConvexFace *newFace = NULL;
	try{
		for(i=0; i<volumeFaceCount; i++){
			const decConvexVolumeFace &volumeFace = *volume.GetFaceAt(i);
			const int volumeFaceVertexCount = volumeFace.GetVertexCount();
			
			newFace = new decConvexFace;
			newFace->SetNormal(volumeFace.GetNormal());
			for(j=0; j<volumeFaceVertexCount; j++){
				newFace->AddVertex(volumeFace.GetVertexAt(j));
			}
			list.AddFace(newFace);
			newFace = NULL;
		}
		
	}catch(const deException &){
		if(newFace){
			delete newFace;
		}
		throw;
	}
	
	// split by the generated convex face list
	SplitByFaceList(list);
	
	// remove all faces with centers located in the split volume
	for(i=pFaces.GetCount()-1; i>=0; i--){
		const decConvexFace &face = *((decConvexFace*)pFaces.GetAt(i));
		const decVector faceCenter = face.CalculateCenter(*this);
		
		for(j=0; j<volumeFaceCount; j++){
			const decConvexVolumeFace &volumeFace = *volume.GetFaceAt(j);
			const decVector &volumeFaceNormal = volumeFace.GetNormal();
			const float volumeFaceDist = volumeFaceNormal * volume.GetVertexAt(volumeFace.GetVertexAt(0));
			const float faceDist = volumeFaceNormal * faceCenter;
			
			if(faceDist > volumeFaceDist){
				break;
			}
		}
		
		if(j == volumeFaceCount){
			RemoveFaceAt(i);
		}
	}
}



// Subclassing
////////////////

decConvexFace *decConvexFaceList::CreateFace(decConvexFace *face){
	decConvexFace *newFace = new decConvexFace;
	
	if(face){
		newFace->SetMarker(face->GetMarker());
	}
	
	return newFace;
}



// Private Functions
//////////////////////

// #define DEBUG_ON 1
#ifdef DEBUG_ON
#define DBG(c) c
#else
#define DBG(c)
#endif

void decConvexFaceList::pSplitFaceByEdge(int faceIndex, const decVector &cutEdgeFrom, const decVector &cutEdgeTo){
	DBG(printf("\nsplit face %i by edge (%f,%f,%f)-(%f,%f,%f)\n", faceIndex, cutEdgeFrom.x,
		cutEdgeFrom.y, cutEdgeFrom.z, cutEdgeTo.x, cutEdgeTo.y, cutEdgeTo.z));
	
	// cut edges and determine at the same time if cut edge points are inside
	decConvexFace &face = *((decConvexFace*)pFaces.GetAt(faceIndex));
	const decVector cutEdgeDir = cutEdgeTo - cutEdgeFrom;
	const decVector &faceNormal = face.GetNormal();
	bool insideEdgeFrom = true;
	bool insideEdgeTo = true;
	bool hasSplitEdge = false;
	int i;
	DBG(printf("faceNormal=(%f,%f,%f) cutEdgeDir=(%f,%f,%f)\n", faceNormal.x, faceNormal.y, faceNormal.z, cutEdgeDir.x, cutEdgeDir.y, cutEdgeDir.z));
	
	for(i=0; i<face.GetVertexCount(); i++){
		const decVector &v1 = pVertices[face.GetVertexAt(i)];
		const decVector &v2 = pVertices[face.GetVertexAt((i + 1) % face.GetVertexCount())];
		const decVector edge = v2 - v1;
		const float edgeLen = edge.Length();
		const decVector edgeNorm = edge / edgeLen;
		const decVector planeNormal = faceNormal % edgeNorm;
		const float planeDist = planeNormal * v1;
		
		// test if the edge points are not inside
		const float fromDist = planeNormal * cutEdgeFrom - planeDist;
		const float toDist = planeNormal * cutEdgeTo - planeDist;
		const int fromSide = PLANE_SIDE(fromDist);
		const int toSide = PLANE_SIDE(toDist);
		insideEdgeFrom &= (fromDist > -THRESHOLD_ON_PLANE); // touching counts as inside
		insideEdgeTo &= (toDist > -THRESHOLD_ON_PLANE); // touching counts as inside
		
		// cut edge if intersected by the split edge. parallel does not cut since other edges do this
		const float numer = fromDist;
		const float denom = planeNormal * cutEdgeDir;
		
		if(fabsf(denom) < THRESHOLD_ON_PLANE){
			// if the cut edge is located on the face edge do not even try to cut
			if(fabsf(numer) < THRESHOLD_ON_PLANE){
				return;
			}
			
			// otherwise just ignore this edge
			continue;
		}
		
		const float dist = -numer / denom;
		if(dist < 0.0f || dist > 1.0f){
			continue;
		}
		
		const decVector hitPoint = cutEdgeFrom + cutEdgeDir * dist;
		const float hitDist = edgeNorm * (hitPoint - v1);
		if(hitDist < THRESHOLD_EQUAL || hitDist > edgeLen - THRESHOLD_EQUAL){
			continue;
		}
		
		DBG(printf("split edge %i in face %i cut (%f,%f,%f)\n", i, faceIndex, hitPoint.x, hitPoint.y, hitPoint.z));
		int cutIndex = IndexOfVertex(hitPoint);
		
		if(cutIndex == -1){
			cutIndex = pVertexCount;
			AddVertex(hitPoint);
			
		}else if(face.HasVertex(cutIndex)){
			// the split vertex is already part of the face so this edge is not split
			DBG(printf("cut point exists already and is already part of the face -> drop it\n"));
			continue;
		}
		
		face.InsertVertex(i + 1, cutIndex);
		i++;
		
		// if the cut edge is touching the split point with one end it is possible the other end
		// is outside the face and then no split has to be done
		if(hitPoint.IsEqualTo(cutEdgeFrom, THRESHOLD_EQUAL)){
			if(toSide < 1){
				continue;
			}
			
		}else if(hitPoint.IsEqualTo(cutEdgeTo, THRESHOLD_EQUAL)){
			if(fromSide < 1){
				continue;
			}
		}
		
		hasSplitEdge = true;
	}
	
	DBG(printf("inside test face %i from=%i to=%i\n", faceIndex, insideEdgeFrom?1:0, insideEdgeTo?1:0));
	
	// add the inside edge vertices and keep track of their indices
	int indexCutEdgeFrom = -1;
	int indexCutEdgeTo = -1;
	
	if(insideEdgeFrom){
		indexCutEdgeFrom = IndexOfVertex(cutEdgeFrom);
		
		if(indexCutEdgeFrom == -1){
			indexCutEdgeFrom = pVertexCount;
			AddVertex(cutEdgeFrom);
			
		// if the index exist already in the face drop it to avoid invalid 0-length edge
		}else if(face.HasVertex(indexCutEdgeFrom)){
			DBG(printf("curEdgeFrom exists already and is already part of the face -> drop it\n"));
			indexCutEdgeFrom = -1;
		}
	}
	
	if(insideEdgeTo){
		indexCutEdgeTo = IndexOfVertex(cutEdgeTo);
		
		if(indexCutEdgeTo == -1){
			indexCutEdgeTo = pVertexCount;
			AddVertex(cutEdgeTo);
			
		// if the index exist already in the face drop it to avoid invalid 0-length edge
		}else if(face.HasVertex(indexCutEdgeTo)){
			DBG(printf("curEdgeTo exists already and is already part of the face -> drop it\n"));
			indexCutEdgeTo = -1;
		}
	}
	
	// split up the face if at least one edge vertex is inside. it doesn't matter which
	// edge vertex is used for the splitting as long as it is inside. the chosen vertex
	// acts as pivot point and the edge as pivot axis. 
	if(!(hasSplitEdge || (insideEdgeFrom && insideEdgeTo))){
		return;
	}
	
	const decVector &refAxis = faceNormal % cutEdgeDir.Normalized();
	const float refDist = refAxis * cutEdgeFrom;
	
	// splitting the face by an edge can result in up to four faces which each can be
	// empty. the front face contains all points with a side of >= 0. the back face
	// contains all points with a side of <= 0. the two filler faces contain each the
	// two consequtive vertices crossing an edge and the respective split edge end
	// point. crossing the edge means one vertex is < 0 and the other > 0. if one of
	// the vertices is 0 or the respective cut edge end is not inside the respective
	// filler face does not exist.
	decConvexFace *newFaceBack = NULL;
	decConvexFace *newFaceFiller = NULL;
	
	int lastVertex = face.GetVertexAt(face.GetVertexCount() - 1);
	int lastSide = PLANE_SIDE(refAxis * pVertices[lastVertex] - refDist);
	DBG(for(i=0; i<face.GetVertexCount(); i++) printf("state: face %i vertex %i(%i) (%f,%f,%f)\n", faceIndex, i, face.GetVertexAt(i),
		pVertices[face.GetVertexAt(i)].x, pVertices[face.GetVertexAt(i)].y, pVertices[face.GetVertexAt(i)].z);)
	
	try{
		for(i=0; i<face.GetVertexCount(); i++){
			const int curVertex = face.GetVertexAt(i);
			const int curSide = PLANE_SIDE(refAxis * pVertices[curVertex] - refDist);
			DBG(printf("corner=%i last=(%i,%i) cur=(%i,%i) indexCutEdgeFrom=%i indexCutEdgeTo=%i\n",
				i, lastVertex, lastSide, curVertex, curSide, indexCutEdgeFrom, indexCutEdgeTo));
			
			// if the last vertex is < 0 and the cur vertex > 0 the cut edge has been cross at the
			// cutEdgeTo closer end point.
			if(lastSide < 0 && curSide > 0){
				// add filler face if the last vertex, cur vertex and cutEdgeTo are not co-linear.
				// this is the case if insideEdgeTo is true
				if(indexCutEdgeTo != -1){
					DBG(printf("add filler face at cutEdgeTo with vertices %i, %i and %i\n", indexCutEdgeTo, lastVertex, curVertex));
					newFaceFiller = CreateFace(&face);
					newFaceFiller->SetNormal(faceNormal);
					newFaceFiller->SetMarker(face.GetMarker());
					newFaceFiller->AddVertex(indexCutEdgeTo);
					newFaceFiller->AddVertex(lastVertex);
					newFaceFiller->AddVertex(curVertex);
					AddFace(newFaceFiller);
					newFaceFiller = NULL;
				}
				
				// if the end point has been added to the face list add it also to the front and back face
				if(indexCutEdgeTo != -1){
					// add cutEdgeTo to the back face
					if(!newFaceBack){
						DBG(printf("crossTo: create back face\n"));
						newFaceBack = CreateFace(&face);
						newFaceBack->SetNormal(faceNormal);
						newFaceBack->SetMarker(face.GetMarker());
					}
					DBG(printf("crossTo: add indexCutEdgeTo %i to back face\n", indexCutEdgeTo));
					newFaceBack->AddVertex(indexCutEdgeTo);
					
					// add cutEdgeTo to the front face
					DBG(printf("crossTo: insert indexCutEdgeTo %i into front face at %i\n", indexCutEdgeTo, i));
					face.InsertVertex(i, indexCutEdgeTo);
					i++;
				}
			}
			
			// if the last vertex is > 0 and the cur vertex < 0 the cut edge has been cross at the
			// cutEdgeFrom closer end point.
			if(lastSide > 0 && curSide < 0){
				// add filler face if the last vertex, cur vertex and cutEdgeFrom are not co-linear.
				// this is the case if insideEdgeFrom is true
				if(indexCutEdgeFrom != -1){
					DBG(printf("add filler face at cutEdgeFrom with vertices %i, %i and %i\n", indexCutEdgeFrom, lastVertex, curVertex));
					newFaceFiller = CreateFace(&face);
					newFaceFiller->SetNormal(faceNormal);
					newFaceFiller->SetMarker(face.GetMarker());
					newFaceFiller->AddVertex(indexCutEdgeFrom);
					newFaceFiller->AddVertex(lastVertex);
					newFaceFiller->AddVertex(curVertex);
					AddFace(newFaceFiller);
					newFaceFiller = NULL;
				}
				
				// if the from point has been added to the face list add it also to the front and back face
				if(indexCutEdgeFrom != -1){
					// add cutEdgeFrom to the back face
					if(!newFaceBack){
						DBG(printf("crossFrom: create back face\n"));
						newFaceBack = CreateFace(&face);
						newFaceBack->SetNormal(faceNormal);
						newFaceBack->SetMarker(face.GetMarker());
					}
					DBG(printf("crossFrom: add indexCutEdgeFrom %i to back face\n", indexCutEdgeFrom));
					newFaceBack->AddVertex(indexCutEdgeFrom);
					
					// add cutEdgeFrom to the back face
					DBG(printf("crossFrom: insert indexCutEdgeFrom %i into front face at %i\n", indexCutEdgeFrom, i));
					face.InsertVertex(i, indexCutEdgeFrom);
					i++;
				}
			}
			
			// if the side is <= 0 add the vertex to the back face. this has to come after the
			// crossing checks above to keep the ordering correct
			if(curSide <= 0){
				if(!newFaceBack){
					DBG(printf("curSide<=0: create back face\n"));
					newFaceBack = CreateFace(&face);
					newFaceBack->SetNormal(faceNormal);
					newFaceBack->SetMarker(face.GetMarker());
				}
				DBG(printf("curSide<=0: add vertex %i to back face\n", curVertex));
				newFaceBack->AddVertex(curVertex);
			}
			
			// if the side is strictly < 0 remove the vertex from the front face since
			// it belongs now entirely to the back face
			if(curSide < 0){
				DBG(printf("curSide<0: remove vertex %i from front face at %i\n", curVertex, i));
				face.RemoveVertexFrom(i);
				i--;
			}
			
			// keep track of the last vertex
			lastVertex = curVertex;
			lastSide = curSide;
		}
		
		// add the new faces if they have enough corners
		if(newFaceBack && newFaceBack->GetVertexCount() > 2){
			AddFace(newFaceBack);
			newFaceBack = NULL;
		}
		
	}catch(const deException &){
		if(newFaceFiller){
			delete newFaceFiller;
		}
		if(newFaceBack){
			delete newFaceBack;
		}
		throw;
	}
}
