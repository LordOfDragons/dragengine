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

// Constructor, destructor
////////////////////////////

aeLink::aeLink(aeWindowMain &windowMain, const char *aname) :
pMetaContext(aeMCLink::Ref::New(windowMain, this)),
pAnimator(nullptr),
pEngLink(nullptr),
pMPName(windowMain.GetMCAnimatorProperties().link.name, pMetaContext, aname),
pMPController(windowMain.GetMCAnimatorProperties().link.controller, pMetaContext),
pMPRepeat(windowMain.GetMCAnimatorProperties().link.repeat, pMetaContext, 1),
pMPCurve(windowMain.GetMCAnimatorProperties().link.curve, pMetaContext),
pMPBone(windowMain.GetMCAnimatorProperties().link.bone, pMetaContext),
pMPBoneParameter(windowMain.GetMCAnimatorProperties().link.boneParameter, pMetaContext),
pMPBoneMinimum(windowMain.GetMCAnimatorProperties().link.boneMinimum, pMetaContext),
pMPBoneMaximum(windowMain.GetMCAnimatorProperties().link.boneMaximum, pMetaContext),
pMPVertexPositionSet(windowMain.GetMCAnimatorProperties().link.vertexPositionSet, pMetaContext),
pMPVpsMinimum(windowMain.GetMCAnimatorProperties().link.vertexPositionSetMinimum, pMetaContext),
pMPVpsMaximum(windowMain.GetMCAnimatorProperties().link.vertexPositionSetMaximum, pMetaContext),
pMPWrapY(windowMain.GetMCAnimatorProperties().link.wrapY, pMetaContext)
{
	pMPName.onValueChanged = [&](){
		if(pAnimator){
			pAnimator->NotifyLinkNameChanged(this);
		}
	};
	
	pMPController.onValueChanged = [&](){
		UpdateController();
		if(pAnimator && !pTempNoNotify){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	pMPRepeat.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetRepeat(pMPRepeat);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	pMPCurve.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetCurve(pMPCurve);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	pMPBone.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetBone(pMPBone);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	pMPBoneParameter.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetBoneParameter(pMPBoneParameter);
			pUpdateBoneLimits();
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	pMPBoneMinimum.onValueChanged = [&](){
		if(pEngLink){
			pUpdateBoneLimits();
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	pMPBoneMaximum.onValueChanged = pMPBoneMinimum.onValueChanged;
	
	pMPVertexPositionSet.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetVertexPositionSet(pMPVertexPositionSet);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	
	pMPVpsMinimum.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetVertexPositionSetValueRange(pMPVpsMinimum, pMPVpsMaximum);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
	pMPVpsMaximum.onValueChanged = pMPVpsMinimum.onValueChanged;
	
	pMPWrapY.onValueChanged = [&](){
		if(pEngLink){
			pEngLink->SetWrapY(pMPWrapY);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	};
}

aeLink::aeLink(aeWindowMain &windowMain, const aeLink &copy) :
aeLink(windowMain, copy.pMPName)
{
	pMPController.SetValue(copy.pMPController, false);
	pMPRepeat.SetValue(copy.pMPRepeat, false);
	pMPCurve.SetValue(copy.pMPCurve, false);
	pMPBone.SetValue(copy.pMPBone, false);
	pMPBoneParameter.SetValue(copy.pMPBoneParameter, false);
	pMPBoneMinimum.SetValue(copy.pMPBoneMinimum, false);
	pMPBoneMaximum.SetValue(copy.pMPBoneMaximum, false);
	pMPVertexPositionSet.SetValue(copy.pMPVertexPositionSet, false);
	pMPVpsMinimum.SetValue(copy.pMPVpsMinimum, false);
	pMPVpsMaximum.SetValue(copy.pMPVpsMaximum, false);
	pMPWrapY.SetValue(copy.pMPWrapY, false);
}

aeLink::~aeLink(){
	SetAnimator(nullptr);
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}



// Management
///////////////

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
		
		pEngLink->SetCurve(pMPCurve);
		pEngLink->SetRepeat(pMPRepeat);
		pEngLink->SetBone(pMPBone);
		pEngLink->SetBoneParameter(pMPBoneParameter);
		pEngLink->SetVertexPositionSet(pMPVertexPositionSet);
		pEngLink->SetVertexPositionSetValueRange(pMPVpsMinimum, pMPVpsMaximum);
		pEngLink->SetWrapY(pMPWrapY);
		pUpdateBoneLimits();
		
		NotifyLinkChanged();
	}
}

void aeLink::SetName(const char *aname){
	pMPName = aname;
}

void aeLink::SetController(aeController *acontroller, bool notify){
	pTempNoNotify = !notify;
	pMPController = acontroller;
	pTempNoNotify = false;
}

void aeLink::SetRepeat(int arepeat){
	pMPRepeat = arepeat;
}

void aeLink::SetCurve(const decCurveBezier &acurve){
	pMPCurve = acurve;
}

void aeLink::SetBone(const char *abone){
	pMPBone = abone;
}

void aeLink::SetBoneParameter(deAnimatorLink::eBoneParameter parameter){
	pMPBoneParameter = parameter;
}

void aeLink::SetBoneMinimum(float value){
	pMPBoneMinimum = value;
}

void aeLink::SetBoneMaximum(float value){
	pMPBoneMaximum = value;
}

void aeLink::SetVertexPositionSet(const char *vps){
	pMPVertexPositionSet = vps;
}

void aeLink::SetVertexPositionSetMinimum(float value){
	pMPVpsMinimum = value;
}

void aeLink::SetVertexPositionSetMaximum(float value){
	pMPVpsMaximum = value;
}

void aeLink::SetWrapY(bool wrap){
	pMPWrapY = wrap;
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
	if(pMPController.GetValue().IsNotNull() && engAnimator){
		indexController = pMPController.GetValue()->GetIndex();
	}
	
	pEngLink->SetController(indexController);
	
	NotifyLinkChanged();
}


void aeLink::pUpdateBoneLimits(){
	if(!pEngLink){
		return;
	}
	
	switch(pMPBoneParameter){
	case deAnimatorLink::ebpRotationX:
	case deAnimatorLink::ebpRotationY:
	case deAnimatorLink::ebpRotationZ:
		pEngLink->SetBoneValueRange(pMPBoneMinimum * DEG2RAD, pMPBoneMaximum * DEG2RAD);
		break;
		
	default:
		pEngLink->SetBoneValueRange(pMPBoneMinimum, pMPBoneMaximum);
	}
}
