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

#include "reCLRigSimulation.h"
#include "../rig/reRig.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Rig Editor"




// Class reCLRigSimulation
////////////////////////////

// Constructor, destructor
////////////////////////////

reCLRigSimulation::reCLRigSimulation(reRig &rig) :
pRig(rig){
}

reCLRigSimulation::~reCLRigSimulation(){
}



// Notifications
//////////////////

void reCLRigSimulation::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(owner != pRig.GetEngineSimulationCollider()){
		return;
	}
	
	deColliderComponent &collider = *pRig.GetEngineSimulationCollider();
	
	switch(collider.GetResponseType()){
	case deCollider::ertDynamic:{
		const int ownerBoneIndex = info->GetOwnerBone();
		
		if(ownerBoneIndex == -1){
			/*
			const decVector &linvelo = collider.GetLinearVelocity();
			const float mass = collider.GetMass();
			const float impulse = linvelo.Length() * mass;
			pRig.GetLogger()->LogInfoFormat(LOGSOURCE, "CollisionResponse rig: velocity=%f mass=%f impulse=%f",
				linvelo.Length(), mass, impulse);
			*/
			
		}else{
			deColliderBone &ownerBone = collider.GetBoneAt(ownerBoneIndex);
			const decVector &linvelo = ownerBone.GetLinearVelocity();
			
			if(ownerBone.GetDynamic()){
				/*
				const float mass = ownerBone.GetMass();
				const float impulse = linvelo.Length() * mass;
				pRig.GetLogger()->LogInfoFormat(LOGSOURCE, "CollisionResponse bone(%i): velocity=%f mass=%f impulse=%f",
					ownerBoneIndex, linvelo.Length(), mass, impulse);
				*/
				
			}else{
				const decVector &normal = info->GetNormal();
				if(linvelo * normal < 0.0f){
					ownerBone.SetPosition(ownerBone.GetPosition() + decDVector(normal) * 0.001);
					ownerBone.SetLinearVelocity(linvelo - normal * (normal * linvelo));
				}
			}
		}
		
		}break;
		
	case deCollider::ertKinematic:{
		const decVector &linvelo = collider.GetLinearVelocity();
		const decVector &normal = info->GetNormal();
		if(linvelo * normal < 0.0f){
			collider.SetPosition(collider.GetPosition() + decDVector(normal) * 0.001);
			collider.SetLinearVelocity(linvelo - normal * (normal * linvelo));
		}
		}break;
		
	case deCollider::ertStatic:
		break;
	}
}

bool reCLRigSimulation::CanHitCollider(deCollider *owner, deCollider *collider){
	// currently we allow no collisions at all. once we have pushers or other
	// objects this can change.
	return false;
	
	/*
	deColliderVisitorIdentify identify;
	
	collider->Visit(identify);
	if(identify.IsVolume()){
		deColliderVolume *colliderVolume = identify.CastToVolume();
		
		if(pCanSelectBones && pGetBoneFromCollider(colliderVolume)){
			return true;
		}
		
		if(pCanSelectShapes && pGetShapeFromCollider(colliderVolume)){
			return true;
		}
	}
	
	return false;
	*/
}

void reCLRigSimulation::ColliderChanged(deCollider *owner){
	pRig.UpdateFromSimulation();
}
