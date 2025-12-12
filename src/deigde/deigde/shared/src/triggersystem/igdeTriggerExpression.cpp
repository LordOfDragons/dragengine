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

#include "igdeTriggerTarget.h"
#include "igdeTriggerTargetList.h"
#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionComponent.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerExpression
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpression::igdeTriggerExpression(){
	pResult = false;
	pEnabled = true;
}

igdeTriggerExpression::~igdeTriggerExpression(){
	UnlinkTriggerTargets();
}



// Management
///////////////

void igdeTriggerExpression::SetRootComponent(igdeTriggerExpressionComponent *component){
	if(component == pRootComponent){
		return;
	}
	
	pRootComponent = component;
	pResult = false;
}

bool igdeTriggerExpression::IsEmpty() const{
	return pRootComponent.IsNull();
}

bool igdeTriggerExpression::IsNotEmpty() const{
	return pRootComponent.IsNotNull();
}

void igdeTriggerExpression::SetResult(bool result){
	pResult = result;
}

void igdeTriggerExpression::SetEnabled(bool enabled){
	pEnabled = enabled;
}



void igdeTriggerExpression::LinkTriggerTargets(igdeTriggerTargetList &triggerTable, igdeTriggerListener *listener){
	if(pRootComponent){
		pRootComponent->LinkTargets(triggerTable, listener);
		Evaluate();
	}
}

void igdeTriggerExpression::UnlinkTriggerTargets(){
	if(pRootComponent){
		pRootComponent->UnlinkTargets();
	}
}

bool igdeTriggerExpression::Evaluate(){
	if(pEnabled && pRootComponent){
		const bool result = pRootComponent->Evaluate();
		
		if(result != pResult){
			pResult = result;
			return true;
		}
	}
	
	return false;
}
