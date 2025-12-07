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

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deColliderRig.h"
#include "deColliderBone.h"
#include "deColliderVisitor.h"
#include "deColliderComponent.h"
#include "../component/deComponent.h"
#include "../rig/deRig.h"
#include "../rig/deRigBone.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/physics/deBasePhysicsCollider.h"



// class deColliderRig
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderRig::deColliderRig(deColliderManager *manager) :
deCollider(manager),
pBones(NULL),
pBoneCount(0){
}

deColliderRig::~deColliderRig(){
	if(pBones){
		int i;
		
		for(i=0; i<pBoneCount; i++){
			if(pBones[i]){
				delete pBones[i];
			}
		}
		
		delete [] pBones;
	}
}



// Management
///////////////

void deColliderRig::SetRig(deRig *rig){
	if(rig == pRig){
		return;
	}
	
	pRig = rig;
	
	// determine the bone count of the component if existing
	int boneCount = 0;
	if(rig){
		boneCount = rig->GetBoneCount();
	}
	
	// if the count changed resize the bones array
	if(boneCount != pBoneCount){
		deColliderBone **bones = NULL;
		int i;
		
		if(boneCount > 0){
			try{
				bones = new deColliderBone*[boneCount];
				for(i=0; i<boneCount; i++){
					bones[i] = NULL;
				}
				
				for(i=0; i<boneCount; i++){
					bones[i] = new deColliderBone(this, i);
				}
				
			}catch(const deException &){
				if(bones){
					for(i=0; i<boneCount; i++){
						if(bones[i]){
							delete bones[i];
						}
					}
					delete [] bones;
				}
				throw;
			}
		}
		
		if(pBones){
			delete [] pBones;
			pBones = NULL;
		}
		
		pBones = bones;
		pBoneCount = boneCount;
	}
	
	// notify peers about rig change
	pNotifyRigChanged();
}

deColliderBone &deColliderRig::GetBoneAt(int index) const{
	if(index < 0 || index >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	return *pBones[index];
}



void deColliderRig::CopyStatesFromColliderRig(const deColliderRig &collider){
	if(! pRig){
		return;
	}
	
	const deRig * const rig = collider.GetRig();
	if(! rig){
		return;
	}
	
	int i;
	for(i=0; i<pBoneCount; i++){
		const int bone2 = rig->IndexOfBoneNamed(pRig->GetBoneAt(i).GetName());
		if(bone2 == -1){
			continue;
		}
		
		const deColliderBone &colbone2 = collider.GetBoneAt(bone2);
		const decVector &cmpTo = pRig->GetBoneAt(i).GetCentralMassPoint();
		const decVector &cmpFrom = rig->GetBoneAt(bone2).GetCentralMassPoint();
		
		pBones[i]->SetPosition(colbone2.GetMatrix() * (cmpTo - cmpFrom));
		pBones[i]->SetOrientation(colbone2.GetOrientation());
		pBones[i]->SetLinearVelocity(colbone2.GetLinearVelocity());
		pBones[i]->SetAngularVelocity(colbone2.GetAngularVelocity());
		pBones[i]->UpdateMatrix();
	}
}

void deColliderRig::CopyStateFromColliderRig(int bone, const deColliderRig &collider){
	if(! pRig || bone < 0 || bone >= pBoneCount){
		return;
	}
	
	const deRig * const rig = collider.GetRig();
	if(! rig){
		return;
	}
	
	const int bone2 = rig->IndexOfBoneNamed(pRig->GetBoneAt(bone).GetName());
	if(bone2 == -1){
		return;
	}
	
	const deColliderBone &colbone2 = collider.GetBoneAt(bone2);
	const decVector &cmpTo = pRig->GetBoneAt(bone).GetCentralMassPoint();
	const decVector &cmpFrom = rig->GetBoneAt(bone2).GetCentralMassPoint();
	
	pBones[bone]->SetPosition(colbone2.GetMatrix() * (cmpTo - cmpFrom));
	pBones[bone]->SetOrientation(colbone2.GetOrientation());
	pBones[bone]->SetLinearVelocity(colbone2.GetLinearVelocity());
	pBones[bone]->SetAngularVelocity(colbone2.GetAngularVelocity());
	pBones[bone]->UpdateMatrix();
}

void deColliderRig::CopyStateFromColliderRig(int boneFrom, int boneTo, const deColliderRig &collider){
	if(! pRig || boneTo < 0 || boneTo >= pBoneCount){
		return;
	}
	
	const deRig * const rig = collider.GetRig();
	if(! rig || boneFrom < 0 || boneFrom >= rig->GetBoneCount()){
		return;
	}
	
	const deColliderBone &colbone2 = collider.GetBoneAt(boneFrom);
	const decVector &cmpTo = pRig->GetBoneAt(boneTo).GetCentralMassPoint();
	const decVector &cmpFrom = rig->GetBoneAt(boneFrom).GetCentralMassPoint();
	
	pBones[boneTo]->SetPosition(colbone2.GetMatrix() * (cmpTo - cmpFrom));
	pBones[boneTo]->SetOrientation(colbone2.GetOrientation());
	pBones[boneTo]->SetLinearVelocity(colbone2.GetLinearVelocity());
	pBones[boneTo]->SetAngularVelocity(colbone2.GetAngularVelocity());
	pBones[boneTo]->UpdateMatrix();
}



void deColliderRig::EnableBoneConstraint(int bone, int constraint, bool enable){
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->EnableBoneConstraint(bone, constraint, enable);
	}
}

void deColliderRig::ReplaceBoneConstraint(int bone, int constraint, const deRigConstraint &replacement) {
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->ReplaceBoneConstraint(bone, constraint, replacement);
	}
}



// Forces
///////////

void deColliderRig::ApplyBoneImpuls(int bone, const decVector &impuls){
	if(bone < 0 || bone >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->ApplyBoneImpuls(bone, impuls);
	}
}

void deColliderRig::ApplyBoneImpulsAt(int bone, const decVector &impuls, const decVector &point){
	if(bone < 0 || bone >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->ApplyBoneImpulsAt(bone, impuls, point);
	}
}

void deColliderRig::ApplyBoneTorqueImpuls(int bone, const decVector &torqueImpuls){
	if(bone < 0 || bone >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->ApplyBoneTorqueImpuls(bone, torqueImpuls);
	}
}

void deColliderRig::ApplyBoneForce(int bone, const decVector &force){
	if(bone < 0 || bone >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->ApplyBoneForce(bone, force);
	}
}

void deColliderRig::ApplyBoneForceAt(int bone, const decVector &force, const decVector &point){
	if(bone < 0 || bone >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->ApplyBoneForceAt(bone, force, point);
	}
}

void deColliderRig::ApplyBoneTorque(int bone, const decVector &torque){
	if(bone < 0 || bone >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	
	deBasePhysicsCollider * const phyCollider = GetPeerPhysics();
	if(phyCollider){
		phyCollider->ApplyBoneTorque(bone, torque);
	}
}



// Visiting
/////////////

void deColliderRig::Visit(deColliderVisitor &visitor){
	visitor.VisitRig(*this);
}
