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

#include "desynSynthesizerLink.h"
#include "desynSynthesizerInstance.h"
#include "desynSynthesizerController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>



// Class desynSynthesizerLink
///////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerLink::desynSynthesizerLink(const deSynthesizerLink &link) :
pController(link.GetController()),
pRepeat(link.GetRepeat()),
pCurve(link.GetCurve()){
}

desynSynthesizerLink::desynSynthesizerLink(const deSynthesizerLink &link, const decIntList &controllerMapping) :
pController(link.GetController()),
pRepeat(link.GetRepeat()),
pCurve(link.GetCurve())
{
	if(pController == -1){
		return;
	}
	
	if(pController < controllerMapping.GetCount()){
		pController = controllerMapping.GetAt(pController);
		
	}else{
		pController = -1;
	}
}

desynSynthesizerLink::~desynSynthesizerLink(){
}



// Management
///////////////

float desynSynthesizerLink::GetValue(const desynSynthesizerInstance &instance, int sample, float defaultValue) const{
	if(pController == -1){
		return defaultValue;
	}
	
	float value = instance.GetControllerAt(pController).GetValue(sample);
	
	if(pRepeat > 1){
		value *= (float)pRepeat;
		value -= floorf(value);
	}
	
	return pCurve.Evaluate(value);
}
