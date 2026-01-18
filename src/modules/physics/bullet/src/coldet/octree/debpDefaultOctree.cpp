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
}

debpDefaultOctree::~debpDefaultOctree(){
	RemoveAllElements();
}



// Management
///////////////

debpOctree *debpDefaultOctree::CreateOctree(int octant) const{
	decVector halfSize = GetHalfSize() * 0.5f;
	const decVector &center = GetCenter();
	debpOctree *node = (debpDefaultOctree*)nullptr;
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
		if(!nextNode) break;
		curNode = nextNode;
	}
	
	((debpDefaultOctree*)curNode)->AddElement(element);
	return (debpDefaultOctree*)curNode;
}



// Elements
/////////////

void *debpDefaultOctree::GetElementAt(int index) const{
	return pElements.GetAt(index);
}

int debpDefaultOctree::IndexOfElement(void *element) const{
	return pElements.IndexOf(element);
}

void debpDefaultOctree::AddElement(void *element){
	if(!element) DETHROW(deeInvalidParam);
	
	pElements.Add(element);
}

void debpDefaultOctree::RemoveElement(void *element){
	const int index = IndexOfElement(element);
	if(index == -1) DETHROW(deeInvalidParam);
	
	pElements.RemoveFrom(index);
}

void debpDefaultOctree::RemoveAllElements(){
	pElements.RemoveAll();
}
