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
#include <string.h>
#include <stdlib.h>
#include "decDefaultBSPTree.h"
#include "decBSPTreeFace.h"
#include "../exceptions.h"



// Class decDefaultBSPTree
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

decDefaultBSPTree::decDefaultBSPTree(const decVector &faceNormal, float distance) : decBSPTree(faceNormal, distance){
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

decDefaultBSPTree::~decDefaultBSPTree(){
	RemoveAllFaces();
	if(pFaces) delete [] pFaces;
}



// Management
///////////////

decBSPTree *decDefaultBSPTree::CreateBSPTree(const decVector &faceNormal, float distance) const{
	decDefaultBSPTree *bsptree = new decDefaultBSPTree(faceNormal, distance);
	if(!bsptree) DETHROW(deeOutOfMemory);
	
	return bsptree;
}

void decDefaultBSPTree::ClearNodeContent(){
	RemoveAllFaces();
}

void decDefaultBSPTree::AddCoplanarFace(const decBSPTreeFace &face){
	int v, vertexCount = face.GetVertexCount();
	decBSPTreeFace *newFace = NULL;
	
	try{
		newFace = new decBSPTreeFace;
		if(!newFace) DETHROW(deeOutOfMemory);
		
		for(v=0; v<vertexCount; v++){
			newFace->AddVertex(face.GetVertexAt(v));
		}
		
		newFace->SetUserData(face.GetUserData());
		
		AddFace(newFace);
		
	}catch(const deException &){
		if(newFace) delete newFace;
		throw;
	}
}



// Faces
//////////

decBSPTreeFace *decDefaultBSPTree::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount) DETHROW(deeInvalidParam);
	
	return pFaces[index];
}

int decDefaultBSPTree::IndexOfFace(decBSPTreeFace *face) const{
	int i;
	
	for(i=0; i<pFaceCount; i++){
		if(pFaces[i] == face) return i;
	}
	
	return -1;
}

void decDefaultBSPTree::AddFace(decBSPTreeFace *face){
	if(!face) DETHROW(deeInvalidParam);
	
	if(pFaceCount == pFaceSize){
		int newSize = pFaceSize * 3 / 2 + 1;
		decBSPTreeFace **newArray = new decBSPTreeFace*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pFaces){
			memcpy(newArray, pFaces, sizeof(decBSPTreeFace*) * pFaceSize);
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaces[pFaceCount] = face;
	pFaceCount++;
}

void decDefaultBSPTree::RemoveFace(decBSPTreeFace *face){
	int i, index = IndexOfFace(face);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pFaceCount; i++){
		pFaces[i - 1] = pFaces[i];
	}
	pFaces[pFaceCount - 1] = NULL;
	pFaceCount--;
	
	delete face;
}

void decDefaultBSPTree::RemoveAllFaces(){
	while(pFaceCount > 0){
		pFaceCount--;
		delete pFaces[pFaceCount];
	}
}
