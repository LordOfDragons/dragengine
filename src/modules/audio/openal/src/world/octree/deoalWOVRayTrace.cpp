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

#include "deoalWOVRayTrace.h"
#include "deoalWorldOctree.h"
#include "../../component/deoalAComponent.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVRayTrace
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVRayTrace::deoalWOVRayTrace() :
pCheckAxisX(false),
pCheckAxisY(false),
pCheckAxisZ(false),
pRayLength(0.0)
{
	SetVisitAll(false);
	SetVisitComponents(true);
}

deoalWOVRayTrace::~deoalWOVRayTrace(){
}



// Visiting
/////////////

void deoalWOVRayTrace::SetRay(const decDVector &origin, const decDVector &direction){
	pRayOrigin = origin;
	pRayTarget = origin + direction;
	pRayDirection = direction;
	pRayLength = direction.Length();
	
	pInvRayDirection.SetZero();
	pCheckAxisX = false;
	pCheckAxisY = false;
	pCheckAxisZ = false;
	
	if(fabs(direction.x) > DOUBLE_SAFE_EPSILON){
		pInvRayDirection.x = 1.0 / direction.x;
		pCheckAxisX = true;
	}
	if(fabs(direction.y) > DOUBLE_SAFE_EPSILON){
		pInvRayDirection.y = 1.0 / direction.y;
		pCheckAxisY = true;
	}
	if(fabs(direction.z) > DOUBLE_SAFE_EPSILON){
		pInvRayDirection.z = 1.0 / direction.z;
		pCheckAxisZ = true;
	}
	
	const decDVector margin(0.0005, 0.0005, 0.0005);
	pRayBoxMin = pRayOrigin.Smallest(pRayTarget) - margin;
	pRayBoxMax = pRayOrigin.Largest(pRayTarget) + margin;
}

void deoalWOVRayTrace::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}



void deoalWOVRayTrace::VisitNode(deoalWorldOctree &node){
	// visit components in the node
	const int count = node.GetComponentCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalAComponent &component = *node.GetComponentAt(i);
		// this is implicit. components are not added to the octree if not affecting sound
// 		if( ! component.GetAffectsSound() ){
// 			continue;
// 		}
		if(component.GetLayerMask().MatchesNot(pLayerMask)){
			continue;
		}
// 		if( component.GetMaxExtend() < pRayBoxMin || component.GetMinExtend() > pRayBoxMax ){
// 			continue;
// 		}
		
		if(pRayHitsBox(component.GetBoxCenter(), component.GetBoxHalfExtends())){
			VisitComponent(&component);
		}
	}
	
	// visit child nodes if hit by ray
	for(i=0; i<8; i++){
		deoalDOctree * const childNode = node.GetNodeAt(i);
		if(childNode && pRayHitsBox(childNode->GetCenter(), childNode->GetHalfSize())){
			VisitNode(*((deoalWorldOctree*)childNode));
		}
	}
}

void deoalWOVRayTrace::VisitNode(deoalDOctree *node, int /*intersection*/){
	const deoalWorldOctree &sonode = *((deoalWorldOctree*)node);
	const int count = sonode.GetComponentCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalAComponent &component = *sonode.GetComponentAt(i);
		if(! component.GetAffectsSound()){
			continue;
		}
		if(component.GetLayerMask().MatchesNot(pLayerMask)){
			continue;
		}
		if(component.GetMaxExtend() < pRayBoxMin || component.GetMinExtend() > pRayBoxMax){
			continue;
		}
		
		if(pRayHitsBox(component.GetBoxCenter(), component.GetBoxHalfExtends())){
			VisitComponent(&component);
		}
	}
}



// Protected Functions
////////////////////////

bool deoalWOVRayTrace::pRayHitsBox(const decDVector &center, const decDVector &halfExtends){
	const decDVector point(pRayOrigin - center);
	if(point.Absolute() <= halfExtends || (pRayTarget - center).Absolute() <= halfExtends){
		return true;
	}
	
	// x axis
	if(pCheckAxisX){
		// face on the positive side
		const double lambda1 = pInvRayDirection.x * (halfExtends.x - point.x);
		if(lambda1 >= 0.0 && lambda1 <= 1.0){
			const double y = point.y + pRayDirection.y * lambda1;
			if(y >= -halfExtends.y && y <= halfExtends.y){
				const double z = point.z + pRayDirection.z * lambda1;
				if(z >= -halfExtends.z && z <= halfExtends.z){
					return true;
				}
			}
		}
		
		// face on the negative side
		const double lambda2 = pInvRayDirection.x * (-halfExtends.x - point.x);
		if(lambda2 >= 0.0 && lambda2 <= 1.0){
			const double y = point.y + pRayDirection.y * lambda2;
			if(y >= -halfExtends.y && y <= halfExtends.y){
				const double z = point.z + pRayDirection.z * lambda2;
				if(z >= -halfExtends.z && z <= halfExtends.z){
					return true;
				}
			}
		}
	}
	
	// y axis
	if(pCheckAxisY){
		// face on the positive side
		const double lambda1 = pInvRayDirection.y * (halfExtends.y - point.y);
		if(lambda1 >= 0.0 && lambda1 <= 1.0){
			const double x = point.x + pRayDirection.x * lambda1;
			if(x >= -halfExtends.x && x <= halfExtends.x){
				const double z = point.z + pRayDirection.z * lambda1;
				if(z >= -halfExtends.z && z <= halfExtends.z){
					return true;
				}
			}
		}
		
		// face on the negative side
		const double lambda2 = pInvRayDirection.y * (-halfExtends.y - point.y);
		if(lambda2 >= 0.0 && lambda2 <= 1.0){
			const double x = point.x + pRayDirection.x * lambda2;
			if(x >= -halfExtends.x && x <= halfExtends.x){
				const double z = point.z + pRayDirection.z * lambda2;
				if(z >= -halfExtends.z && z <= halfExtends.z){
					return true;
				}
			}
		}
	}
	
	// z axis
	if(pCheckAxisZ){
		// face on the positive side
		const double lambda1 = pInvRayDirection.z * (halfExtends.z - point.z);
		if(lambda1 >= 0.0 && lambda1 <= 1.0){
			const double x = point.x + pRayDirection.x * lambda1;
			if(x >= -halfExtends.x && x <= halfExtends.x){
				const double y = point.y + pRayDirection.y * lambda1;
				if(y >= -halfExtends.y && y <= halfExtends.y){
					return true;
				}
			}
		}
		
		// face on the negative side
		const double lambda2 = pInvRayDirection.z * (-halfExtends.z - point.z);
		if(lambda2 >= 0.0 && lambda2 <= 1.0){
			const double x = point.x + pRayDirection.x * lambda2;
			if(x >= -halfExtends.x && x <= halfExtends.x){
				const double y = point.y + pRayDirection.y * lambda2;
				if(y >= -halfExtends.y && y <= halfExtends.y){
					return true;
				}
			}
		}
	}
	
	// no hit
	return false;
}
