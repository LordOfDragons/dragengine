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

#include "aeAttachment.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"


// Class aeAttachment::cAsyncLoadListener
///////////////////////////////////////////

aeAttachment::cAsyncLoadListener::cAsyncLoadListener(aeAttachment &attachment) :
pAttachment(attachment){
}

void aeAttachment::cAsyncLoadListener::LoadFinished(igdeWObject &wrapper, bool succeeded){
	pAttachment.ReattachCollider();
}


// Class aeAttachment
///////////////////////

// Constructor, destructor
////////////////////////////

aeAttachment::aeAttachment(aeWindowMain &windowMain, const char *aname) :
pMetaContext(aeMCAttachment::Ref::New(windowMain, this)),
pAnimator(nullptr),
pAsyncLoadListener(*this),
name(windowMain.GetMCAnimatorProperties().attachment.name, pMetaContext, aname),
attachType(windowMain.GetMCAnimatorProperties().attachment.attachType, pMetaContext),
boneName(windowMain.GetMCAnimatorProperties().attachment.boneName, pMetaContext),
wobject(windowMain.GetMCAnimatorProperties().attachment.wobject, pMetaContext)
{
	try{
		decLayerMask layerMask;
		layerMask.SetBit(aeAnimator::eclElements);
		
		pObjectWrapper = igdeWObject::Ref::New(windowMain.GetEnvironment());
		pObjectWrapper->SetVisible(true);
		pObjectWrapper->SetDynamicCollider(false);
		pObjectWrapper->SetCollisionFilter(decCollisionFilter(layerMask));
		pObjectWrapper->SetCollisionFilterFallback(decCollisionFilter(layerMask));
		pObjectWrapper->SetAsyncLoadFinished(&pAsyncLoadListener);
		wobject.SetValue(pObjectWrapper->GetMetaContext(), false);
		
		pObjectWrapper->onChanged = [this](){
			ReattachCollider();
			if(pAnimator){
				pAnimator->NotifyAttachmentChanged(this);
			}
		};
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	name.onValueChanged = [this](){
		if(pAnimator){
			pAnimator->NotifyAttachmentChanged(this);
		}
	};
	
	attachType.onValueChanged = [this](){
		ReattachCollider();
		if(pAnimator){
			pAnimator->NotifyAttachmentChanged(this);
		}
	};
	boneName.onValueChanged = attachType.onValueChanged;
}

aeAttachment::aeAttachment(aeWindowMain &windowMain, const aeAttachment &copy) :
aeAttachment(windowMain, copy.name){
	attachType.SetValue(copy.attachType, false);
	boneName.SetValue(copy.boneName, false);
}

aeAttachment::~aeAttachment(){
	pCleanUp();
}



// Management
///////////////

void aeAttachment::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	pAnimator = animator;
	
	if(animator){
		pObjectWrapper->SetWorld(animator->GetEngineWorld());
		AttachCollider();
		
	}else{
		DetachCollider();
		pObjectWrapper->SetWorld(nullptr);
	}
}



void aeAttachment::SetName(const char *aname){
	name = aname;
}

void aeAttachment::SetAttachType(eAttachTypes type){
	attachType = type;
}

void aeAttachment::SetBoneName(const char *aname){
	boneName = aname;
}

void aeAttachment::ReattachCollider(){
	DetachCollider();
	AttachCollider();
	
	if(pAnimator){
		pAnimator->AttachmentsForceUpdate();
	}
	ResetPhysics();
}


void aeAttachment::Update(float elapsed){
	pObjectWrapper->Update(elapsed);
}

void aeAttachment::ResetPhysics(){
	pObjectWrapper->ResetPhysics();
}

void aeAttachment::AttachCollider(){
	if(!pAnimator){
		return;
	}
	
	try{
		switch(attachType){
		case eatBone:
			if(!boneName->IsEmpty()){
				pObjectWrapper->AttachColliderBone(pAnimator->GetEngineCollider(), boneName,
					pObjectWrapper->GetPosition(), pObjectWrapper->GetOrientation());
			}
			break;
			
		case eatRig:
			pObjectWrapper->AttachColliderRig(pAnimator->GetEngineCollider());
			break;
			
		default:
			break;
		};
		
	}catch(const deException &e){
		pAnimator->GetLogger()->LogException("Animator Editor", e);
	}
}

void aeAttachment::DetachCollider(){
	pObjectWrapper->DetachCollider();
}



// Private Functions
//////////////////////

void aeAttachment::pCleanUp(){
	SetAnimator(nullptr);
	if(pObjectWrapper){
		pObjectWrapper->SetAsyncLoadFinished(nullptr);
		pObjectWrapper.Clear();
	}
}
