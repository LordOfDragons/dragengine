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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSynthesizerController.h"

#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deSynthesizerController
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerController::deSynthesizerController() :
pMinValue(0.0f),
pMaxValue(1.0f),
pClamp(true){
}

deSynthesizerController::deSynthesizerController(const deSynthesizerController &controller) :
pName(controller.pName),
pMinValue(controller.pMinValue),
pMaxValue(controller.pMaxValue),
pClamp(controller.pClamp),
pCurve(controller.pCurve){
}

deSynthesizerController::~deSynthesizerController(){
}



// Management
///////////////

void deSynthesizerController::SetName(const char *name){
	pName = name;
}

void deSynthesizerController::SetValueRange(float minValue, float maxValue){
	pMinValue = minValue;
	pMaxValue = decMath::max(maxValue, minValue);
}

void deSynthesizerController::SetClamp(bool clamp){
	pClamp = clamp;
}

void deSynthesizerController::SetCurve(const decCurveBezier &curve){
	pCurve = curve;
}



// Operators
//////////////

deSynthesizerController &deSynthesizerController::operator=(const deSynthesizerController &controller){
	pName = controller.pName;
	pMinValue = controller.pMinValue;
	pMaxValue = controller.pMaxValue;
	pCurve = controller.pCurve;
	pClamp = controller.pClamp;
	return *this;
}
