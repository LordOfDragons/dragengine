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

#include "aeLink.h"
#include "../aeAnimator.h"
#include "../controller/aeController.h"
#include "../../gui/aeWindowMain.h"
#include "../../meta/animator/aeMCPLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>



// Class aeLink
/////////////////

aeLink::MetaContext::Ref aeLink::CreateMetaContext(aeWindowMain &windowMain, aeLink *link){
	return MetaContext::Ref::New("animator.link", "Link", "Link properties",
		windowMain.GetMCAnimatorProperties().link.metaProperties, link);
}

// Constructor, destructor
////////////////////////////

aeLink::aeLink(aeWindowMain &windowMain, const char *aname) :
pWindowMain(windowMain),
pMetaContext(CreateMetaContext(windowMain, this)),
pAnimator(nullptr),
pEngLink(nullptr),
mpName(windowMain.GetMCAnimatorProperties().link.name, pMetaContext, aname),
mpController(windowMain.GetMCAnimatorProperties().link.controller, pMetaContext),
mpRepeat(windowMain.GetMCAnimatorProperties().link.repeat, pMetaContext, 1),
mpCurve(windowMain.GetMCAnimatorProperties().link.curve, pMetaContext),
mpBone(windowMain.GetMCAnimatorProperties().link.bone, pMetaContext),
mpBoneParameter(windowMain.GetMCAnimatorProperties().link.boneParameter, pMetaContext),
mpBoneMinimum(windowMain.GetMCAnimatorProperties().link.boneMinimum, pMetaContext),
mpBoneMaximum(windowMain.GetMCAnimatorProperties().link.boneMaximum, pMetaContext),
mpVertexPositionSet(windowMain.GetMCAnimatorProperties().link.vertexPositionSet, pMetaContext),
mpVpsMinimum(windowMain.GetMCAnimatorProperties().link.vertexPositionSetMinimum, pMetaContext),
mpVpsMaximum(windowMain.GetMCAnimatorProperties().link.vertexPositionSetMaximum, pMetaContext),
mpWrapY(windowMain.GetMCAnimatorProperties().link.wrapY, pMetaContext)
{
	mpName.onValueChanged = [&](){
		if(pAnimator){
			pAnimator->NotifyLinkNameChanged(this);
		}
	};
	
	mpController.onValueChanged = [&](){
		UpdateController();
		if(pAnimator && !pTempNoNotify){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	mpRepeat.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetRepeat(mpRepeat);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	mpCurve.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetCurve(mpCurve);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	mpBone.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetBone(mpBone);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	mpBoneParameter.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetBoneParameter(mpBoneParameter);
			pUpdateBoneLimits();
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	mpBoneMinimum.onValueChanged = [&](){
		if(pEngLink){
			pUpdateBoneLimits();
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	mpBoneMaximum.onValueChanged = mpBoneMinimum.onValueChanged;
	
	mpVertexPositionSet.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetVertexPositionSet(mpVertexPositionSet);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	mpVpsMinimum.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetVertexPositionSetValueRange(mpVpsMinimum, mpVpsMaximum);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	mpVpsMaximum.onValueChanged = mpVpsMinimum.onValueChanged;
	
	mpWrapY.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetWrapY(mpWrapY);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
}

aeLink::aeLink(const aeLink &copy) :
aeLink(copy.pWindowMain, copy.mpName)
{
	mpController.SetValue(copy.mpController, false);
	mpRepeat.SetValue(copy.mpRepeat, false);
	mpCurve.SetValue(copy.mpCurve, false);
	mpBone.SetValue(copy.mpBone, false);
	mpBoneParameter.SetValue(copy.mpBoneParameter, false);
	mpBoneMinimum.SetValue(copy.mpBoneMinimum, false);
	mpBoneMaximum.SetValue(copy.mpBoneMaximum, false);
	mpVertexPositionSet.SetValue(copy.mpVertexPositionSet, false);
	mpVpsMinimum.SetValue(copy.mpVpsMinimum, false);
	mpVpsMaximum.SetValue(copy.mpVpsMaximum, false);
	mpWrapY.SetValue(copy.mpWrapY, false);
}

aeLink::~aeLink(){
	SetAnimator(nullptr);
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}



// Management
///////////////

aeAnimator &aeLink::GetAnimatorRef() const{
	DEASSERT_NOTNULL(pAnimator)
	return *pAnimator;
}

void aeLink::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	if(pAnimator && pEngLink){
		pAnimator->GetEngineAnimator()->RemoveLink(pEngLink);
		pEngLink = nullptr;
	}
	
	pAnimator = animator;
	
	if(animator){
		const deAnimatorLink::Ref link(deAnimatorLink::Ref::New());
		animator->GetEngineAnimator()->AddLink(link);
		pEngLink = link;
		
		UpdateController();
		
		pEngLink->SetCurve(mpCurve);
		pEngLink->SetRepeat(mpRepeat);
		pEngLink->SetBone(mpBone);
		pEngLink->SetBoneParameter(mpBoneParameter);
		pEngLink->SetVertexPositionSet(mpVertexPositionSet);
		pEngLink->SetVertexPositionSetValueRange(mpVpsMinimum, mpVpsMaximum);
		pEngLink->SetWrapY(mpWrapY);
		pUpdateBoneLimits();
		
		NotifyLinkChanged();
	}
}

igdeEnvironment &aeLink::GetEnvironment() const{
	return GetAnimatorRef().GetEnvironment();
}

igdeUndoSystem *aeLink::GetUndoSystem() const{
	return GetAnimatorRef().GetUndoSystem();
}

void aeLink::SetName(const char *aname){
	mpName = aname;
}

void aeLink::SetController(aeController *acontroller, bool notify){
	pTempNoNotify = !notify;
	mpController = acontroller;
	pTempNoNotify = false;
}

void aeLink::SetRepeat(int arepeat){
	mpRepeat = arepeat;
}

void aeLink::SetCurve(const decCurveBezier &acurve){
	mpCurve = acurve;
}

void aeLink::SetBone(const char *abone){
	mpBone = abone;
}

void aeLink::SetBoneParameter(deAnimatorLink::eBoneParameter parameter){
	mpBoneParameter = parameter;
}

void aeLink::SetBoneMinimum(float value){
	mpBoneMinimum = value;
}

void aeLink::SetBoneMaximum(float value){
	mpBoneMaximum = value;
}

void aeLink::SetVertexPositionSet(const char *vps){
	mpVertexPositionSet = vps;
}

void aeLink::SetVertexPositionSetMinimum(float value){
	mpVpsMinimum = value;
}

void aeLink::SetVertexPositionSetMaximum(float value){
	mpVpsMaximum = value;
}

void aeLink::SetWrapY(bool wrap){
	mpWrapY = wrap;
}



void aeLink::NotifyLinkChanged(){
	if(pEngLink && pAnimator){
		deAnimator *engAnimator = pAnimator->GetEngineAnimator();
		
		engAnimator->NotifyLinkChangedAt(engAnimator->GetLinks().IndexOf(pEngLink));
	}
}



void aeLink::UpdateController(){
	if(!pEngLink){
		return;
	}
	
	deAnimator *engAnimator = pAnimator->GetEngineAnimator();
	int indexController = -1;
	if(mpController.GetValue().IsNotNull() && engAnimator){
		indexController = mpController.GetValue()->GetIndex();
	}
	
	pEngLink->SetController(indexController);
	
	NotifyLinkChanged();
}


void aeLink::pUpdateBoneLimits(){
	if(!pEngLink){
		return;
	}
	
	switch(mpBoneParameter){
	case deAnimatorLink::ebpRotationX:
	case deAnimatorLink::ebpRotationY:
	case deAnimatorLink::ebpRotationZ:
		pEngLink->SetBoneValueRange(mpBoneMinimum * DEG2RAD, mpBoneMaximum * DEG2RAD);
		break;
		
	default:
		pEngLink->SetBoneValueRange(mpBoneMinimum, mpBoneMaximum);
	}
}
