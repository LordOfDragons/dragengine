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
#include <string.h>
#include <stdlib.h>
#include "decDefaultQuadtree.h"
#include "../exceptions.h"



// Class decDefaultQuadtree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

decDefaultQuadtree::decDefaultQuadtree(const decVector2 &center, const decVector2 &halfSize) : decQuadtree(center, halfSize){
	pElements = NULL;
	pElementCount = 0;
	pElementSize = 0;
}

decDefaultQuadtree::~decDefaultQuadtree(){
	RemoveAllElements();
	if(pElements) delete [] pElements;
}



// Management
///////////////

decQuadtree *decDefaultQuadtree::CreateQuadtree(int quadrant) const{
	decVector2 halfSize = GetHalfSize() * 0.5f;
	const decVector2 &center = GetCenter();
	decQuadtree *node = (decDefaultQuadtree*)NULL;
	decVector2 nc;
	
	// determine the smallest and largest coordinates
	if((quadrant & 2) == 2){
		nc.x = center.x + halfSize.x;
	}else{
		nc.x = center.x - halfSize.x;
	}
	if((quadrant & 1) == 1){
		nc.y = center.y + halfSize.y;
	}else{
		nc.y = center.y - halfSize.y;
	}
	
	// create child node
	node = (decDefaultQuadtree*)new decDefaultQuadtree(nc, halfSize);
	if(!node) DETHROW(deeOutOfMemory);
	return node;
}

void decDefaultQuadtree::ClearNodeContent(){
	RemoveAllElements();
}

decDefaultQuadtree *decDefaultQuadtree::InsertIntoTree(void *element, const decVector2 &boxCenter, const decVector2 &boxHalfSize, int maxDepth){
	decQuadtree *curNode = this;
	decQuadtree *nextNode;
	int d;
	
	for(d=0; d<maxDepth; d++){
		nextNode = curNode->GetNodeAtBox(boxCenter, boxHalfSize);
		if(!nextNode) break;
		curNode = nextNode;
	}
	
	((decDefaultQuadtree*)curNode)->AddElement(element);
	return (decDefaultQuadtree*)curNode;
}



// Elements
/////////////

void *decDefaultQuadtree::GetElementAt(int index) const{
	if(index < 0 || index >= pElementCount) DETHROW(deeInvalidParam);
	return pElements[index];
}

int decDefaultQuadtree::IndexOfElement(void *element) const{
	int i;
	for(i=0; i<pElementCount; i++){
		if(pElements[i] == element) return i;
	}
	return -1;
}

void decDefaultQuadtree::AddElement(void *element){
	if(!element) DETHROW(deeInvalidParam);
	
	if(pElementCount == pElementSize){
		int i, newSize = pElementSize * 3 / 2 + 1;
		void **newArray = new void*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
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

void decDefaultQuadtree::RemoveElement(void *element){
	int i, index = IndexOfElement(element);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pElementCount; i++) pElements[i - 1] = pElements[i];
	pElements[pElementCount - 1] = NULL;
	pElementCount--;
}

void decDefaultQuadtree::RemoveAllElements(){
	pElementCount = 0;
}
