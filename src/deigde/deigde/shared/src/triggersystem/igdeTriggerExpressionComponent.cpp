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
#include "igdeTriggerExpressionComponent.h"
#include "igdeTriggerListener.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerExpressionComponent
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionComponent::igdeTriggerExpressionComponent() :
pNegate(false),
pCurState(false),
pType(ectTarget){
}

igdeTriggerExpressionComponent::~igdeTriggerExpressionComponent(){
	if(pTarget && pTargetListener){
		pTarget->RemoveListener(pTargetListener);
	}
}



// Management
///////////////

void igdeTriggerExpressionComponent::SetNegate(bool negate){
	pNegate = negate;
}

void igdeTriggerExpressionComponent::SetCurState(bool curState){
	pCurState = curState;
}

void igdeTriggerExpressionComponent::SetType (eComponentTypes type){
	if(type < ectTarget || type > ectOr){
		DETHROW(deeInvalidParam);
	}
	pType = type;
}

void igdeTriggerExpressionComponent::SetTargetName(const char *name){
	pTargetName = name;
}

void igdeTriggerExpressionComponent::SetTarget(igdeTriggerTarget *target){
	pTarget = target;
}

void igdeTriggerExpressionComponent::SetTargetListener(igdeTriggerListener *listener){
	pTargetListener = listener;
}

void igdeTriggerExpressionComponent::LinkTargets(igdeTriggerTargetList &triggerTable, igdeTriggerListener *listener){
	if(pTargetListener){
		pTarget->RemoveListener(pTargetListener);
	}
	
	SetTarget(nullptr);
	SetTargetListener(nullptr);
	
	if(pType == ectTarget && !pTargetName.IsEmpty()){
		SetTarget(triggerTable.GetNamedAddIfMissing(pTargetName.GetString()));
		
		if(listener){
			pTarget->AddListener(listener);
			SetTargetListener(listener);
		}
	}
	
	const int count = pChildred.GetCount();
	int i;
	for(i=0; i<count; i++){
		static_cast<igdeTriggerExpressionComponent*>(pChildred.GetAt(i))->
			LinkTargets(triggerTable, listener);
	}
}

void igdeTriggerExpressionComponent::UnlinkTargets(){
	if(pTargetListener){
		pTarget->RemoveListener(pTargetListener);
	}
	
	SetTarget(nullptr);
	SetTargetListener(nullptr);
	
	const int count = pChildred.GetCount();
	int i;
	for(i=0; i<count; i++){
		static_cast<igdeTriggerExpressionComponent*>(pChildred.GetAt(i))->UnlinkTargets();
	}
}

bool igdeTriggerExpressionComponent::Evaluate(){
	bool result = false;
	
	switch(pType){
	case ectTarget:
		if(pTarget){
			if(pCurState){
				result = pTarget->GetFired();
				
			}else{
				result = pTarget->GetHasFired();
			}
			
		}else{
			return false; // in the case the trigger is not correctly configurated
		}
		break;
		
	case ectAnd:{
		const int count = pChildred.GetCount();
		int i;
		for(i=0; i<count; i++){
			if(!static_cast<igdeTriggerExpressionComponent*>(pChildred.GetAt(i))->Evaluate()){
				break;
			}
		}
		result = count > 0 && i == count;
		}break;
		
	case ectOr:{
		const int count = pChildred.GetCount();
		int i;
		for(i=0; i<count; i++){
			if(static_cast<igdeTriggerExpressionComponent*>(pChildred.GetAt(i))->Evaluate()){
				break;
			}
		}
		result = i < count;
		}break;
		
	default:
		break;
	}
	
	return result ^ pNegate;
}



// Children
/////////////

int igdeTriggerExpressionComponent::GetChildCount() const{
	return pChildred.GetCount();
}

igdeTriggerExpressionComponent *igdeTriggerExpressionComponent::GetChildAt(int index) const{
	return (igdeTriggerExpressionComponent*)pChildred.GetAt(index);
}

int igdeTriggerExpressionComponent::IndexOfChild(igdeTriggerExpressionComponent *child) const{
	return pChildred.IndexOf(child);
}

void igdeTriggerExpressionComponent::AddChild(igdeTriggerExpressionComponent *child){
	if(!child){
		DETHROW(deeInvalidParam);
	}
	pChildred.Add(child);
}

void igdeTriggerExpressionComponent::InsertChild(igdeTriggerExpressionComponent *child, int index){
	if(!child){
		DETHROW(deeInvalidParam);
	}
	pChildred.Insert(child, index);
}

void igdeTriggerExpressionComponent::RemoveChild(igdeTriggerExpressionComponent *child){
	pChildred.Remove(child);
}

void igdeTriggerExpressionComponent::RemoveAllChildren(){
	pChildred.RemoveAll();
}
