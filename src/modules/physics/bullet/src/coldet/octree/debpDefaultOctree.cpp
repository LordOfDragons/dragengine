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

#include "debpDefaultOctree.h"

#include <dragengine/common/exceptions.h>



// Class debpDefaultOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

debpDefaultOctree::debpDefaultOctree(const decVector &center, const decVector &halfSize) : debpOctree(center, halfSize){
	pElements = NULL;
	pElementCount = 0;
	pElementSize = 0;
}

debpDefaultOctree::~debpDefaultOctree(){
	RemoveAllElements();
	if(pElements) delete [] pElements;
}



// Management
///////////////

debpOctree *debpDefaultOctree::CreateOctree(int octant) const{
	decVector halfSize = GetHalfSize() * 0.5f;
	const decVector &center = GetCenter();
	debpOctree *node = (debpDefaultOctree*)NULL;
	decVector nc;
	
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
	node = (debpDefaultOctree*)new debpDefaultOctree(nc, halfSize);
	if(! node) DETHROW(deeOutOfMemory);
	return node;
}

void debpDefaultOctree::ClearNodeContent(){
	RemoveAllElements();
}

debpDefaultOctree *debpDefaultOctree::InsertIntoTree(void *element, const decVector &boxCenter, const decVector &boxHalfSize, int maxDepth){
	debpOctree *curNode = this;
	debpOctree *nextNode;
	int d;
	
	for(d=0; d<maxDepth; d++){
		nextNode = curNode->GetNodeAtBox(boxCenter, boxHalfSize);
		if(! nextNode) break;
		curNode = nextNode;
	}
	
	((debpDefaultOctree*)curNode)->AddElement(element);
	return (debpDefaultOctree*)curNode;
}



// Elements
/////////////

void *debpDefaultOctree::GetElementAt(int index) const{
	if(index < 0 || index >= pElementCount) DETHROW(deeInvalidParam);
	return pElements[index];
}

int debpDefaultOctree::IndexOfElement(void *element) const{
	int i;
	for(i=0; i<pElementCount; i++){
		if(pElements[i] == element) return i;
	}
	return -1;
}

void debpDefaultOctree::AddElement(void *element){
	if(! element) DETHROW(deeInvalidParam);
	
	if(pElementCount == pElementSize){
		int i, newSize = pElementSize * 3 / 2 + 1;
		void **newArray = new void*[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		if(pElements){
			for(i=0; i<pElementSize; i++) newArray[i] = pElements[i];
			delete [] pElements;
		}
		pElements = newArray;
		pElementSize = newSize;
	}
	
	pElements[pElementCount] = element;
	pElementCount++;
}

void debpDefaultOctree::RemoveElement(void *element){
	int i, index = IndexOfElement(element);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pElementCount; i++) pElements[i - 1] = pElements[i];
	pElements[pElementCount - 1] = NULL;
	pElementCount--;
}

void debpDefaultOctree::RemoveAllElements(){
	pElementCount = 0;
}
