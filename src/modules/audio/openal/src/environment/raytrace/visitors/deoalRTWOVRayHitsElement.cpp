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

#include "deoalMOVRayHitsFaces.h"
#include "deoalRTWOVRayHitsElement.h"
#include "../deoalRayTraceResult.h"
#include "../deoalRayTraceHitElement.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../world/octree/deoalRTWorldBVH.h"
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTWOVRayHitsElement
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTWOVRayHitsElement::deoalRTWOVRayHitsElement() :
pResult(NULL),
pThresholdTransmit(1e-3f)
{
	(void)pThresholdTransmit; // not used yet
}

deoalRTWOVRayHitsElement::~deoalRTWOVRayHitsElement(){
}



// Visiting
/////////////

void deoalRTWOVRayHitsElement::SetResult(deoalRayTraceResult *result){
	pResult = result;
}

#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
void deoalRTWOVRayHitsElement::StartTiming(){
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

void deoalRTWOVRayHitsElement::EndTiming(){
}

void deoalRTWOVRayHitsElement::VisitBVH(const deoalRTWorldBVH &bvh){
	timingAllCount++;
	timerAll.Reset();
	deoalRTWorldBVHVisitor::VisitBVH(bvh);
	timingAll += timerAll.GetElapsedTime();
}
#endif



void deoalRTWOVRayHitsElement::VisitComponent(const deoalRTWorldBVH::sVisitComponent &rtcomponent){
	// WARNING everything in here has to be thread-safe
	#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
	timingComponentCount++;
	timerComponent.Reset();
	#endif
	
	const deoalAComponent &component = *rtcomponent.component;
	const deoalAModel &model = *component.GetModel();
	
	const decVector rayOrigin(rtcomponent.inverseMatrix * GetRayOrigin());
	const decVector rayDirection(rtcomponent.inverseMatrix.TransformNormal(GetRayDirection()));
	
	deoalMOVRayHitsFaces visitor(*rtcomponent.component, model);
	visitor.SetRay(rayOrigin, rayDirection);
	visitor.SetResult(pResult);
	visitor.SetResultInWorldSpace(true);
	
	#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
	timingComponent += timerComponent.GetElapsedTime();
	#endif
	
	if(component.GetBVH()){
		visitor.VisitBVH(*component.GetBVH());
		
	}else{
		visitor.VisitBVH(*model.GetRTBVH());
	}
	
	#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
	const float elapsed = timerComponent.GetElapsedTime();
	timingComponentOctree += elapsed;
	timingComponentOctreeCount++;
	#ifdef MOVRAYHITSFACES_DO_TIMIING
	timingFaces += visitor.timing;
	timingFacesCount += visitor.timingCount;
	#endif
	#endif
}

void deoalRTWOVRayHitsElement::pVisitNode(const deoalRTWorldBVH &bvh, const deoalRTWorldBVH::sVisitNode &node){
	// visit components
	if(node.componentCount > 0){
		const deoalRTWorldBVH::sVisitComponent * const components =
			bvh.GetVisitComponents() + node.firstComponent;
		int i;
		
		for(i=0; i<node.componentCount; i++){
			const deoalRTWorldBVH::sVisitComponent &component = components[i];
			if(pRayHitsBox(component.center, component.halfSize)){
				VisitComponent(component);
			}
		}
		
	// visit child nodes if hit by ray
	}else{
		const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[node.node1];
		if(pRayHitsBox(child1.center, child1.halfSize)){
			pVisitNode(bvh, child1);
		}
		
		const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[node.node2];
		if(pRayHitsBox(child2.center, child2.halfSize)){
			pVisitNode(bvh, child2);
		}
	}
}
