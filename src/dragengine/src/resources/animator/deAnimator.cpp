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

#include "deAnimator.h"
#include "deAnimatorManager.h"
#include "deAnimatorLink.h"
#include "controller/deAnimatorController.h"
#include "rule/deAnimatorRule.h"
#include "../animation/deAnimation.h"
#include "../rig/deRig.h"
#include "../../systems/modules/animator/deBaseAnimatorAnimator.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deAnimator
/////////////////////

// Constructor, destructor
////////////////////////////

deAnimator::deAnimator(deAnimatorManager *manager) :
deResource(manager),
pPeerAnimator(NULL){
}

deAnimator::~deAnimator(){
	pCleanUp();
}



// Management
///////////////

void deAnimator::SetRig(deRig *rig){
	if(rig == pRig){
		return;
	}
	
	pRig = rig;
	
	if(pPeerAnimator){
		pPeerAnimator->RigChanged();
	}
}

void deAnimator::SetAnimation(deAnimation *animation){
	if(pAnimation == animation){
		return;
	}
	
	pAnimation = animation;
	
	if(pPeerAnimator){
		pPeerAnimator->AnimationChanged();
	}
}

void deAnimator::NotifyBonesChanged(){
	if(pPeerAnimator){
		pPeerAnimator->BonesChanged();
	}
}

void deAnimator::NotifyVertexPositionSetsChanged(){
	if(pPeerAnimator){
		pPeerAnimator->VertexPositionSetsChanged();
	}
}



// Controller Management
//////////////////////////

int deAnimator::IndexOfControllerNamed(const char *name) const{
	const int count = pControllers.GetCount();
	for(int i=0; i<count; i++){
		if(pControllers.GetAt(i)->GetName() == name){
			return i;
		}
	}
	return -1;
}

void deAnimator::AddController(deAnimatorController *controller){
	if(!controller) DETHROW(deeInvalidParam);
	pControllers.Add(controller);
	if(pPeerAnimator){
		pPeerAnimator->ControllerCountChanged();
	}
}

void deAnimator::RemoveController(deAnimatorController *controller){
	pControllers.Remove(controller);
	if(pPeerAnimator){
		pPeerAnimator->ControllerCountChanged();
	}
}

void deAnimator::RemoveAllControllers(){
	pControllers.RemoveAll();
	if(pPeerAnimator){
		pPeerAnimator->ControllerCountChanged();
	}
}

void deAnimator::NotifyControllerChangedAt(int index){
	if(index < 0 || index >= pControllers.GetCount()) DETHROW(deeInvalidParam);
	if(pPeerAnimator){
		pPeerAnimator->ControllerChanged(index, pControllers.GetAt(index));
	}
}



// Link Management
////////////////////

void deAnimator::AddLink(deAnimatorLink *link){
	if(!link) DETHROW(deeInvalidParam);
	pLinks.Add(link);
	if(pPeerAnimator) pPeerAnimator->LinksChanged();
}

void deAnimator::RemoveLink(deAnimatorLink *link){
	pLinks.Remove(link);
	if(pPeerAnimator) pPeerAnimator->LinksChanged();
}

void deAnimator::RemoveAllLinks(){
	pLinks.RemoveAll();
	if(pPeerAnimator) pPeerAnimator->LinksChanged();
}

void deAnimator::NotifyLinkChangedAt(int index){
	if(index < 0 || index >= pLinks.GetCount()) DETHROW(deeInvalidParam);
	if(pPeerAnimator) pPeerAnimator->LinksChanged();
}



// Rule Management
////////////////////

void deAnimator::AddRule(deAnimatorRule *rule){
	if(!rule){
		DETHROW(deeInvalidParam);
	}
	
	pRules.Add(rule);
	
	if(pPeerAnimator){
		pPeerAnimator->RulesChanged();
	}
}

void deAnimator::RemoveRule(deAnimatorRule *rule){
	pRules.Remove(rule);
	
	if(pPeerAnimator){
		pPeerAnimator->RulesChanged();
	}
}

void deAnimator::RemoveAllRules(){
	pRules.RemoveAll();
	
	if(pPeerAnimator){
		pPeerAnimator->RulesChanged();
	}
}

void deAnimator::NotifyRulesChanged(){
	if(pPeerAnimator){
		pPeerAnimator->RulesChanged();
	}
}


// System Peers
/////////////////

void deAnimator::SetPeerAnimator(deBaseAnimatorAnimator *peer){
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

void deAnimator::pCleanUp(){
	if(pPeerAnimator){
		delete pPeerAnimator;
		pPeerAnimator = NULL;
	}
	
	RemoveAllRules();
	RemoveAllLinks();
	RemoveAllControllers();
}
