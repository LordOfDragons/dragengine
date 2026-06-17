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
name(windowMain.GetMCAnimatorProperties().link.name, pMetaContext, aname),
controller(windowMain.GetMCAnimatorProperties().link.controller, pMetaContext),
repeat(windowMain.GetMCAnimatorProperties().link.repeat, pMetaContext, 1),
curve(windowMain.GetMCAnimatorProperties().link.curve, pMetaContext),
bone(windowMain.GetMCAnimatorProperties().link.bone, pMetaContext),
boneParameter(windowMain.GetMCAnimatorProperties().link.boneParameter, pMetaContext),
boneMinimum(windowMain.GetMCAnimatorProperties().link.boneMinimum, pMetaContext),
boneMaximum(windowMain.GetMCAnimatorProperties().link.boneMaximum, pMetaContext),
vertexPositionSet(windowMain.GetMCAnimatorProperties().link.vertexPositionSet, pMetaContext),
vertexPositionSetMinimum(windowMain.GetMCAnimatorProperties().link.vertexPositionSetMinimum, pMetaContext),
vertexPositionSetMaximum(windowMain.GetMCAnimatorProperties().link.vertexPositionSetMaximum, pMetaContext),
wrapY(windowMain.GetMCAnimatorProperties().link.wrapY, pMetaContext)
{
	name.SetOnChanged([&](){
		if(pAnimator){
			pAnimator->NotifyLinkNameChanged(this);
			pAnimator->links.Property().NotifyObjectItemInfoChanged(pAnimator->GetMetaContext());
		}
	});
	
	controller.SetOnChanged([&](){
		UpdateController();
		if(pAnimator && !pTempNoNotify){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	
	repeat.SetOnChanged([&](){
		if(pEngLink){
			pEngLink->SetRepeat(repeat);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	
	curve.SetOnChanged([&](){
		pUpdateCurve();
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	
	bone.SetOnChanged([&](){
		if(pEngLink){
			pEngLink->SetBone(bone);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	
	boneParameter.SetOnChanged([&](){
		if(pEngLink){
			pEngLink->SetBoneParameter(boneParameter);
			pUpdateBoneLimits();
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	
	boneMinimum.SetOnChanged([&](){
		if(pEngLink){
			pUpdateBoneLimits();
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	boneMaximum.SetOnChanged(boneMinimum.GetOnChanged());
	
	vertexPositionSet.SetOnChanged([&](){
		if(pEngLink){
			pEngLink->SetVertexPositionSet(vertexPositionSet);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	
	vertexPositionSetMinimum.SetOnChanged([&](){
		if(pEngLink){
			pEngLink->SetVertexPositionSetValueRange(vertexPositionSetMinimum, vertexPositionSetMaximum);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
	vertexPositionSetMaximum.SetOnChanged(vertexPositionSetMinimum.GetOnChanged());
	
	wrapY.SetOnChanged([&](){
		if(pEngLink){
			pEngLink->SetWrapY(wrapY);
			NotifyLinkChanged();
		}
		if(pAnimator){
			pAnimator->NotifyLinkChanged(this);
		}
	});
}

aeLink::aeLink(aeWindowMain &windowMain, const aeLink &copy) :
aeLink(windowMain, copy.name)
{
	controller.SetValue(copy.controller.GetValue(), false);
	repeat.SetValue(copy.repeat.GetValue(), false);
	curve.SetValue(copy.curve.GetValue(), false);
	bone.SetValue(copy.bone.GetValue(), false);
	boneParameter.SetValue(copy.boneParameter.GetValue(), false);
	boneMinimum.SetValue(copy.boneMinimum.GetValue(), false);
	boneMaximum.SetValue(copy.boneMaximum.GetValue(), false);
	vertexPositionSet.SetValue(copy.vertexPositionSet.GetValue(), false);
	vertexPositionSetMinimum.SetValue(copy.vertexPositionSetMinimum.GetValue(), false);
	vertexPositionSetMaximum.SetValue(copy.vertexPositionSetMaximum.GetValue(), false);
	wrapY.SetValue(copy.wrapY.GetValue(), false);
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
		
		pEngLink->SetRepeat(repeat);
		pEngLink->SetBone(bone);
		pEngLink->SetBoneParameter(boneParameter);
		pEngLink->SetVertexPositionSet(vertexPositionSet);
		pEngLink->SetVertexPositionSetValueRange(vertexPositionSetMinimum, vertexPositionSetMaximum);
		pEngLink->SetWrapY(wrapY);
		pUpdateBoneLimits();
		
		NotifyLinkChanged();
		
		pUpdateCurve();
	}
}

void aeLink::SetName(const char *aname){
	name = aname;
}

void aeLink::SetController(aeController *acontroller, bool notify){
	pTempNoNotify = !notify;
	controller = acontroller;
	pTempNoNotify = false;
}

void aeLink::SetRepeat(int arepeat){
	repeat = arepeat;
}

void aeLink::SetCurve(const decCurveBezier &acurve){
	curve = acurve;
}

void aeLink::SetBone(const char *abone){
	bone = abone;
}

void aeLink::SetBoneParameter(deAnimatorLink::eBoneParameter parameter){
	boneParameter = parameter;
}

void aeLink::SetBoneMinimum(float value){
	boneMinimum = value;
}

void aeLink::SetBoneMaximum(float value){
	boneMaximum = value;
}

void aeLink::SetVertexPositionSet(const char *vps){
	vertexPositionSet = vps;
}

void aeLink::SetVertexPositionSetMinimum(float value){
	vertexPositionSetMinimum = value;
}

void aeLink::SetVertexPositionSetMaximum(float value){
	vertexPositionSetMaximum = value;
}

void aeLink::SetWrapY(bool wrap){
	wrapY = wrap;
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
	if(controller.GetValue().IsNotNull() && engAnimator){
		indexController = controller.GetValue()->GetIndex();
	}
	
	pEngLink->SetController(indexController);
	
	NotifyLinkChanged();
}



// Operators
//////////////

aeLink &aeLink::operator=(const aeLink &copy){
	name = copy.name;
	controller = copy.controller;
	repeat = copy.repeat;
	curve = copy.curve;
	bone = copy.bone;
	boneParameter = copy.boneParameter;
	boneMinimum = copy.boneMinimum;
	boneMaximum = copy.boneMaximum;
	vertexPositionSet = copy.vertexPositionSet;
	vertexPositionSetMinimum = copy.vertexPositionSetMinimum;
	vertexPositionSetMaximum = copy.vertexPositionSetMaximum;
	wrapY = copy.wrapY;
	return *this;
}

void aeLink::pUpdateCurve(){
	if(!pEngLink){
		return;
	}
	
	pEngLink->SetCurve(curve);
	
	NotifyLinkChanged();
}

void aeLink::pUpdateBoneLimits(){
	if(!pEngLink){
		return;
	}
	
	switch(boneParameter){
	case deAnimatorLink::ebpRotationX:
	case deAnimatorLink::ebpRotationY:
	case deAnimatorLink::ebpRotationZ:
		pEngLink->SetBoneValueRange(boneMinimum * DEG2RAD, boneMaximum * DEG2RAD);
		break;
		
	default:
		pEngLink->SetBoneValueRange(boneMinimum, boneMaximum);
	}
}
