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
#include "deoalRTWOVRayBlocked.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTWOVRayBlocked
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTWOVRayBlocked::deoalRTWOVRayBlocked() :
pBlocked(false){
}

deoalRTWOVRayBlocked::~deoalRTWOVRayBlocked(){
}



// Visiting
/////////////

void deoalRTWOVRayBlocked::SetBlocked(bool blocked){
	pBlocked = blocked;
}

#ifdef RTWOVRAYBLOCKED_DO_TIMING
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
void deoalRTWOVRayBlocked::StartTiming(){
	timingAll = 0.0f;
	timingAllCount = 0;
	timingComponent = 0.0f;
	timingComponentCount = 0;
	timingComponentOctree = 0.0f;
	timingComponentOctreeCount = 0;
	timingFaces = 0.0f;
	timingFacesCount = 0;
	timerAll.Reset();
}

void deoalRTWOVRayBlocked::EndTiming(){
}
#endif



void deoalRTWOVRayBlocked::VisitBVH(const deoalRTWorldBVH &bvh){
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingAllCount++;
	timerAll.Reset();
	#endif
	
	if(bvh.GetVisitNodeCount() > 0){
		VisitNode(bvh, bvh.GetVisitNodes()[0]);
	}
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingAll += timerAll.GetElapsedTime();
	#endif
}

void deoalRTWOVRayBlocked::VisitNode(const deoalRTWorldBVH &bvh,
const deoalRTWorldBVH::sVisitNode &node){
	// visit components
	if(node.componentCount > 0){
		const deoalRTWorldBVH::sVisitComponent * const components =
			bvh.GetVisitComponents() + node.firstComponent;
		int i;
		
		for(i=0; i<node.componentCount; i++){
			const deoalRTWorldBVH::sVisitComponent &component = components[i];
			if(/*pRayHitsSphere(component.sphereCenter, component.sphereRadiusSquared)
			&& */pRayHitsBox(component.center, component.halfSize)){
				VisitComponent(component);
				if(pBlocked){
					return;
				}
			}
		}
		
	// visit child nodes if hit by ray
	}else{
		const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[node.node1];
		if(pRayHitsBox(child1.center, child1.halfSize)){
			VisitNode(bvh, child1);
			if(pBlocked){
				return;
			}
		}
		
		const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[node.node2];
		if(pRayHitsBox(child2.center, child2.halfSize)){
			VisitNode(bvh, child2);
			if(pBlocked){
				return;
			}
		}
	}
}

void deoalRTWOVRayBlocked::VisitComponent(const deoalRTWorldBVH::sVisitComponent &rtcomponent){
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingComponentCount++;
	timerComponent.Reset();
	#endif
	
	const decVector rayOrigin(rtcomponent.inverseMatrix * GetRayOrigin());
	const decVector rayDirection(rtcomponent.inverseMatrix.TransformNormal(GetRayDirection()));
	
	deoalAComponent &component = *rtcomponent.component;
	
	deoalMOVRayBlocked visitor(component);
	visitor.SetRay(rayOrigin, rayDirection);
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingComponent += timerComponent.GetElapsedTime();
	#endif
	
	if(component.GetBVH()){
		visitor.VisitBVH(*component.GetBVH());
		
	}else{
		visitor.VisitBVH(*component.GetModel()->GetRTBVH());
	}
	
	pBlocked = visitor.GetBlocked();
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	const float elapsed = timerComponent.GetElapsedTime();
	timingComponentOctree += elapsed;
	timingComponentOctreeCount++;
	#endif
}
