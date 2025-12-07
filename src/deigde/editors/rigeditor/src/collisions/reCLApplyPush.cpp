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

#include "reCLApplyPush.h"
#include "../rig/reRig.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>



// Class reCLApplyPush
////////////////////////

// Constructor, destructor
////////////////////////////

reCLApplyPush::reCLApplyPush(reRig *rig){
	if(!rig) DETHROW(deeInvalidParam);
	
	pRig = rig;
	
	pPush.Set(0.0f, 0.0f, 1.0f);
	pCollider = NULL;
}

reCLApplyPush::~reCLApplyPush(){
}



// Management
///////////////

void reCLApplyPush::SetRay(const decDVector &position, const decVector &direction){
	pRayPosition = position;
	pRayDirection = direction;
}

void reCLApplyPush::SetPush(const decVector &push){
	pPush = push;
}

void reCLApplyPush::SetCollider(deCollider *collider){
	pCollider = collider;
}



void reCLApplyPush::Reset(){
	pClosestCollider = NULL;
	pClosestBone = 0;
	pClosestDistance = 0.0f;
}

void reCLApplyPush::ApplyPush(){
	if(pClosestCollider){
		const decDVector hitPoint = pRayPosition + pRayDirection * (double)pClosestDistance;
		const decDMatrix matrix = decDMatrix::CreateWorld(pClosestCollider->GetPosition(),
			pClosestCollider->GetOrientation()).Invert();
		deColliderVisitorIdentify identify;
		
		pClosestCollider->Visit(identify);
		
		if(identify.IsVolume()){
			deColliderVolume &colliderVolume = identify.CastToVolume();
			colliderVolume.ApplyImpulsAt(pPush, (matrix * hitPoint).ToVector());
			
		}else if(identify.IsComponent()){
			deColliderComponent &colliderComponent = identify.CastToComponent();
			
			if(pClosestBone == -1){
				colliderComponent.ApplyImpulsAt(pPush, (matrix * hitPoint).ToVector());
				
			}else{
				const decDMatrix boneInvMatrix = colliderComponent.GetBoneAt(pClosestBone).GetInverseMatrix();
				colliderComponent.ApplyBoneImpulsAt(pClosestBone, pPush, (boneInvMatrix * hitPoint).ToVector());
			}
		}
	}
}



void reCLApplyPush::ApplyPushIn(deBasePhysicsWorld &phyWorld, const decLayerMask &layerMask){
	Reset();
	phyWorld.RayHits(pRayPosition, pRayDirection.ToVector(), this, layerMask);
	ApplyPush();
}



// Notifications
//////////////////

void reCLApplyPush::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(info->IsCollider()){
		if(!pClosestCollider || info->GetDistance() < pClosestDistance){
			pClosestCollider = info->GetCollider();
			pClosestBone = info->GetBone();
			pClosestDistance = info->GetDistance();
		}
	}
}

bool reCLApplyPush::CanHitCollider(deCollider *owner, deCollider *collider){
	return collider == pCollider;
}

void reCLApplyPush::ColliderChanged(deCollider *owner){
}
