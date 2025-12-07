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

#include "deoalRayCacheOctree.h"
#include "deoalRayCacheRay.h"
#include "../collision/deoalCollisionBox.h"
#include "../collision/deoalCollisionVolume.h"
#include "../collision/deoalCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayCacheOctree
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayCacheOctree::deoalRayCacheOctree(const decVector &center, const decVector &halfSize) :
deoalOctree(center, halfSize){
}

deoalRayCacheOctree::~deoalRayCacheOctree(){
	RemoveAllRays();
}



// Management
///////////////

deoalOctree *deoalRayCacheOctree::CreateOctree(int octant) const{
	const decVector halfSize(GetHalfSize() * 0.5f);
	const decVector &center = GetCenter();
	decVector nc;
	
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
	
	return new deoalRayCacheOctree(nc, halfSize);
}

void deoalRayCacheOctree::ClearNodeContent(){
	RemoveAllRays();
}

void deoalRayCacheOctree::ClearRays(){
	RemoveAllRays();
	
	int i;
	for(i=0; i<8; i++){
		deoalOctree * const node = GetNodeAt(i);
		if(node){
			((deoalRayCacheOctree*)node)->ClearRays();
		}
	}
}

void deoalRayCacheOctree::InsertRayIntoTree(deoalRayCacheRay *ray, int maxDepth){
	if(! ray || maxDepth < 0){
		DETHROW(deeInvalidParam);
	}
	
	pGetNodeFor(ray->GetOrigin(), maxDepth)->AddRay(ray);
}



// Rays
/////////

int deoalRayCacheOctree::GetRayCount() const{
	return pRays.GetCount();
}

deoalRayCacheRay *deoalRayCacheOctree::GetRayAt(int index) const{
	return (deoalRayCacheRay*)pRays.GetAt(index);
}

int deoalRayCacheOctree::IndexOfRay(deoalRayCacheRay *ray) const{
	return pRays.IndexOf(ray);
}

void deoalRayCacheOctree::AddRay(deoalRayCacheRay *ray){
	if(! ray || ray->GetOctreeNode()){
		DETHROW(deeInvalidParam);
	}
	
	pRays.Add(ray);
	ray->SetOctreeNode(this);
}

void deoalRayCacheOctree::RemoveRay(deoalRayCacheRay *ray){
	if(! ray || ray->GetOctreeNode() != this){
		DETHROW(deeInvalidParam);
	}
	
	pRays.RemoveFrom(pRays.IndexOf(ray));
	ray->SetOctreeNode(NULL);
}

void deoalRayCacheOctree::RemoveAllRays(){
	const int count = pRays.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoalRayCacheRay*)pRays.GetAt(i))->SetOctreeNode(NULL);
	}
	pRays.RemoveAll();
}



	
// Private Functions
//////////////////////

deoalRayCacheOctree *deoalRayCacheOctree::pGetNodeFor(const decVector &position, int maxDepth){
	deoalOctree *curNode = this;
	int i;
	
	for(i=0; i<maxDepth; i++){
		const int octant = curNode->FindOctantAtPoint(position);
		if(octant == deoalOctree::eoNotFound){
			break;
		}
		
		deoalOctree *nextNode = curNode->GetNodeAt(octant);
		if(! nextNode){
			nextNode = curNode->CreateOctree(octant);
			curNode->SetNodeAt(octant, nextNode);
		}
		
		curNode = nextNode;
	}
	
	return (deoalRayCacheOctree*)curNode;
}
