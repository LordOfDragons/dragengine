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

#include "deoalMOVRayBlocked.h"
#include "deoalWOVRayBlocked.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVRayBlocked
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVRayBlocked::deoalWOVRayBlocked() :
pBlocked(false){
}

deoalWOVRayBlocked::~deoalWOVRayBlocked(){
}



// Visiting
/////////////

void deoalWOVRayBlocked::SetBlocked(bool blocked){
	pBlocked = blocked;
}



void deoalWOVRayBlocked::VisitNode(deoalWorldOctree &node){
	const decLayerMask &layerMask = GetLayerMask();
	const int count = node.GetComponentCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalAComponent &component = *node.GetComponentAt(i);
		// this is implicit. components are not added to the octree if not affecting sound
// 		if( ! component.GetAffectsSound() ){
// 			continue;
// 		}
		if(component.GetLayerMask().MatchesNot(layerMask)){
			continue;
		}
		
		if(pRayHitsBox(component.GetBoxCenter(), component.GetBoxHalfExtends())){
			VisitComponent(&component);
			if(pBlocked){
				return;
			}
		}
	}
	
	for(i=0; i<8; i++){
		deoalWorldOctree * const childNode = (deoalWorldOctree*)node.GetNodeAt(i);
		if(childNode && pRayHitsBox(childNode->GetCenter(), childNode->GetHalfSize())){
			VisitNode(*childNode);
			if(pBlocked){
				return;
			}
		}
	}
}

void deoalWOVRayBlocked::VisitNode(deoalDOctree*, int){
	DETHROW(deeInvalidAction);   // do not use this version
}

void deoalWOVRayBlocked::VisitComponent(deoalAComponent *component){
	deoalAModel &model = *component->GetModel();
	if(!model.GetRTBVH() && !component->GetBVH()){
		return;
	}
	
	const decDMatrix &invMatrix = component->GetInverseMatrix();
	const decVector rayOrigin(invMatrix * GetRayOrigin());
	const decVector rayDirection(invMatrix.TransformNormal(GetRayDirection()));
	
	deoalMOVRayBlocked visitor(*component);
	visitor.SetRay(rayOrigin, rayDirection);
	
	if(component->GetBVH()){
		visitor.VisitBVH(*component->GetBVH());
		
	}else{
		visitor.VisitBVH(*model.GetRTBVH());
	}
	
	pBlocked = visitor.GetBlocked();
}
