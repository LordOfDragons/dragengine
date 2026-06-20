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
name(windowMain.GetMCAnimatorProperties().controller.name, pMetaContext, aname),
minimumValue(windowMain.GetMCAnimatorProperties().controller.minimumValue, pMetaContext),
maximumValue(windowMain.GetMCAnimatorProperties().controller.maximumValue, pMetaContext),
currentValue(windowMain.GetMCAnimatorProperties().controller.currentValue, pMetaContext),
clamp(windowMain.GetMCAnimatorProperties().controller.clamp, pMetaContext),
frozen(windowMain.GetMCAnimatorProperties().controller.frozen, pMetaContext),
vector(windowMain.GetMCAnimatorProperties().controller.vector, pMetaContext),
locomotionAttribute(windowMain.GetMCAnimatorProperties().controller.locomotionAttribute, pMetaContext),
locomotionLeg(windowMain.GetMCAnimatorProperties().controller.locomotionLeg, pMetaContext),
vectorSimulation(windowMain.GetMCAnimatorProperties().controller.vectorSimulation, pMetaContext),
defaultValue(windowMain.GetMCAnimatorProperties().controller.defaultValue, pMetaContext),
defaultVector(windowMain.GetMCAnimatorProperties().controller.defaultVector, pMetaContext)
{
	name.SetOnChanged([this](){
		if(pAnimator){
			pAnimator->NotifyControllerNameChanged(this);
		}
	});
	
	minimumValue.SetOnChanged([this](){
		pOnLimitsChanged();
		currentValue.SetLowerLimit(minimumValue);
	});
	
	maximumValue.SetOnChanged([this](){
		pOnLimitsChanged();
		currentValue.SetUpperLimit(maximumValue);
	});
	
	currentValue.SetOnChanged([this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetCurrentValue(currentValue);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerValueChanged();
	});
	
	clamp.SetOnChanged([this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetClamp(clamp);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerChanged();
	});
	
	frozen.SetOnChanged([this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetFrozen(frozen);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerChanged();
	});
	
	vector.SetOnChanged([this](){
		if(pIndex != -1){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			instance.GetControllers()[pIndex]->SetVector(vector);
			instance.NotifyControllerChangedAt(pIndex);
		}
		pNotifyControllerValueChanged();
		if(pGizmoIKPosition){
			pGizmoIKPosition->OnObjectGeometryChanged();
		}
	});
	
	locomotionAttribute.SetOnChanged([this](){ pNotifyControllerChanged(); });
	
	vectorSimulation.SetOnChanged([this](){
		pReleaseGizmos();
		pCreateGizmos();
		pNotifyControllerChanged();
	});
	
	defaultValue.SetOnChanged([this](){ pNotifyControllerChanged(); });
	defaultVector.SetOnChanged([this](){ pNotifyControllerChanged(); });
}

aeController::aeController(aeWindowMain &windowMain, const aeController &copy) :
aeController(windowMain, copy.name)
{
	minimumValue.SetValue(copy.minimumValue, false);
	maximumValue.SetValue(copy.maximumValue, false);
	currentValue.SetValue(copy.currentValue, false);
	clamp.SetValue(copy.clamp, false);
	frozen.SetValue(copy.frozen, false);
	vector.SetValue(copy.vector, false);
	locomotionAttribute.SetValue(copy.locomotionAttribute, false);
	locomotionLeg.SetValue(copy.locomotionLeg, false);
	vectorSimulation.SetValue(copy.vectorSimulation, false);
	defaultValue.SetValue(copy.defaultValue, false);
	defaultVector.SetValue(copy.defaultVector, false);
}

aeController::~aeController(){
	pCleanUp();
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
			
			controller.SetName(name);
			controller.SetValueRange(minimumValue, maximumValue);
			controller.SetCurrentValue(currentValue);
			controller.SetFrozen(frozen);
			controller.SetClamp(clamp);
			controller.SetVector(vector);
			
			currentValue = controller.GetCurrentValue();
			
			instance.NotifyControllerChangedAt(index);
		}
	}
}



void aeController::SetName(const char *aname){
	name = aname;
}

void aeController::SetMinimumValue(float value){
	minimumValue = value;
}

void aeController::SetMaximumValue(float value){
	maximumValue = value;
}

void aeController::SetCurrentValue(float value){
	if(!frozen){
		currentValue = pCheckValue(value);
	}
}

void aeController::IncrementCurrentValue(float incrementBy){
	SetCurrentValue(currentValue + incrementBy);
}

void aeController::SetFrozen(bool afrozen){
	frozen = afrozen;
}

void aeController::SetClamp(bool aclamp){
	clamp = aclamp;
}

void aeController::SetVector(const decVector &avector){
	vector = avector;
}

void aeController::SetLocomotionAttribute(aeAnimatorLocomotion::eAttributes attribute){
	locomotionAttribute = attribute;
}

void aeController::SetLocomotionLeg(int leg){
	locomotionLeg = leg;
}

void aeController::SetVectorSimulation(eVectorSimulation simulation){
	vectorSimulation = simulation;
}

void aeController::SetDefaultValue(float value){
	defaultValue = value;
}

void aeController::SetDefaultVector(const decVector &avector){
	defaultVector = avector;
}



void aeController::UpdateValue(float elapsed){
	const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	const bool wakeboarding = pAnimator->GetWakeboard().GetEnabled();
	const bool enabled = locomotion.GetEnabled();
	
	switch(locomotionAttribute){
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
			if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
				SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetGroundPosition());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if(enabled || wakeboarding){
			if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
				SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetGroundNormal());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if(enabled || wakeboarding){
			if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKInfluence());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if(enabled || wakeboarding){
			if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKInfluence());
				SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKPosition());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if(enabled || wakeboarding){
			if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
				SetCurrentValue(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKInfluence());
				SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKOrientation());
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaTimeTurnIP:
	case aeAnimatorLocomotion::eaNone:
		break;
	}
}

void aeController::ResetValue(){
	const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	
	switch(locomotionAttribute){
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
		if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
			SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetGroundPosition());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
			SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetGroundNormal());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKInfluence());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKInfluence());
			SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKPosition());
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if(locomotionLeg >= 0 && locomotionLeg < locomotion.GetLegs().GetCount()){
			SetCurrentValue(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKInfluence());
			SetVector(locomotion.GetLegs().GetAt(locomotionLeg)->GetIKOrientation());
		}
		break;
		
	default:
		SetCurrentValue(defaultValue);
		SetVector(defaultVector);
	}
}

void aeController::InverseValue(){
	SetCurrentValue(maximumValue - currentValue);
}



// Private Functions
//////////////////////

void aeController::pCleanUp(){
	SetAnimator(nullptr);
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}

float aeController::pCheckValue(float value){
	float range = maximumValue - minimumValue;
	bool hasRange = range > 1e-5f;
	
	// make sure the current value is in the correct range
	if(clamp){
		if(value < minimumValue){
			value = minimumValue;
			
		}else if(value > maximumValue){
			value = maximumValue;
		}
		
	}else{
		if(hasRange){
			value = fmodf(value - minimumValue, range);
			
			if(value < 0.0f){
				value += minimumValue + range;
				
			}else{
				value += minimumValue;
			}
			
		}else{
			value = minimumValue;
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
	
	switch(vectorSimulation){
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
		instance->GetControllers()[pIndex]->SetValueRange(minimumValue, maximumValue);
		instance->NotifyControllerChangedAt(pIndex);
	}
	pNotifyControllerChanged();
}
