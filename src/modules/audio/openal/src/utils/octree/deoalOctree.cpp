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

#include "deoalOctree.h"
#include "deoalOctreeVisitor.h"
#include "../collision/deoalCollisionBox.h"
#include "../collision/deoalCollisionDetection.h"
#include <dragengine/common/exceptions.h>



// Class deoalOctree
////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalOctree::deoalOctree(const decVector &center, const decVector &halfSize){
	int i;
	
	for(i=0; i<8; i++) pNodes[i] = NULL;
	pCenter = center;
	pHalfSize = halfSize;
	pParent = NULL;
}

deoalOctree::~deoalOctree(){
	int i;
	
	for(i=0; i<8; i++){
		if(pNodes[i]) delete pNodes[i];
	}
}



// Management
///////////////

void deoalOctree::SetParent(deoalOctree *parent){
	pParent = parent;
}

deoalOctree *deoalOctree::GetNodeAt(int octant) const{
	if(octant < 0 || octant > 7) DETHROW(deeInvalidParam);
	return pNodes[octant];
}

void deoalOctree::SetNodeAt(int octant, deoalOctree *node){
	if(octant < 0 || octant > 7) DETHROW(deeInvalidParam);
	if(pNodes[octant] != node){
		if(pNodes[octant]) delete pNodes[octant];
		pNodes[octant] = node;
	}
}

deoalOctree *deoalOctree::GetNodeAtBox(const decVector &boxCenter, const decVector &boxHalfSize){
	int octant = FindOctantAtBox(boxCenter, boxHalfSize);
	
	// if we found no matching octant return NULL
	if(octant == eoNotFound) return NULL;
	
	// if the node does not exist create it
	if(!pNodes[octant]){
		pNodes[octant] = CreateOctree(octant);
		pNodes[octant]->SetParent(this);
	}
	
	// return the node that we found
	return pNodes[octant];
}

deoalOctree *deoalOctree::FindNodeAtBox(const decVector &boxCenter, const decVector &boxHalfSize) const{
	int octant = FindOctantAtBox(boxCenter, boxHalfSize);
	
	// if we found no matching octant return NULL
	if(octant == eoNotFound) return NULL;
	
	// return the node that we found
	return pNodes[octant];
}

int deoalOctree::FindOctantAtBox(const decVector &boxCenter, const decVector &boxHalfSize) const{
	int octant = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit. if an
	// axis does not split no octant is found.
	if(boxCenter.x - boxHalfSize.x >= pCenter.x){
		octant |= 4;
	}else if(boxCenter.x + boxHalfSize.x >= pCenter.x){
		return eoNotFound;
	}
	if(boxCenter.y - boxHalfSize.y >= pCenter.y){
		octant |= 2;
	}else if(boxCenter.y + boxHalfSize.y >= pCenter.y){
		return eoNotFound;
	}
	if(boxCenter.z - boxHalfSize.z >= pCenter.z){
		octant |= 1;
	}else if(boxCenter.z + boxHalfSize.z >= pCenter.z){
		return eoNotFound;
	}
	
	// return the found octant
	return octant;
}

bool deoalOctree::ContainsBox(const decVector &boxCenter, const decVector &boxHalfSize) const{
	return (boxCenter - boxHalfSize) >= (pCenter - pHalfSize)
		&& (boxCenter + boxHalfSize) < (pCenter + pHalfSize);
}

deoalOctree *deoalOctree::FindNodeAtPoint(const decVector &point) const{
	int octant = FindOctantAtPoint(point);
	
	// if we found no matching octant return NULL
	if(octant == eoNotFound) return NULL;
	
	// return the node that we found
	return pNodes[octant];
}

int deoalOctree::FindOctantAtPoint(const decVector &point) const{
	int bitmask = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit.
	if(point.x >= pCenter.x) bitmask |= 4;
	if(point.y >= pCenter.y) bitmask |= 2;
	if(point.z >= pCenter.z) bitmask |= 1;
	
	// return the found octant
	return bitmask;
}

bool deoalOctree::ContainsPoint(const decVector &point) const{
	return point >= (pCenter - pHalfSize) &&  point < (pCenter + pHalfSize);
}



deoalOctree *deoalOctree::SearchTreeForBox(const decVector &boxCenter, const decVector &boxHalfSize) const{
	deoalOctree *nextNode = FindNodeAtBox(boxCenter, boxHalfSize);
	deoalOctree *curNode = (deoalOctree*)this;
	
	while(nextNode){
		curNode = nextNode;
		nextNode = curNode->FindNodeAtBox(boxCenter, boxHalfSize);
	}
	
	return curNode;
}

deoalOctree *deoalOctree::SearchTreeForPoint(const decVector &point) const{
	deoalOctree *nextNode = FindNodeAtPoint(point);
	deoalOctree *curNode = (deoalOctree*)this;
	
	while(nextNode){
		curNode = nextNode;
		nextNode = curNode->FindNodeAtPoint(point);
	}
	
	return curNode;
}

void deoalOctree::VisitNodes(deoalOctreeVisitor *visitor){
	if(!visitor) DETHROW(deeInvalidParam);
	int i;
	
	// visit
	visitor->VisitNode(this, deoalCollisionDetection::eirInside);
	
	// visit each child node
	for(i=0; i<8; i++){
		if(pNodes[i]) pNodes[i]->VisitNodes(visitor);
	}
}

void deoalOctree::VisitNodesColliding(deoalOctreeVisitor *visitor, deoalCollisionVolume *volume){
	if(!visitor || !volume) DETHROW(deeInvalidParam);
	deoalCollisionBox colBox(pCenter, pHalfSize);
	int i;
	
	// exit if this node is not in the collision volume
	if(!volume->BoxHitsVolume(&colBox)) return;
	
	// visit
	visitor->VisitNode(this, deoalCollisionDetection::eirPartial);
	
	// test each child node
	for(i=0; i<8; i++){
		if(pNodes[i]) pNodes[i]->VisitNodesColliding(visitor, volume);
	}
}

void deoalOctree::VisitNodesColliding(deoalOctreeVisitor *visitor, const decVector &boxMinExtend, const decVector &boxMaxExtend){
	if(!visitor) DETHROW(deeInvalidParam);
	int i, result;
	
	result = deoalCollisionDetection::AABoxIntersectsAABox(pCenter - pHalfSize, pCenter + pHalfSize, boxMinExtend, boxMaxExtend);
	
	if(result == deoalCollisionDetection::eirOutside) return;
	
	visitor->VisitNode(this, result);
	
	if(result == deoalCollisionDetection::eirInside){
		for(i=0; i<8; i++){
			if(pNodes[i]){
				pNodes[i]->VisitNodes(visitor);
			}
		}
		
	}else{
		for(i=0; i<8; i++){
			if(pNodes[i]){
				pNodes[i]->VisitNodesColliding(visitor, boxMinExtend, boxMaxExtend);
			}
		}
	}
}

void deoalOctree::ClearTree(bool clearNodes){
	int i;
	
	ClearNodeContent();
	
	for(i=0; i<8; i++){
		if(pNodes[i]){
			pNodes[i]->ClearTree(clearNodes);
			if(clearNodes){
				delete pNodes[i];
				pNodes[i] = NULL;
			}
		}
	}
}
