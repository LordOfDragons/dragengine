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

#include "deParticleEmitterController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deParticleEmitterController
//////////////////////////////////////

// Constructor, Destructor
////////////////////////////

deParticleEmitterController::deParticleEmitterController(const char *name) :
pName(name),
pLower(0.0f),
pUpper(1.0f),
pValue(0.0f),
pClamp(true),
pFrozen(false){
}

deParticleEmitterController::~deParticleEmitterController(){
}



// Management
///////////////

void deParticleEmitterController::SetName(const char *name){
	pName = name;
}

void deParticleEmitterController::SetRange(float lower, float upper){
	if(pFrozen){
		return;
	}
	
	pLower = lower;
	pUpper = upper;
	
	pUpdateValue();
}

void deParticleEmitterController::SetValue(float value){
	if(pFrozen){
		return;
	}
	
	pValue = value;
	
	pUpdateValue();
}

void deParticleEmitterController::Increment(float amount){
	if(pFrozen){
		return;
	}
	
	pValue += amount;
	
	pUpdateValue();
}

void deParticleEmitterController::SetFrozen(bool frozen){
	pFrozen = frozen;
}

void deParticleEmitterController::SetClamp(bool clamp){
	pClamp = clamp;
}



// Operators
//////////////

deParticleEmitterController &deParticleEmitterController::operator=(const deParticleEmitterController &controller){
	pName = controller.pName;
	pLower = controller.pLower;
	pUpper = controller.pUpper;
	pValue = controller.pValue;
	pClamp = controller.pClamp;
	pFrozen = controller.pFrozen;
	return *this;
}



// Private functions
//////////////////////

void deParticleEmitterController::pUpdateValue(){
	const float range = pUpper - pLower;
	const bool hasRange = range > 1e-5f;
	
	// make sure the current value is in the correct range
	if(pClamp){
		if(pValue < pLower){
			pValue = pLower;
			
		}else if(pValue > pUpper){
			pValue = pUpper;
		}
		
	}else{
		if(hasRange){
			pValue = fmodf(pValue - pLower, range);
			if(pValue < 0){
				pValue += pLower + range;
				
			}else{
				pValue += pLower;
			}
			
		}else{
			pValue = pLower;
		}
	}
}
