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

#include "aeController.h"
#include "../aeAnimator.h"
#include "../locomotion/aeAnimatorLocomotion.h"
#include "../locomotion/aeAnimatorLocomotionLeg.h"
#include "../wakeboard/aeWakeboard.h"
#include "../../gui/aeWindowMain.h"
#include "../../visitors/aeElementVisitor.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/common/exceptions.h>



// Class aeController
///////////////////////

aeController::MetaContext::Ref aeController::CreateMetaContext(aeWindowMain &windowMain, aeController *controller){
	return MetaContext::Ref::New("animator.controller", "Controller", "Controller properties",
		windowMain.GetMCAnimatorProperties().controller.metaProperties, controller);
}

// Constructor, destructor
////////////////////////////

aeController::aeController(aeWindowMain &windowMain, const char *aname) :
pWindowMain(windowMain),
pMetaContext(CreateMetaContext(windowMain, this)),
pAnimator(nullptr),
pIndex(-1),
mpName(windowMain.GetMCAnimatorProperties().controller.name, pMetaContext, aname),
mpMinimumValue(windowMain.GetMCAnimatorProperties().controller.minimumValue, pMetaContext),
mpMaximumValue(windowMain.GetMCAnimatorProperties().controller.maximumValue, pMetaContext),
mpCurrentValue(windowMain.GetMCAnimatorProperties().controller.currentValue, pMetaContext),
mpClamp(windowMain.GetMCAnimatorProperties().controller.clamp, pMetaContext),
mpFrozen(windowMain.GetMCAnimatorProperties().controller.frozen, pMetaContext),
mpVector(windowMain.GetMCAnimatorProperties().controller.vector, pMetaContext),
mpLocomotionAttribute(windowMain.GetMCAnimatorProperties().controller.locomotionAttribute, pMetaContext),
mpLocomotionLeg(windowMain.GetMCAnimatorProperties().controller.locomotionLeg, pMetaContext),
mpVectorSimulation(windowMain.GetMCAnimatorProperties().controller.vectorSimulation, pMetaContext),
mpDefaultValue(windowMain.GetMCAnimatorProperties().controller.defaultValue, pMetaContext),
mpDefaultVector(windowMain.GetMCAnimatorProperties().controller.defaultVector, pMetaContext)
{
	mpName.onValueChanged = [this](){
		if(pAnimator){
			pAnimator->NotifyControllerNameChanged(this);
		}
	};
	
	mpMinimumValue.onValueChanged = [this](){
		pOnLimitsChanged();
		mpCurrentValue.SetLowerLimit(mpMinimumValue);
		mpCurrentValue.SetTickSpacing((mpMaximumValue - mpMinimumValue) / 10.0f);
	};
	
	mpMaximumValue.onValueChanged = [this](){
		pOnLimitsChanged();
		mpCurrentValue.SetUpperLimit(mpMaximumValue);
		mpCurrentValue.SetTickSpacing((mpMaximumValue - mpMinimumValue) / 10.0f);
	};
	
	mpCurrentValue.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetCurrentValue(mpCurrentValue);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerValueChanged();
	};
	
	mpClamp.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetClamp(mpClamp);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerChanged();
	};
	
	mpFrozen.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetFrozen(mpFrozen);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerChanged();
	};
	
	mpVector.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetVector(mpVector);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerValueChanged();
		if(pGizmoIKPosition){
			pGizmoIKPosition->OnObjectGeometryChanged();
		}
	};
	
	mpLocomotionAttribute.onValueChanged = [this](){ pNotifyControllerChanged(); };
	
	mpVectorSimulation.onValueChanged = [this](){
		pReleaseGizmos();
		pCreateGizmos();
		pNotifyControllerChanged();
	};
	
	mpDefaultValue.onValueChanged = [this](){ pNotifyControllerChanged(); };
	mpDefaultVector.onValueChanged = [this](){ pNotifyControllerChanged(); };
}

aeController::aeController(const aeController &copy) :
aeController(copy.pWindowMain, copy.mpName)
{
	mpMinimumValue.SetValue(copy.mpMinimumValue, false);
	mpMaximumValue.SetValue(copy.mpMaximumValue, false);
	mpCurrentValue.SetValue(copy.mpCurrentValue, false);
	mpClamp.SetValue(copy.mpClamp, false);
	mpFrozen.SetValue(copy.mpFrozen, false);
	mpVector.SetValue(copy.mpVector, false);
	mpLocomotionAttribute.SetValue(copy.mpLocomotionAttribute, false);
	mpLocomotionLeg.SetValue(copy.mpLocomotionLeg, false);
	mpVectorSimulation.SetValue(copy.mpVectorSimulation, false);
	mpDefaultValue.SetValue(copy.mpDefaultValue, false);
	mpDefaultVector.SetValue(copy.mpDefaultVector, false);
}

aeController::~aeController(){
	SetAnimator(nullptr);
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}



