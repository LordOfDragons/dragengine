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

#include "deoalWorldOctree.h"
#include "deoalWOVPrepareRayTrace.h"
#include "deoalRTWorldBVH.h"
#include "../deoalAWorld.h"
#include "../../audiothread/deoalAudioThread.h"
#include "../../audiothread/deoalATLogger.h"
#include "../../component/deoalAComponent.h"
#include "../../model/deoalAModel.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



// #define DO_TIMING 1



// Class deoalWOVPrepareRayTrace
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVPrepareRayTrace::deoalWOVPrepareRayTrace() :
pRadius(0.0),
pRadiusSquared(0.0),
    pRTWorldBVH(NULL)
{
	SetVisitAll(false);
	SetVisitComponents(true);
}

deoalWOVPrepareRayTrace::~deoalWOVPrepareRayTrace(){
}



// Visiting
/////////////

void deoalWOVPrepareRayTrace::SetCenter(const decDVector &center){
	pCenter = center;
}

void deoalWOVPrepareRayTrace::SetRadius(double radius){
	pRadius = decMath::max(radius, 0.0);
	pRadiusSquared = pRadius * pRadius;
}

void deoalWOVPrepareRayTrace::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}

void deoalWOVPrepareRayTrace::SetRTWorldBVH(deoalRTWorldBVH *bvh){
	   pRTWorldBVH = bvh;
}



#include "../../audiothread/deoalATDebug.h"
#if 0
class DummyTest {
	deoalAComponent *component;
	bool found;
public:
	DummyTest(deoalAComponent *c) : component(c), found(false){}
	bool VisitBVH(const deoalRTWorldBVH &bvh){
		if(bvh.GetVisitNodeCount() > 0) VisitNode(bvh, bvh.GetVisitNodes()[0]);
		return found;
	}
	void VisitNode(const deoalRTWorldBVH &bvh, const deoalRTWorldBVH::sVisitNode &node){
		const deoalRTWorldBVH::sVisitComponent * const components = bvh.GetVisitComponents() + node.firstComponent;
		int i;
		for(i=0; i<node.componentCount; i++){
			if(components[i].component == component) found=true;
		}
		if(node.node1 != -1){
			VisitNode(bvh, bvh.GetVisitNodes()[node.node1]);
		}
		if(node.node2 != -1){
			VisitNode(bvh, bvh.GetVisitNodes()[node.node2]);
		}
	}
};
#endif

void deoalWOVPrepareRayTrace::Visit(deoalAWorld &world){
	#ifdef DO_TIMING
	decTimer timer;
	#endif
	
	pComponents.RemoveAll();
	
	const decDVector radius(pRadius, pRadius, pRadius);
	world.GetOctree()->VisitNodesColliding(this, pCenter - radius, pCenter + radius);
	
	const int count = pComponents.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoalAComponent*)pComponents.GetAt(i))->PrepareOctree();
	}
	
	#ifdef DO_TIMING
	if(world.GetAudioThread().GetDebug().GetLogCalcEnvProbe()){
		world.GetAudioThread().GetLogger().LogInfoFormat("WOVPrepareRT.Visit: %.2fms", timer.GetElapsedTime() * 1e3f);
	}
	#endif
	
	if(pRTWorldBVH){
		pPrepareRTWorldBVH(*pRTWorldBVH);
		#ifdef DO_TIMING
		if(world.GetAudioThread().GetDebug().GetLogCalcEnvProbe())
		world.GetAudioThread().GetLogger().LogInfoFormat("WOVPrepareRT.RTBVH: %.2fms (%d nodes, %d components)",
			timer.GetElapsedTime() * 1e3f, pRTWorldBVH->GetVisitNodeCount(), pRTWorldBVH->GetVisitComponentCount());
		#endif
	}
	
	// TEMPORARY VERIFY
#if 0
	if(world.GetAudioThread().GetDebug().GetLogCalcEnvProbe())
	if(pRTWorldBVH){
		const int count = pComponents.GetCount();
		if(pRTWorldBVH->GetVisitComponentCount() != count){
			world.GetAudioThread().GetLogger().LogInfoFormat("ERROR: Component Count Differs: should %d is %d",
				count, pRTWorldBVH->GetVisitComponentCount());
			return;
		}
		
		int i;
		for(i=0; i<count; i++){
			if(!DummyTest((deoalAComponent*)pComponents.GetAt(i)).VisitBVH(*pRTWorldBVH)){
				world.GetAudioThread().GetLogger().LogInfoFormat("ERROR: Component %d missing (count %d)", i, count);
				return;
			}
		}
		
		world.GetAudioThread().GetLogger().LogInfo("ALL OK");
	}
#endif
}



void deoalWOVPrepareRayTrace::VisitNode(deoalDOctree *node, int){
	const deoalWorldOctree &sonode = *((deoalWorldOctree*)node);
	const int count = sonode.GetComponentCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalAComponent &component = *sonode.GetComponentAt(i);
		if(component.GetLayerMask().MatchesNot(pLayerMask)){
			continue;
		}
		
		const decDVector closest(pCenter.Clamped(component.GetMinExtend(), component.GetMaxExtend()));
		const double distSquared = (closest - pCenter).LengthSquared();
		if(distSquared > pRadiusSquared){
			continue;
		}
		
		pComponents.Add(&component);
	}
}



void deoalWOVPrepareRayTrace::pPrepareRTWorldBVH(deoalRTWorldBVH &bvh){
	bvh.Build(pCenter);
	
	const int count = pComponents.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalAComponent * const component = (deoalAComponent*)pComponents.GetAt(i);
		if(!component->GetModel()){
			continue;
		}
		if(!component->GetBVH() && !component->GetModel()->GetRTBVH()){
			continue;
		}
		
		bvh.AddComponent(component);
	}
	
	bvh.Finish();
}
