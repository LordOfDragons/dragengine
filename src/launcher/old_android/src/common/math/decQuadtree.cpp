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
#include "decQuadtree.h"
#include "decQuadtreeVisitor.h"
#include "collision/decCollisionBox.h"
#include "../exceptions.h"



// Class decQuadtree
////////////////////

// Constructors and Destructors
/////////////////////////////////

decQuadtree::decQuadtree(const decVector2 &center, const decVector2 &halfSize){
	int i;
	
	for(i=0; i<4; i++) pNodes[i] = NULL;
	pCenter = center;
	pHalfSize = halfSize;
	pParent = NULL;
}

decQuadtree::~decQuadtree(){
	int i;
	
	for(i=0; i<4; i++){
		if(pNodes[i]) delete pNodes[i];
	}
}



// Management
///////////////

void decQuadtree::SetParent(decQuadtree *parent){
	pParent = parent;
}

decQuadtree *decQuadtree::GetNodeAt(int quadrant) const{
	if(quadrant < 0 || quadrant > 3) DETHROW(deeInvalidParam);
	return pNodes[quadrant];
}

void decQuadtree::SetNodeAt(int quadrant, decQuadtree *node){
	if(quadrant < 0 || quadrant > 3) DETHROW(deeInvalidParam);
	if(pNodes[quadrant] != node){
		if(pNodes[quadrant]) delete pNodes[quadrant];
		pNodes[quadrant] = node;
	}
}

decQuadtree *decQuadtree::GetNodeAtBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize){
	int quadrant = FindQuadrantAtBox(boxCenter, boxHalfSize);
	
	// if we found no matching quadrant return NULL
	if(quadrant == eqNotFound) return NULL;
	
	// if the node does not exist create it
	if(! pNodes[quadrant]){
		pNodes[quadrant] = CreateQuadtree(quadrant);
		pNodes[quadrant]->SetParent(this);
	}
	
	// return the node that we found
	return pNodes[quadrant];
}

decQuadtree *decQuadtree::GetNodeAtPoint(const decVector2 &point){
	int quadrant = FindQuadrantAtPoint(point);
	
	// if we found no matching quadrant return NULL
	if(quadrant == eqNotFound) return NULL;
	
	// if the node does not exist create it
	if(! pNodes[quadrant]){
		pNodes[quadrant] = CreateQuadtree(quadrant);
		pNodes[quadrant]->SetParent(this);
	}
	
	// return the node that we found
	return pNodes[quadrant];
}

decQuadtree *decQuadtree::FindNodeAtBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const{
	int quadrant = FindQuadrantAtBox(boxCenter, boxHalfSize);
	
	// if we found no matching quadrant return NULL
	if(quadrant == eqNotFound) return NULL;
	
	// return the node that we found
	return pNodes[quadrant];
}

int decQuadtree::FindQuadrantAtBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const{
	int quadrant = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit. if an
	// axis does not split no quadrant is found.
	if(boxCenter.x - boxHalfSize.x >= pCenter.x){
		quadrant |= 2;
	}else if(boxCenter.x + boxHalfSize.x >= pCenter.x){
		return eqNotFound;
	}
	if(boxCenter.y - boxHalfSize.y >= pCenter.y){
		quadrant |= 1;
	}else if(boxCenter.y + boxHalfSize.y >= pCenter.y){
		return eqNotFound;
	}
	
	// return the found quadrant
	return quadrant;
}

bool decQuadtree::ContainsBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const{
	return (boxCenter - boxHalfSize) >= (pCenter - pHalfSize)
		&& (boxCenter + boxHalfSize) < (pCenter + pHalfSize);
}

decQuadtree *decQuadtree::FindNodeAtPoint(const decVector2 &point) const{
	int quadrant = FindQuadrantAtPoint(point);
	
	// if we found no matching quadrant return NULL
	if(quadrant == eqNotFound) return NULL;
	
	// return the node that we found
	return pNodes[quadrant];
}

int decQuadtree::FindQuadrantAtPoint(const decVector2 &point) const{
	int bitmask = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit.
	if(point.x >= pCenter.x) bitmask |= 2;
	if(point.y >= pCenter.y) bitmask |= 1;
	
	// return the found quadrant
	return bitmask;
}

bool decQuadtree::ContainsPoint(const decVector2 &point) const{
	return point >= (pCenter - pHalfSize) &&  point < (pCenter + pHalfSize);
}



decQuadtree *decQuadtree::SearchTreeForBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const{
	decQuadtree *nextNode = FindNodeAtBox(boxCenter, boxHalfSize);
	decQuadtree *curNode = (decQuadtree*)this;
	
	while(nextNode){
		curNode = nextNode;
		nextNode = curNode->FindNodeAtBox(boxCenter, boxHalfSize);
	}
	
	return curNode;
}

decQuadtree *decQuadtree::SearchTreeForPoint(const decVector2 &point) const{
	decQuadtree *nextNode = FindNodeAtPoint(point);
	decQuadtree *curNode = (decQuadtree*)this;
	
	while(nextNode){
		curNode = nextNode;
		nextNode = curNode->FindNodeAtPoint(point);
	}
	
	return curNode;
}

void decQuadtree::VisitNodes(decQuadtreeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	int i;
	
	// visit
	visitor->VisitNode(this);
	
	// visit each child node
	for(i=0; i<4; i++){
		if(pNodes[i]) pNodes[i]->VisitNodes(visitor);
	}
}

void decQuadtree::VisitNodesColliding(decQuadtreeVisitor *visitor, decCollisionVolume *volume){
	if(! visitor || ! volume) DETHROW(deeInvalidParam);
	decCollisionBox colBox(decVector(pCenter.x, 0.0, pCenter.y), decVector(pHalfSize.x, 250.0, pHalfSize.y));
	int i;
	
	// exit if this node is not in the collision volume
	if(! volume->BoxHitsVolume(&colBox)) return;
	
	// visit
	visitor->VisitNode(this);
	
	// test each child node
	for(i=0; i<4; i++){
		if(pNodes[i]) pNodes[i]->VisitNodesColliding(visitor, volume);
	}
}

void decQuadtree::ClearTree(bool clearNodes){
	int i;
	
	ClearNodeContent();
	
	for(i=0; i<4; i++){
		if(pNodes[i]){
			pNodes[i]->ClearTree(clearNodes);
			if(clearNodes){
				delete pNodes[i];
				pNodes[i] = NULL;
			}
		}
	}
}