// Management
///////////////

aeAnimator &aeController::GetAnimatorRef() const{
	DEASSERT_NOTNULL(pAnimator)
	return *pAnimator;
}

void aeController::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	pReleaseGizmos();
	
	pAnimator = animator;
	pIndex = -1;
	
	pCreateGizmos();
}

igdeEnvironment &aeController::GetEnvironment() const{
	return GetAnimatorRef().GetEnvironment();
}

igdeUndoSystem *aeController::GetUndoSystem() const{
	return GetAnimatorRef().GetUndoSystem();
}

void aeController::SetIndex(int index){
	if(index != pIndex){
		pIndex = index;
		
		if(index != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllers().GetAt(index);
			
			controller.SetName(mpName);
			controller.SetValueRange(mpMinimumValue, mpMaximumValue);
			controller.SetCurrentValue(mpCurrentValue);
			controller.SetFrozen(mpFrozen);
			controller.SetClamp(mpClamp);
			controller.SetVector(mpVector);
			
			mpCurrentValue = controller.GetCurrentValue();
			
			instance.NotifyControllerChangedAt(index);
		}
	}
}



void aeController::SetName(const char *aname){
	mpName = aname;
}

void aeController::SetMinimumValue(float value){
	mpMinimumValue = value;
}

void aeController::SetMaximumValue(float value){
	mpMaximumValue = value;
}

void aeController::SetCurrentValue(float value){
	if(!mpFrozen){
		mpCurrentValue = pCheckValue(value);
	}
}

void aeController::IncrementCurrentValue(float incrementBy){
	SetCurrentValue(mpCurrentValue + incrementBy);
}

void aeController::SetFrozen(bool afrozen){
	mpFrozen = afrozen;
}

void aeController::SetClamp(bool aclamp){
	mpClamp = aclamp;
}

void aeController::SetVector(const decVector &avector){
	mpVector = avector;
}

void aeController::SetLocomotionAttribute(aeAnimatorLocomotion::eAttributes attribute){
	mpLocomotionAttribute = attribute;
}

void aeController::SetLocomotionLeg(int leg){
	mpLocomotionLeg = leg;
}

void aeController::SetVectorSimulation(eVectorSimulation simulation){
	mpVectorSimulation = simulation;
}

void aeController::SetDefaultValue(float value){
	mpDefaultValue = value;
}

void aeController::SetDefaultVector(const decVector &avector){
	mpDefaultVector = avector;
}



void aeController::UpdateValue(float elapsed){
	const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	const bool wakeboarding = pAnimator->GetWakeboard().GetEnabled();
	const bool enabled = locomotion.GetEnabled();
	
	switch(mpLocomotionAttribute){
	case aeAnimatorLocomotion::eaElapsedTime:
		IncrementCurrentValue(elapsed);
		break;
		
	case aeAnimatorLocomotion::eaMovingSpeed:
		if(enabled){
			SetCurrentValue(locomotion.GetMovingSpeed());
		}
		break;
		
	case aeAnimatorLocomotion::eaMovingDirection:
		if(enabled){
			SetCurrentValue(locomotion.GetMovingDirection());
		}
		break;
		
	case aeAnimatorLocomotion::eaRelativeMovingSpeed:
		if(enabled){
			SetCurrentValue(locomotion.GetRelativeMovingSpeed());
		}
		break;
		
	case aeAnimatorLocomotion::eaTurningSpeed:
		if(enabled){
			SetCurrentValue(locomotion.GetTurnVelocity());
		}
		break;
		
	case aeAnimatorLocomotion::eaStance:
		if(enabled){
			SetCurrentValue(locomotion.GetStance().GetValue());
		}
		break;
		
	case aeAnimatorLocomotion::eaDisplacement:
		if(enabled){
			IncrementCurrentValue(locomotion.GetMovingSpeed() * elapsed);
		}
		break;
		
	case aeAnimatorLocomotion::eaRelativeDisplacement:
		if(enabled){
			IncrementCurrentValue(locomotion.GetRelativeMovingSpeed() * elapsed);
		}
		break;
		
	case aeAnimatorLocomotion::eaLookUpDown:
		if(enabled || wakeboarding){
			SetCurrentValue(locomotion.GetLookUpDown().GetValue());
		}
		break;
		
	case aeAnimatorLocomotion::eaLookLeftRight:
		if(enabled || wakeboarding){
			SetCurrentValue(locomotion.GetLookLeftRight().GetValue());
		}
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltOffset:
		if(enabled || wakeboarding){
			SetCurrentValue(locomotion.GetTiltOffset());
		}
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltUpDown:
		if(enabled || wakeboarding){
			SetCurrentValue(locomotion.GetTiltUpDown());
		}
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltRightLeft:
		if(enabled || wakeboarding){
			SetCurrentValue(locomotion.GetTiltLeftRight());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundPosition:
		if(enabled || wakeboarding){
			if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetGroundPosition());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if(enabled || wakeboarding){
			if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetGroundNormal());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if(enabled || wakeboarding){
			if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKInfluence());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if(enabled || wakeboarding){
			if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKInfluence());
				SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKPosition());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if(enabled || wakeboarding){
			if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKInfluence());
				SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKOrientation());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaTimeTurnIP:
	case aeAnimatorLocomotion::eaNone:
		break;
	}
}

