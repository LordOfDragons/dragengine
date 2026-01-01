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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "skyeController.h"
#include "../skyeSky.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/resources/sky/deSkyInstance.h>



// Class skyeController
///////////////////////

// Constructor, destructor
////////////////////////////

skyeController::skyeController() :
pSky(nullptr),
pIndex(-1),

pName("Controller"),

pMinValue(0.0f),
pMaxValue(1.0f),
pCurValue(0.0f),
pClamp(true),
pFrozen(false),

pSelected(false),
pActive(false){
}

skyeController::~skyeController(){
}



// Management
///////////////

void skyeController::SetSky(skyeSky *sky){
	pSky = sky;
}

void skyeController::SetIndex(int index){
	pIndex = index;
}

void skyeController::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	if(pSky){
		pSky->NotifyControllerNameChanged(this);
	}
}

void skyeController::SetMinimumValue(float value){
	if(fabsf(value - pMinValue) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	if(value > pMaxValue){
		pMinValue = value;
		pMaxValue = value;
		
	}else{
		pMinValue = value;
	}
	
	pCurValue = pCheckValue(pCurValue);
	
	if(pSky){
		pSky->NotifyControllerChanged(this);
	}
}

void skyeController::SetMaximumValue(float value){
	if(fabsf(value - pMaxValue) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	if(value < pMinValue){
		pMinValue = value;
		pMaxValue = value;
		
	}else{
		pMaxValue = value;
	}
	
	pCurValue = pCheckValue(pCurValue);
	
	if(pSky){
		pSky->NotifyControllerChanged(this);
	}
}

void skyeController::SetCurrentValue(float value){
	if(pFrozen){
		return;
	}
	
	value = pCheckValue(value);
	if(fabsf(value - pCurValue) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pCurValue = value;
	
	if(pSky && pSky->GetEngineSkyInstance()){
		pSky->GetEngineSkyInstance()->GetControllerAt(pIndex).SetCurrentValue(pCurValue);
		pSky->GetEngineSkyInstance()->NotifyControllerChangedAt(pIndex);
	}
	
	if(pSky){
		pSky->NotifyControllerValueChanged(this);
	}
}

void skyeController::SetFrozen(bool frozen){
	if(frozen == pFrozen){
		return;
	}
	
	pFrozen = frozen;
	if(pSky){
		pSky->NotifyControllerChanged(this);
	}
}

void skyeController::SetClamp(bool clamp){
	if(clamp == pClamp){
		return;
	}
	
	pClamp = clamp;
	if(pSky){
		pSky->NotifyControllerChanged(this);
	}
}

void skyeController::SetSelected(bool selected){
	pSelected = selected;
}

void skyeController::SetActive(bool active){
	pActive = active;
}



// Private Functions
//////////////////////

float skyeController::pCheckValue(float value){
	if(pClamp){
		return decMath::clamp(value, pMinValue, pMaxValue);
		
	}else{
		return decMath::normalize(value, pMinValue, pMaxValue);
	}
}
