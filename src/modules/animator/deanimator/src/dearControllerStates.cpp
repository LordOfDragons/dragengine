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

#include "dearControllerStates.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/common/exceptions.h>



// Class dearControllerStates
///////////////////////////////

// Constructor, destructor
////////////////////////////

dearControllerStates::dearControllerStates(){
	pStates = NULL;
	pStateCount = 0;
}

dearControllerStates::~dearControllerStates(){
	if(pStates){
		delete [] pStates;
	}
}



// Management
///////////////

void dearControllerStates::SetStateCount(int count){
	if(count != pStateCount){
		if(pStates){
			delete [] pStates;
			pStates = NULL;
		}
		pStateCount = 0;
		
		if(count > 0){
			pStates = new sState[count];
			if(!pStates) DETHROW(deeOutOfMemory);
		}
		pStateCount = count;
	}
}



float dearControllerStates::GetValueAt(int state) const{
	if(state < 0 || state >= pStateCount) DETHROW(deeInvalidParam);
	
	return pStates[state].value;
}

const decVector &dearControllerStates::GetVectorAt(int state) const{
	if(state < 0 || state >= pStateCount) DETHROW(deeInvalidParam);
	
	return pStates[state].vector;
}

void dearControllerStates::SetValueAt(int state, float value){
	if(state < 0 || state >= pStateCount) DETHROW(deeInvalidParam);
	
	pStates[state].value = value;
}

void dearControllerStates::SetVectorAt(int state, const decVector &vector){
	if(state < 0 || state >= pStateCount) DETHROW(deeInvalidParam);
	
	pStates[state].vector = vector;
}

void dearControllerStates::SetStateAt(int state, float value, const decVector &vector){
	if(state < 0 || state >= pStateCount) DETHROW(deeInvalidParam);
	
	pStates[state].value = value;
	pStates[state].vector = vector;
}

void dearControllerStates::ResetStateAt(int state){
	if(state < 0 || state >= pStateCount) DETHROW(deeInvalidParam);
	
	pStates[state].value = 0.0f;
	pStates[state].vector.SetZero();
}

void dearControllerStates::SetStateFrom(int destState, const dearControllerStates &sourceStates, int sourceState){
	if(sourceState < 0 || sourceState >= sourceStates.pStateCount){
		DETHROW(deeInvalidParam);
	}
	if(destState < 0 || destState >= pStateCount){
		DETHROW(deeInvalidParam);
	}
	
	pStates[destState].value = sourceStates.pStates[sourceState].value;
	pStates[destState].vector = sourceStates.pStates[sourceState].vector;
}