void aeController::ResetValue(bool fullReset){
	const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	
	switch(mpLocomotionAttribute){
	case aeAnimatorLocomotion::eaElapsedTime:
	case aeAnimatorLocomotion::eaTimeTurnIP:
		SetCurrentValue(0.0f);
		break;
		
	case aeAnimatorLocomotion::eaLookUpDown:
		SetCurrentValue(locomotion.GetLookUpDown().GetValue());
		break;
		
	case aeAnimatorLocomotion::eaLookLeftRight:
		SetCurrentValue(locomotion.GetLookLeftRight().GetValue());
		break;
		
	case aeAnimatorLocomotion::eaMovingSpeed:
		SetCurrentValue(locomotion.GetMovingSpeed());
		break;
		
	case aeAnimatorLocomotion::eaMovingDirection:
		SetCurrentValue(locomotion.GetMovingDirection());
		break;
		
	case aeAnimatorLocomotion::eaRelativeMovingSpeed:
		SetCurrentValue(locomotion.GetRelativeMovingSpeed());
		break;
		
	case aeAnimatorLocomotion::eaTurningSpeed:
		SetCurrentValue(locomotion.GetTurnVelocity());
		break;
		
	case aeAnimatorLocomotion::eaStance:
		SetCurrentValue(locomotion.GetStance().GetValue());
		break;
		
	case aeAnimatorLocomotion::eaDisplacement:
	case aeAnimatorLocomotion::eaRelativeDisplacement:
		SetCurrentValue(0.0f);
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltOffset:
		SetCurrentValue(locomotion.GetTiltOffset());
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltUpDown:
		SetCurrentValue(locomotion.GetTiltUpDown());
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltRightLeft:
		SetCurrentValue(locomotion.GetTiltLeftRight());
		break;
		
	case aeAnimatorLocomotion::eaLegGroundPosition:
		if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetGroundPosition());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetGroundNormal());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKInfluence());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKInfluence());
			SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKPosition());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if(mpLocomotionLeg >= 0 && mpLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKInfluence());
			SetVector(locomotion.GetLegs().GetAt(mpLocomotionLeg)->GetIKOrientation());
		}
		break;
		
	default:
		if(fullReset){
			SetCurrentValue(mpDefaultValue);
			SetVector(mpDefaultVector);
		}
		break;
	}
}

void aeController::InverseValue(){
	SetCurrentValue(mpMaximumValue - mpCurrentValue);
}



// Private Functions
//////////////////////

float aeController::pCheckValue(float value){
	float range = mpMaximumValue - mpMinimumValue;
	bool hasRange = range > 1e-5f;
	
	// make sure the current value is in the correct range
	if(mpClamp){
		if(value < mpMinimumValue){
			value = mpMinimumValue;
			
		}else if(value > mpMaximumValue){
			value = mpMaximumValue;
		}
		
	}else{
		if(hasRange){
			value = fmodf(value - mpMinimumValue, range);
			
			if(value < 0.0f){
				value += mpMinimumValue + range;
				
			}else{
				value += mpMinimumValue;
			}
			
		}else{
			value = mpMinimumValue;
		}
	}
	
	return value;
}

void aeController::pReleaseGizmos(){
	if(!pGizmoIKPosition){
		return;
	}
	
	pGizmoIKPosition->SetWorld(nullptr);
	pGizmoIKPosition = nullptr;
}

void aeController::pCreateGizmos(){
	if(pGizmoIKPosition || !pAnimator){
		return;
	}
	
	switch(mpVectorSimulation){
	case evsPosition:
		pGizmoIKPosition = aeGizmoControllerIKPosition::Ref::New(pAnimator->GetEnvironment(), *this);
		pGizmoIKPosition->SetWorld(pAnimator->GetEngineWorld());
		break;
		
	default:
		break;
	}
}

void aeController::pNotifyControllerChanged(){
	if(pAnimator){
		pAnimator->NotifyControllerChanged(this);
	}
}

void aeController::pNotifyControllerValueChanged(){
	if(pAnimator){
		pAnimator->NotifyControllerValueChanged(this);
	}
}

void aeController::pOnLimitsChanged(){
	if(pIndex != -1){
		auto &instance = pAnimator->GetEngineAnimatorInstance();
		instance->GetControllers()[pIndex]->SetValueRange(mpMinimumValue, mpMaximumValue);
		instance->NotifyControllerChangedAt(pIndex);
	}
	pNotifyControllerChanged();
}
