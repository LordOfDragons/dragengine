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



// Class decConvexVolume
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

decConvexVolume::decConvexVolume(){
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

decConvexVolume::~decConvexVolume(){
	RemoveAllFaces();
	if(pFaces) delete [] pFaces;
	RemoveAllVertices();
	if(pVertices) delete [] pVertices;
}



// Vertex Management
//////////////////////

const decVector &decConvexVolume::GetVertexAt(int index) const{
	if(index < 0 || index >= pVertexCount){
		DETHROW(deeInvalidParam);
	}
	return pVertices[index];
}

bool decConvexVolume::HasVertex(const decVector &vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex.IsEqualTo(pVertices[i])) return true;
	}
	
	return false;
}

int decConvexVolume::IndexOfVertex(const decVector &vertex) const{
	int i;
	
	for(i=0; i<pVertexCount; i++){
		if(vertex.IsEqualTo(pVertices[i])) return i;
	}
	
	return -1;
}

void decConvexVolume::AddVertex(const decVector &vertex){
	if(pVertexCount == pVertexSize){
		int i, newSize = pVertexSize * 3 / 2 + 1;
		decVector *newArray = new decVector[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		
		if(pVertices){
			for(i=0; i<pVertexSize; i++) newArray[i] = pVertices[i];
			delete [] pVertices;
		}
		
		pVertices = newArray;
		pVertexSize = newSize;
	}
	
	pVertices[pVertexCount] = vertex;
	pVertexCount++;
}

void decConvexVolume::RemoveVertex(int index){
	if(index < 0 || index >= pVertexCount) DETHROW(deeInvalidParam);
	int i;
	
	for(i=index+1; i<pVertexCount; i++) pVertices[i - 1] = pVertices[i];
	pVertexCount--;
}

void decConvexVolume::RemoveAllVertices(){
	pVertexCount = 0;
}



// Management
///////////////

decConvexVolumeFace *decConvexVolume::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount) DETHROW(deeInvalidParam);
	return pFaces[index];
}

bool decConvexVolume::HasFace(decConvexVolumeFace *face) const{
	int i;
	
	if(face){
		for(i=0; i<pFaceCount; i++){
			if(pFaces[i] == face) return true;
		}
	}
	
	return false;
}

int decConvexVolume::IndexOfFace(decConvexVolumeFace *face) const{
	int i;
	
	if(face){
		for(i=0; i<pFaceCount; i++){
			if(pFaces[i] == face) return i;
		}
	}
	
	return -1;
}

void decConvexVolume::AddFace(decConvexVolumeFace *face){
	if(! face || HasFace(face)) DETHROW(deeInvalidParam);
	
	if(pFaceCount == pFaceSize){
		int i, newSize = pFaceSize * 3 / 2 + 1;
		decConvexVolumeFace **newArray = new decConvexVolumeFace*[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		
		if(pFaces){
			for(i=0; i<pFaceSize; i++) newArray[i] = pFaces[i];
			delete [] pFaces;
		}
		
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaces[pFaceCount] = face;
	pFaceCount++;
}

void decConvexVolume::RemoveFace(decConvexVolumeFace *face){
	int i, index = IndexOfFace(face);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pFaceCount; i++) pFaces[i - 1] = pFaces[i];
	pFaceCount--;
	
	delete face;
}

void decConvexVolume::RemoveAllFaces(){
	while(pFaceCount > 0){
		pFaceCount--;
		delete pFaces[pFaceCount];
	}
}



// Management
///////////////

void decConvexVolume::SetEmpty(){
	RemoveAllFaces();
	RemoveAllVertices();
}

void decConvexVolume::SetToCube(const decVector &halfSize){
	if(halfSize < decVector(0.0f, 0.0f, 0.0f)) DETHROW(deeInvalidParam);
	decConvexVolumeFace *face = NULL;
	
	// remove all faces and vertices
	SetEmpty();
	
	// add the cube corners
	AddVertex(decVector(-halfSize.x, halfSize.y, -halfSize.z));
	AddVertex(decVector(halfSize.x, halfSize.y, -halfSize.z));
	AddVertex(decVector(halfSize.x, -halfSize.y, -halfSize.z));
	AddVertex(decVector(-halfSize.x, -halfSize.y, -halfSize.z));
	AddVertex(decVector(-halfSize.x, halfSize.y, halfSize.z));
	AddVertex(decVector(halfSize.x, halfSize.y, halfSize.z));
	AddVertex(decVector(halfSize.x, -halfSize.y, halfSize.z));
	AddVertex(decVector(-halfSize.x, -halfSize.y, halfSize.z));
	
	// add faces
	try{
		// add front face
		face = new decConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		face->SetNormal(decVector(0.0f, 0.0f, -1.0f));
		face->AddVertex(0);
		face->AddVertex(1);
		face->AddVertex(2);
		face->AddVertex(3);
		AddFace(face);
		face = NULL;
		
		// add right face
		face = new decConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		face->SetNormal(decVector(1.0f, 0.0f, 0.0f));
		face->AddVertex(1);
		face->AddVertex(5);
		face->AddVertex(6);
		face->AddVertex(2);
		AddFace(face);
		face = NULL;
		
		// add back face
		face = new decConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		face->SetNormal(decVector(0.0f, 0.0f, 1.0f));
		face->AddVertex(5);
		face->AddVertex(4);
		face->AddVertex(7);
		face->AddVertex(6);
		AddFace(face);
		face = NULL;
		
		// add left face
		face = new decConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		face->SetNormal(decVector(-1.0f, 0.0f, 0.0f));
		face->AddVertex(4);
		face->AddVertex(0);
		face->AddVertex(3);
		face->AddVertex(7);
		AddFace(face);
		face = NULL;
		
		// add top face
		face = new decConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		face->SetNormal(decVector(0.0f, 1.0f, 0.0f));
		face->AddVertex(4);
		face->AddVertex(5);
		face->AddVertex(1);
		face->AddVertex(0);
		AddFace(face);
		face = NULL;
		
		// add bottom face
		face = new decConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		face->SetNormal(decVector(0.0f, -1.0f, 0.0f));
		face->AddVertex(3);
		face->AddVertex(2);
		face->AddVertex(6);
		face->AddVertex(7);
		AddFace(face);
		
	}catch(const deException &){
		if(face) delete face;
		throw;
	}
}

void decConvexVolume::Move(const decVector &direction){
	int v;
	
	for(v=0; v<pVertexCount; v++){
		pVertices[v] += direction;
	}
}
