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

// Constructor, destructor
////////////////////////////

aeController::aeController(aeWindowMain &windowMain, const char *aname) :
pMetaContext(aeMCController::Ref::New(windowMain, this)),
pAnimator(nullptr),
pIndex(-1),
pMPName(windowMain.GetMCAnimatorProperties().controller.name, pMetaContext, aname),
pMPMinimumValue(windowMain.GetMCAnimatorProperties().controller.minimumValue, pMetaContext),
pMPMaximumValue(windowMain.GetMCAnimatorProperties().controller.maximumValue, pMetaContext),
pMPCurrentValue(windowMain.GetMCAnimatorProperties().controller.currentValue, pMetaContext),
pMPClamp(windowMain.GetMCAnimatorProperties().controller.clamp, pMetaContext),
pMPFrozen(windowMain.GetMCAnimatorProperties().controller.frozen, pMetaContext),
pMPVector(windowMain.GetMCAnimatorProperties().controller.vector, pMetaContext),
pMPLocomotionAttribute(windowMain.GetMCAnimatorProperties().controller.locomotionAttribute, pMetaContext),
pMPLocomotionLeg(windowMain.GetMCAnimatorProperties().controller.locomotionLeg, pMetaContext),
pMPVectorSimulation(windowMain.GetMCAnimatorProperties().controller.vectorSimulation, pMetaContext),
pMPDefaultValue(windowMain.GetMCAnimatorProperties().controller.defaultValue, pMetaContext),
pMPDefaultVector(windowMain.GetMCAnimatorProperties().controller.defaultVector, pMetaContext)
{
	pMPName.onValueChanged = [this](){
		if(pAnimator){
			pAnimator->NotifyControllerNameChanged(this);
		}
	};
	
	pMPMinimumValue.onValueChanged = [this](){
		pOnLimitsChanged();
		pMPCurrentValue.SetLowerLimit(pMPMinimumValue);
		pMPCurrentValue.SetTickSpacing((pMPMaximumValue - pMPMinimumValue) / 10.0f);
	};
	
	pMPMaximumValue.onValueChanged = [this](){
		pOnLimitsChanged();
		pMPCurrentValue.SetUpperLimit(pMPMaximumValue);
		pMPCurrentValue.SetTickSpacing((pMPMaximumValue - pMPMinimumValue) / 10.0f);
	};
	
	pMPCurrentValue.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetCurrentValue(pMPCurrentValue);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerValueChanged();
	};
	
	pMPClamp.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetClamp(pMPClamp);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerChanged();
	};
	
	pMPFrozen.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetFrozen(pMPFrozen);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerChanged();
	};
	
	pMPVector.onValueChanged = [this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetVector(pMPVector);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerValueChanged();
		if(pGizmoIKPosition){
			pGizmoIKPosition->OnObjectGeometryChanged();
		}
	};
	
	pMPLocomotionAttribute.onValueChanged = [this](){ pNotifyControllerChanged(); };
	
	pMPVectorSimulation.onValueChanged = [this](){
		pReleaseGizmos();
		pCreateGizmos();
		pNotifyControllerChanged();
	};
	
	pMPDefaultValue.onValueChanged = [this](){ pNotifyControllerChanged(); };
	pMPDefaultVector.onValueChanged = [this](){ pNotifyControllerChanged(); };
}

aeController::aeController(aeWindowMain &windowMain, const aeController &copy) :
aeController(windowMain, copy.pMPName)
{
	pMPMinimumValue.SetValue(copy.pMPMinimumValue, false);
	pMPMaximumValue.SetValue(copy.pMPMaximumValue, false);
	pMPCurrentValue.SetValue(copy.pMPCurrentValue, false);
	pMPClamp.SetValue(copy.pMPClamp, false);
	pMPFrozen.SetValue(copy.pMPFrozen, false);
	pMPVector.SetValue(copy.pMPVector, false);
	pMPLocomotionAttribute.SetValue(copy.pMPLocomotionAttribute, false);
	pMPLocomotionLeg.SetValue(copy.pMPLocomotionLeg, false);
	pMPVectorSimulation.SetValue(copy.pMPVectorSimulation, false);
	pMPDefaultValue.SetValue(copy.pMPDefaultValue, false);
	pMPDefaultVector.SetValue(copy.pMPDefaultVector, false);
}

aeController::~aeController(){
	SetAnimator(nullptr);
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}



// Management
///////////////

