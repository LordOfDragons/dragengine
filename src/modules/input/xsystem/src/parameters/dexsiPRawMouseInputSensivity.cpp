/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "dexsiPRawMouseInputSensivity.h"
#include "../deXSystemInput.h"

#include <dragengine/common/exceptions.h>


// Class dexsiPRawMouseInputSensivity
///////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiPRawMouseInputSensivity::dexsiPRawMouseInputSensivity(deXSystemInput &xsi) : dexsiParameterFloat(xsi),
pMinVal(-20.0f),
pMaxVal(20.0f),
pMinSensivity(0.25f),
pMaxSensivity(4.0f)
{
	pParameter.SetName("rawMouseInputSensivity");
	pParameter.SetDescription(
		"Raw mouse input sensivity. Value of 0 uses direct device values. "
		" Value of 20 increases speed to 400%. Value of -20 decreases speed to 25%.");
	pParameter.SetType(deModuleParameter::eptRanged);
	pParameter.SetCategory(deModuleParameter::ecBasic);
	pParameter.SetDisplayName("Raw Mouse Input Sensivity");
	pParameter.SetMinimumValue(pMinVal);
	pParameter.SetMaximumValue(pMaxVal);
	pParameter.SetValueStepSize(1.0f);
	pParameter.SetDefaultValue("0");
}


// Parameter Value
////////////////////

float dexsiPRawMouseInputSensivity::GetParameterFloat(){
	const float sensivity = (float)pModule.GetRawMouseInputSensitivity();
	if(sensivity > 1.0f){
		return decMath::linearStep(sensivity, 1.0f, pMaxSensivity, 0.0f, pMaxVal);
		
	}else{
		return decMath::linearStep(sensivity, pMinSensivity, 1.0f, pMinVal, 0.0f);
	}
}

void dexsiPRawMouseInputSensivity::SetParameterFloat(float value){
	if(value > 0.0f){
		pModule.SetRawMouseInputSensitivity((double)decMath::linearStep(value, 0.0f, pMaxVal, 1.0f, pMaxSensivity));
		
	}else{
		pModule.SetRawMouseInputSensitivity((double)decMath::linearStep(value, pMinVal, 0.0f, pMinSensivity, 1.0f));
	}
}
