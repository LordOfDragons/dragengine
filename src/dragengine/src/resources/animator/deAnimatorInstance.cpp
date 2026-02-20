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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deAnimator.h"
#include "deAnimatorInstance.h"
#include "deAnimatorInstanceManager.h"
#include "controller/deAnimatorController.h"
#include "../animation/deAnimation.h"
#include "../component/deComponent.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/collection/decTList.h"
#include "../../systems/modules/animator/deBaseAnimatorAnimatorInstance.h"



// Class deAnimatorInstance
/////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorInstance::deAnimatorInstance(deAnimatorInstanceManager *manager) :
deResource(manager),

pBlendMode(deAnimatorRule::ebmBlend),
pBlendFactor(1.0f),
pEnableRetargeting(true),
pProtectDynamicBones(false),

pPeerAnimator(nullptr){

}

deAnimatorInstance::~deAnimatorInstance(){
	pCleanUp();
}



// Management
///////////////

void deAnimatorInstance::SetAnimator(deAnimator *animator, bool keepValues){
	if(pAnimator == animator){
		return;
	}
	
	const int oldCount = pControllers.GetCount();
	if(keepValues && pAnimator && animator && oldCount > 0 && animator->GetControllers().GetCount() > 0){
		// NOTE the guard is required to keep the controller name alive during transfering.
		//      we can not use pControllers names since they are destroyed during
		//      pUpdateControllers(). instead controller names from guarded animator is used
		const deAnimator::Ref guard(pAnimator);
		struct sControllerValue{
			const char *name;
			float value;
			decVector vector;
		};
		decTList<sControllerValue> transfer(oldCount);
		int i;
		
		for(i=0; i<oldCount; i++){
			transfer.Add({
				pAnimator->GetControllers().GetAt(i)->GetName(),
				pControllers.GetAt(i)->GetCurrentValue(),
				pControllers.GetAt(i)->GetVector()});
		}
		
		pAnimator = animator;
		pUpdateControllers();
		
		for(i=0; i<oldCount; i++){
			const int index = IndexOfControllerNamed(transfer.GetAt(i).name);
			if(index == -1){
				continue;
			}
			
			pControllers.GetAt(index)->SetCurrentValue(transfer.GetAt(i).value);
			pControllers.GetAt(index)->SetVector(transfer.GetAt(i).vector);
		}
		
	}else{
		pAnimator = animator;
		pUpdateControllers();
	}
	
	if(pPeerAnimator){
		pPeerAnimator->AnimatorChanged();
	}
}

void deAnimatorInstance::SetComponent(deComponent *component){
	if(pComponent == component){
		return;
	}
	
	pComponent = component;
	
	if(pPeerAnimator){
		pPeerAnimator->ComponentChanged();
	}
}

void deAnimatorInstance::SetAnimation(deAnimation *animation){
	if(pAnimation == animation){
		return;
	}
	
	pAnimation = animation;
	
	if(pPeerAnimator){
		pPeerAnimator->AnimationChanged();
	}
}



void deAnimatorInstance::SetBlendMode(deAnimatorRule::eBlendModes mode){
	if(mode < deAnimatorRule::ebmBlend || mode > deAnimatorRule::ebmBlend){
		DETHROW(deeInvalidParam);
	}
	
	if(mode == pBlendMode){
		return;
	}
	
	pBlendMode = mode;
	
	if(pPeerAnimator){
		pPeerAnimator->BlendFactorChanged();
	}
}

void deAnimatorInstance::SetBlendFactor(float factor){
	factor = decMath::clamp(factor, 0.0f, 1.0f);
	
	if(fabsf(factor - pBlendFactor) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBlendFactor = factor;
	
	if(pPeerAnimator){
		pPeerAnimator->BlendFactorChanged();
	}
}

void deAnimatorInstance::SetEnableRetargeting(bool enableRetargeting){
	if(enableRetargeting == pEnableRetargeting){
		return;
	}
	
	pEnableRetargeting = enableRetargeting;
	
	if(pPeerAnimator){
		pPeerAnimator->EnableRetargetingChanged();
	}
}

void deAnimatorInstance::SetProtectDynamicBones(bool protectDynamicBones){
	if(protectDynamicBones == pProtectDynamicBones){
		return;
	}
	
	pProtectDynamicBones = protectDynamicBones;
	
	if(pPeerAnimator){
		pPeerAnimator->ProtectDynamicBonesChanged();
	}
}

int deAnimatorInstance::IndexOfControllerNamed(const char *name) const{
	const int count = pControllers.GetCount();
	for(int i=0; i<count; i++){
		if(pControllers.GetAt(i)->GetName() == name){
			return i;
		}
	}
	return -1;
}

void deAnimatorInstance::NotifyControllerChangedAt(int index){
	if(index < 0 || index >= pControllers.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerAnimator){
		pPeerAnimator->ControllerChanged(index);
	}
}



void deAnimatorInstance::Apply(bool direct){
	if(pPeerAnimator){
		pPeerAnimator->Apply(direct);
	}
}

void deAnimatorInstance::CaptureStateInto(int identifier){
	if(pPeerAnimator){
		pPeerAnimator->CaptureStateInto(identifier);
	}
}

void deAnimatorInstance::StoreFrameInto(int identifier, const char *moveName, float moveTime){
	if(!moveName){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerAnimator){
		pPeerAnimator->StoreFrameInto(identifier, moveName, moveTime);
	}
}



// System Peers
/////////////////

void deAnimatorInstance::SetPeerAnimator(deBaseAnimatorAnimatorInstance *peer){
	if(peer == pPeerAnimator){
		return;
	}
	
	if(pPeerAnimator){
		delete pPeerAnimator;
	}
	
	pPeerAnimator = peer;
}



// Private function
/////////////////////

void deAnimatorInstance::pCleanUp(){
	if(pPeerAnimator){
		delete pPeerAnimator;
		pPeerAnimator = nullptr;
	}
}

void deAnimatorInstance::pUpdateControllers(){
	pControllers.RemoveAll();
	
	if(!pAnimator){
		return;
	}
	
	pAnimator->GetControllers().Visit([&](const deAnimatorController &c){
		pControllers.Add(deAnimatorController::Ref::New(c));
	});
}