void aeController::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	pReleaseGizmos();
	
	pAnimator = animator;
	pIndex = -1;
	
	pCreateGizmos();
}

void aeController::SetIndex(int index){
	if(index != pIndex){
		pIndex = index;
		
		if(index != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllers().GetAt(index);
			
			controller.SetName(pMPName);
			controller.SetValueRange(pMPMinimumValue, pMPMaximumValue);
			controller.SetCurrentValue(pMPCurrentValue);
			controller.SetFrozen(pMPFrozen);
			controller.SetClamp(pMPClamp);
			controller.SetVector(pMPVector);
			
			pMPCurrentValue = controller.GetCurrentValue();
			
			instance.NotifyControllerChangedAt(index);
		}
	}
}



void aeController::SetName(const char *aname){
	pMPName = aname;
}

void aeController::SetMinimumValue(float value){
	pMPMinimumValue = value;
}

void aeController::SetMaximumValue(float value){
	pMPMaximumValue = value;
}

void aeController::SetCurrentValue(float value){
	if(!pMPFrozen){
		pMPCurrentValue = pCheckValue(value);
	}
}

void aeController::IncrementCurrentValue(float incrementBy){
	SetCurrentValue(pMPCurrentValue + incrementBy);
}

void aeController::SetFrozen(bool afrozen){
	pMPFrozen = afrozen;
}

void aeController::SetClamp(bool aclamp){
	pMPClamp = aclamp;
}

void aeController::SetVector(const decVector &avector){
	pMPVector = avector;
}

void aeController::SetLocomotionAttribute(aeAnimatorLocomotion::eAttributes attribute){
	pMPLocomotionAttribute = attribute;
}

void aeController::SetLocomotionLeg(int leg){
	pMPLocomotionLeg = leg;
}

void aeController::SetVectorSimulation(eVectorSimulation simulation){
	pMPVectorSimulation = simulation;
}

void aeController::SetDefaultValue(float value){
	pMPDefaultValue = value;
}

void aeController::SetDefaultVector(const decVector &avector){
	pMPDefaultVector = avector;
}



void aeController::UpdateValue(float elapsed){
	const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	const bool wakeboarding = pAnimator->GetWakeboard().GetEnabled();
	const bool enabled = locomotion.GetEnabled();
	
	switch(pMPLocomotionAttribute){
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
			if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetGroundPosition());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if(enabled || wakeboarding){
			if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetGroundNormal());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if(enabled || wakeboarding){
			if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKInfluence());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if(enabled || wakeboarding){
			if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKInfluence());
				SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKPosition());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if(enabled || wakeboarding){
			if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKInfluence());
				SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKOrientation());
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
	
	switch(pMPLocomotionAttribute){
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
		if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetGroundPosition());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetGroundNormal());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKInfluence());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKInfluence());
			SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKPosition());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if(pMPLocomotionLeg >= 0 && pMPLocomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKInfluence());
			SetVector(locomotion.GetLegs().GetAt(pMPLocomotionLeg)->GetIKOrientation());
		}
		break;
		
	default:
		if(fullReset){
			SetCurrentValue(pMPDefaultValue);
			SetVector(pMPDefaultVector);
		}
		break;
	}
}

void aeController::InverseValue(){
	SetCurrentValue(pMPMaximumValue - pMPCurrentValue);
}



// Private Functions
//////////////////////

float aeController::pCheckValue(float value){
	float range = pMPMaximumValue - pMPMinimumValue;
	bool hasRange = range > 1e-5f;
	
	// make sure the current value is in the correct range
	if(pMPClamp){
		if(value < pMPMinimumValue){
			value = pMPMinimumValue;
			
		}else if(value > pMPMaximumValue){
			value = pMPMaximumValue;
		}
		
	}else{
		if(hasRange){
			value = fmodf(value - pMPMinimumValue, range);
			
			if(value < 0.0f){
				value += pMPMinimumValue + range;
				
			}else{
				value += pMPMinimumValue;
			}
			
		}else{
			value = pMPMinimumValue;
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
	
	switch(pMPVectorSimulation){
	case evsPosition:
		pGizmoIKPosition = aeGizmoControllerIKPosition::Ref::New(*pAnimator->GetEnvironment(), *this);
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
		instance->GetControllers()[pIndex]->SetValueRange(pMPMinimumValue, pMPMaximumValue);
		instance->NotifyControllerChangedAt(pIndex);
	}
	pNotifyControllerChanged();
}
