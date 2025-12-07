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

#include "decDefaultOctree.h"
#include "../../exceptions.h"



// Class decDefaultOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

decDefaultOctree::decDefaultOctree(const decVector &center, const decVector &halfSize) : decOctree(center, halfSize){
	pElements = NULL;
	pElementCount = 0;
	pElementSize = 0;
}

decDefaultOctree::~decDefaultOctree(){
	RemoveAllElements();
	if(pElements) delete [] pElements;
}



// Management
///////////////

decOctree *decDefaultOctree::CreateOctree(int octant) const{
	decVector halfSize = GetHalfSize() * 0.5f;
	const decVector &center = GetCenter();
	decOctree *node = (decDefaultOctree*)NULL;
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
	node = (decDefaultOctree*)new decDefaultOctree(nc, halfSize);
	if(! node) DETHROW(deeOutOfMemory);
	return node;
}

void decDefaultOctree::ClearNodeContent(){
	RemoveAllElements();
}

decDefaultOctree *decDefaultOctree::InsertIntoTree(void *element, const decVector &boxCenter, const decVector &boxHalfSize, int maxDepth){
	decOctree *curNode = this;
	decOctree *nextNode;
	int d;
	
	for(d=0; d<maxDepth; d++){
		nextNode = curNode->GetNodeAtBox(boxCenter, boxHalfSize);
		if(! nextNode) break;
		curNode = nextNode;
	}
	
	((decDefaultOctree*)curNode)->AddElement(element);
	return (decDefaultOctree*)curNode;
}



// Elements
/////////////

void *decDefaultOctree::GetElementAt(int index) const{
	if(index < 0 || index >= pElementCount) DETHROW(deeInvalidParam);
	return pElements[index];
}

int decDefaultOctree::IndexOfElement(void *element) const{
	int i;
	for(i=0; i<pElementCount; i++){
		if(pElements[i] == element) return i;
	}
	return -1;
}

void decDefaultOctree::AddElement(void *element){
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

void decDefaultOctree::RemoveElement(void *element){
	int i, index = IndexOfElement(element);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pElementCount; i++) pElements[i - 1] = pElements[i];
	pElements[pElementCount - 1] = NULL;
	pElementCount--;
}

void decDefaultOctree::RemoveAllElements(){
	pElementCount = 0;
}
