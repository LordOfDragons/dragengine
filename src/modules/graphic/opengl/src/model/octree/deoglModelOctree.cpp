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

#include "deoglModelOctree.h"
#include "deoglModelOctreeVisitor.h"
#include "../face/deoglModelFace.h"
#include "../../utils/collision/deoglCollisionBox.h"
#include "../../utils/collision/deoglCollisionVolume.h"
#include "../../utils/collision/deoglCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglModelOctree::deoglModelOctree(const decVector &center, const decVector &halfExtend) : deoglOctree(center, halfExtend){
}

deoglModelOctree::~deoglModelOctree(){
}



// Management
///////////////

deoglOctree *deoglModelOctree::CreateOctree(int octant) const{
	const decVector halfExtend(GetHalfSize() * 0.5f);
	const decVector &center = GetCenter();
	deoglOctree *node = NULL;
	decVector nc;
	
	// determine the smallest and largest coordinates
	if((octant & 4) == 4){
		nc.x = center.x + halfExtend.x;
		
	}else{
		nc.x = center.x - halfExtend.x;
	}
	
	if((octant & 2) == 2){
		nc.y = center.y + halfExtend.y;
		
	}else{
		nc.y = center.y - halfExtend.y;
	}
	
	if((octant & 1) == 1){
		nc.z = center.z + halfExtend.z;
		
	}else{
		nc.z = center.z - halfExtend.z;
	}
	
	// create child node
	node = new deoglModelOctree(nc, halfExtend);
	
	return node;
}

void deoglModelOctree::ClearNodeContent(){
	ClearFaces();
}

void deoglModelOctree::ClearFaces(){
	deoglOctree *node;
	int i;
	
	pFaces.RemoveAll();
	
	for(i=0; i<8; i++){
		node = GetNodeAt(i);
		if(node){
			((deoglModelOctree*)node)->ClearFaces();
		}
	}
}

void deoglModelOctree::InsertFaceIntoTree(deoglModelFace *face, int maxDepth){
	if(!face || maxDepth < 0) DETHROW(deeInvalidParam);
	
	const decVector &minExtend = face->GetMinExtend();
	const decVector &maxExtend = face->GetMaxExtend();
	const decVector boxCenter = (minExtend + maxExtend) * 0.5f;
	const decVector boxHalfExtend = (maxExtend - minExtend) * 0.5f;
	
	pGetNodeFor(boxCenter, boxHalfExtend, maxDepth)->GetFaceList().Add(face);
}

void deoglModelOctree::VisitNodesCollidingVolume(deoglModelOctreeVisitor &visitor, deoglCollisionVolume &volume){
	deoglCollisionBox box(GetCenter(), GetHalfSize());
	const deoglOctree *node;
	int i;
	
	if(volume.BoxHitsVolume(&box)){
		visitor.VisitNode(this, deoglCollisionDetection::eirPartial);
		
		for(i=0; i<8; i++){
			node = GetNodeAt(i);
			
			if(node){
				((deoglModelOctree*)node)->VisitNodesCollidingVolume(visitor, volume);
			}
		}
	}
}



// Private Functions
//////////////////////

deoglModelOctree *deoglModelOctree::pGetNodeFor(const decVector &center, const decVector &halfExtend, int maxDepth){
	deoglOctree *curNode = this;
	deoglOctree *nextNode;
	int d;
	
	for(d=0; d<maxDepth; d++){
		nextNode = curNode->GetNodeAtBox(center, halfExtend);
		if(!nextNode) break;
		curNode = nextNode;
	}
	
	return (deoglModelOctree*)curNode;
}

deoglModelOctree *deoglModelOctree::pGetNodeFor(const decVector &position, int maxDepth){
	deoglOctree *curNode = this;
	deoglOctree *nextNode;
	int d, octant;
	
	for(d=0; d<maxDepth; d++){
		octant = curNode->FindOctantAtPoint(position);
		if(octant == deoglOctree::eoNotFound) break;
		
		nextNode = curNode->GetNodeAt(octant);
		if(!nextNode){
			nextNode = curNode->CreateOctree(octant);
			curNode->SetNodeAt(octant, nextNode);
		}
		
		curNode = nextNode;
	}
	
	return (deoglModelOctree*)curNode;
}
