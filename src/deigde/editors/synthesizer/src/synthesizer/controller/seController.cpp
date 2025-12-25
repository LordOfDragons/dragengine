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

#include "seController.h"
#include "../seSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>



// Class seController
///////////////////////

// Constructor, destructor
////////////////////////////

seController::seController(const char *name) :
pSynthesizer(nullptr),
pEngControllerIndex(-1),

pName(name),

pMinValue(0.0f),
pMaxValue(1.0f),
pClamp(true),

pEditConstantValue(0.0f){
}

seController::seController(const seController &copy) :
pSynthesizer(nullptr),
pEngControllerIndex(-1),

pName(copy.pName),

pMinValue(copy.pMinValue),
pMaxValue(copy.pMaxValue),
pClamp(copy.pClamp),

pCurve(copy.pCurve),

pEditConstantValue(copy.pEditConstantValue){
}

seController::~seController(){
	pCleanUp();
}



// Management
///////////////

void seController::SetSynthesizer(seSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	pSynthesizer = synthesizer;
	pEngControllerIndex = -1;
}

void seController::SetEngineControllerIndex(int index){
	if(index == pEngControllerIndex){
		return;
	}
	
	pEngControllerIndex = index;
	if(index == -1){
		return;
	}
	
	deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
	deSynthesizerController &controller = *instance.GetControllerAt(index);
	
	controller.SetValueRange(pMinValue, pMaxValue);
	controller.SetClamp(pClamp);
	controller.SetCurve(pCurve);
	
	instance.NotifyControllerChangedAt(index);
}



void seController::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	
	if(pSynthesizer){
		pSynthesizer->NotifyControllerNameChanged(this);
	}
}



void seController::SetMinimumValue(float value){
	if(fabsf(value - pMinValue) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	if(value > pMaxValue){
		pMinValue = value;
		pMaxValue = value;
		
	}else{
		pMinValue = value;
	}
	
	if(pEngControllerIndex != -1){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt(pEngControllerIndex);
		
		controller.SetValueRange(pMinValue, pMaxValue);
		
		instance.NotifyControllerChangedAt(pEngControllerIndex);
	}
	
	if(pSynthesizer){
		pSynthesizer->NotifyControllerRangeChanged(this);
	}
}

void seController::SetMaximumValue(float value){
	if(fabsf(value - pMaxValue) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	if(value < pMinValue){
		pMinValue = value;
		pMaxValue = value;
		
	}else{
		pMaxValue = value;
	}
	
	if(pEngControllerIndex != -1){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt(pEngControllerIndex);
		
		controller.SetValueRange(pMinValue, pMaxValue);
		
		instance.NotifyControllerChangedAt(pEngControllerIndex);
	}
	
	if(pSynthesizer){
		pSynthesizer->NotifyControllerRangeChanged(this);
	}
}

void seController::SetClamp(bool clamp){
	if(clamp == pClamp){
		return;
	}
	
	pClamp = clamp;
	
	if(pEngControllerIndex != -1){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt(pEngControllerIndex);
		
		controller.SetClamp(clamp);
		
		instance.NotifyControllerChangedAt(pEngControllerIndex);
	}
	
	if(pSynthesizer){
		pSynthesizer->NotifyControllerChanged(this);
	}
}

void seController::SetCurve(const decCurveBezier &curve){
	if(curve == pCurve){
		return;
	}
	
	pCurve = curve;
	
	if(pEngControllerIndex != -1){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt(pEngControllerIndex);
		
		controller.SetCurve(curve);
		
		instance.NotifyControllerChangedAt(pEngControllerIndex);
	}
	
	if(pSynthesizer){
		pSynthesizer->NotifyControllerCurveChanged(this);
	}
}

void seController::SetEditConstantValue(float value){
	pEditConstantValue = value;
}



// Operators
//////////////

seController &seController::operator=(const seController &controller){
	SetName(controller.pName);
	SetMinimumValue(controller.pMinValue);
	SetMaximumValue(controller.pMaxValue);
	SetClamp(controller.pClamp);
	SetCurve(controller.pCurve);
	SetEditConstantValue(controller.pEditConstantValue);
	return *this;
}



// Private Functions
//////////////////////

void seController::pCleanUp(){
	SetSynthesizer(nullptr);
}
