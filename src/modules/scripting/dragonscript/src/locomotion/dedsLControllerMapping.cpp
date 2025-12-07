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

#include "dedsLocomotion.h"
#include "dedsLControllerMapping.h"

#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>



// Class dedsLControllerMapping
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dedsLControllerMapping::dedsLControllerMapping() :
pController(0),
pAttribute(eaElapsedTime){
}

dedsLControllerMapping::dedsLControllerMapping(const dedsLControllerMapping &mapping) :
pAnimatorInstance(mapping.pAnimatorInstance),
pController(mapping.pController),
pAttribute(mapping.pAttribute){
}



// Management
///////////////

void dedsLControllerMapping::SetAnimatorInstance(deAnimatorInstance *animatorInstance){
	pAnimatorInstance = animatorInstance;
}

void dedsLControllerMapping::SetController(int controller){
	pController = controller;
}

void dedsLControllerMapping::SetAttribute(eAttributes attribute){
	pAttribute = attribute;
}



void dedsLControllerMapping::Apply(const dedsLocomotion &locomotion, float elapsed){
	if(!pAnimatorInstance){
		return;
	}
	
	const int controllerCount = pAnimatorInstance->GetControllerCount();
	if(pController < 0 || pController >= controllerCount){
		return;
	}
	
	deAnimatorController &controller = pAnimatorInstance->GetControllerAt(pController);
	
	switch(pAttribute){
	case eaElapsedTime:
		controller.IncrementCurrentValue(elapsed);
		break;
		
	case eaLookVertical:
		controller.SetCurrentValue(locomotion.GetLookVertical().GetValue());
		break;
		
	case eaLookHorizontal:
		controller.SetCurrentValue(locomotion.GetLookHorizontal().GetValue());
		break;
		
	case eaMovingSpeed:
		controller.SetCurrentValue(locomotion.GetMovingSpeed());
		break;
		
	case eaMovingDirection:
		controller.SetCurrentValue(locomotion.GetMovingDirection());
		break;
		
	case eaRelativeMovingSpeed:
		if(fabs(locomotion.GetMovingDirection()) > 90.0f){
			controller.SetCurrentValue(-locomotion.GetMovingSpeed());
			
		}else{
			controller.SetCurrentValue(locomotion.GetMovingSpeed());
		}
		break;
		
	case eaTurningSpeed:
		controller.SetCurrentValue(locomotion.GetTurningSpeed());
		break;
		
	case eaStance:
		controller.SetCurrentValue(locomotion.GetStance().GetValue());
		break;
		
	case eaDisplacement:
		controller.IncrementCurrentValue(fabsf(locomotion.GetMovingSpeed()) * elapsed);
		break;
		
	case eaRelativeDisplacement:
		if(fabs(locomotion.GetMovingDirection()) > 90.0f){
			controller.IncrementCurrentValue(-fabsf(locomotion.GetMovingSpeed()) * elapsed);
			
		}else{
			controller.IncrementCurrentValue(fabsf(locomotion.GetMovingSpeed()) * elapsed);
		}
		break;
		
	case eaTimeTurnIP:
		if(locomotion.GetReverseTimeTurnIP()){
			controller.SetCurrentValue(controller.GetMaximumValue() - controller.GetCurrentValue());
		}
		if(locomotion.GetResetTimeTurnIP()){
			controller.SetCurrentValue(elapsed);
			
		}else{
			controller.IncrementCurrentValue(elapsed);
		}
		break;
		
	case eaTiltOffset:
		controller.SetCurrentValue(locomotion.GetTiltOffset());
		break;
		
	case eaTiltVertical:
		controller.SetCurrentValue(locomotion.GetTiltVertical().GetValue());
		break;
		
	case eaTiltHorizontal:
		controller.SetCurrentValue(locomotion.GetTiltHorizontal().GetValue());
		break;
		
	default:
		return;
	}
	
	pAnimatorInstance->NotifyControllerChangedAt(pController);
}

void dedsLControllerMapping::ApplyPost(const dedsLocomotion &locomotion, float elapsed){
	if(!pAnimatorInstance){
		return;
	}
	
	const int controllerCount = pAnimatorInstance->GetControllerCount();
	if(pController < 0 || pController >= controllerCount){
		return;
	}
	
	deAnimatorController &controller = pAnimatorInstance->GetControllerAt(pController);
	
	switch(pAttribute){
	case eaTiltOffset:
		controller.SetCurrentValue(locomotion.GetTiltOffset());
		break;
		
	case eaTiltVertical:
		controller.SetCurrentValue(locomotion.GetTiltVertical().GetValue());
		break;
		
	case eaTiltHorizontal:
		controller.SetCurrentValue(locomotion.GetTiltHorizontal().GetValue());
		break;
		
	default:
		return;
	}
	
	pAnimatorInstance->NotifyControllerChangedAt(pController);
}



// Operators
//////////////

dedsLControllerMapping &dedsLControllerMapping::operator=(const dedsLControllerMapping &mapping){
	pAnimatorInstance = mapping.pAnimatorInstance;
	pController = mapping.pController;
	pAttribute = mapping.pAttribute;
	return *this;
}
