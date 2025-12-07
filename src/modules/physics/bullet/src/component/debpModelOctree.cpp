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

#include "debpModelOctree.h"

#include <dragengine/common/exceptions.h>



// Class debpModelOctree
//////////////////////////

// Constructor, destructor
////////////////////////////

debpModelOctree::debpModelOctree(const decDVector &center, const decDVector &halfSize) : debpDOctree(center, halfSize){
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

debpModelOctree::debpModelOctree(const decVector &center, const decVector &halfSize) :
debpDOctree(decDVector(center), decDVector(halfSize)){
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

debpModelOctree::~debpModelOctree(){
	RemoveAllFaces();
	if(pFaces) delete [] pFaces;
}



// Management
///////////////

debpDOctree *debpModelOctree::CreateOctree(int octant) const{
	const decDVector halfSize(GetHalfSize() * 0.5);
	const decDVector &center = GetCenter();
	debpDOctree *node;
	decDVector nc;
	
	// determine the smallest and largest coordinates
	if((octant & 4) == 4){
		nc.x = center.x + halfSize.x;
		
	}else{
		nc.x = center.x - halfSize.x;
	}
	
	if((octant & 2) == 2){
		nc.y = center.y + halfSize.y;
		
	}else{
		nc.y = center.y - halfSize.y;
	}
	
	if((octant & 1) == 1){
		nc.z = center.z + halfSize.z;
		
	}else{
		nc.z = center.z - halfSize.z;
	}
	
	// create child node
	node = new debpModelOctree(nc, halfSize);
	if(!node) DETHROW(deeOutOfMemory);
	
	return node;
}

void debpModelOctree::ClearNodeContent(){
	RemoveAllFaces();
}

void debpModelOctree::InsertFaceIntoTree(int face, const decVector &center, const decVector &halfSize, int maxDepth){
	pGetNodeFor(center, halfSize, maxDepth)->AddFace(face);
}

void debpModelOctree::ClearFaces(){
	debpDOctree *node;
	int n;
	
	RemoveAllFaces();
	
	for(n=0; n<8; n++){
		node = GetNodeAt(n);
		if(node){
			((debpModelOctree*)node)->ClearFaces();
		}
	}
}



int debpModelOctree::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount) DETHROW(deeInvalidParam);
	
	return pFaces[index];
}

int debpModelOctree::IndexOfFace(int face) const{
	int f;
	
	for(f=0; f<pFaceCount; f++){
		if(pFaces[f] == face){
			return f;
		}
	}
	
	return -1;
}

void debpModelOctree::AddFace(int face){
	if(face < 0) DETHROW(deeInvalidParam);
	
	if(pFaceCount == pFaceSize){
		int newSize = pFaceSize * 3 / 2 + 1;
		int *newArray = new int[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pFaces){
			memcpy(newArray, pFaces, sizeof(int) * pFaceSize);
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaces[pFaceCount] = face;
	pFaceCount++;
}

void debpModelOctree::RemoveFace(int face){
	int f, index = IndexOfFace(face);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(f=index+1; f<pFaceCount; f++){
		pFaces[f - 1] = pFaces[f];
	}
	pFaceCount--;
}

void debpModelOctree::RemoveAllFaces(){
	pFaceCount = 0;
}



// Private Functions
//////////////////////

debpModelOctree *debpModelOctree::pGetNodeFor(const decVector &center, const decVector &halfSize, int maxDepth){
	const decDVector dcenter(center);
	const decDVector dhalfSize(halfSize);
	debpDOctree *curNode = this;
	debpDOctree *nextNode;
	int d;
	
	for(d=0; d<maxDepth; d++){
		nextNode = curNode->GetNodeAtBox(dcenter, dhalfSize);
		if(!nextNode) break;
		
		curNode = nextNode;
	}
	
	return (debpModelOctree*)curNode;
}
