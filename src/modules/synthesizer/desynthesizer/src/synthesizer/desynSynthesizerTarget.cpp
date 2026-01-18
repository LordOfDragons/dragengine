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

#include "desynSynthesizerTarget.h"
#include "desynSynthesizer.h"
#include "desynSynthesizerInstance.h"
#include "desynSynthesizerLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/deSynthesizerControllerTarget.h>



// Class desynSynthesizerTarget
/////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerTarget::desynSynthesizerTarget(const desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerControllerTarget &target)
{
	target.GetLinks().Visit([&](int link){
		pLinks.Add(&synthesizer.GetLinkAt(firstLink + link));
	});
}

desynSynthesizerTarget::~desynSynthesizerTarget(){
}



// Management
///////////////

const desynSynthesizerLink &desynSynthesizerTarget::GetLinkAt(int index) const{
	return *pLinks.GetAt(index);
}



float desynSynthesizerTarget::GetValue(const desynSynthesizerInstance &instance, int sample, float defaultValue) const{
	if(pLinks.IsEmpty()){
		return defaultValue;
	}
	
	float value = defaultValue;
	bool firstValue = true;
	
	pLinks.Visit([&](const desynSynthesizerLink *link){
		if(!link->HasController()){
			return;
		}
		
		if(firstValue){
			value = link->GetValue(instance, sample, 1.0f);
			firstValue = false;
			
		}else{
			value *= link->GetValue(instance, sample, 1.0f);
		}
	});
	
	return decMath::clamp(value, 0.0f, 1.0f);
}
