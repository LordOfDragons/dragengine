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

#include "aeRuleStateSnapshot.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleStateSnapshot
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleStateSnapshot::aeRuleStateSnapshot() :
aeRule(deAnimatorRuleVisitorIdentify::ertStateSnapshot),
pUseLastState(true),
pID(0),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true)
{
	SetName("State Snapshot");
}

aeRuleStateSnapshot::aeRuleStateSnapshot(const aeRuleStateSnapshot &copy) :
aeRule(copy),
pUseLastState(copy.pUseLastState),
pID(copy.pID),
pEnablePosition(copy.pEnablePosition),
pEnableOrientation(copy.pEnableOrientation),
pEnableSize(copy.pEnableSize),
pEnableVertexPositionSet(copy.pEnableVertexPositionSet){
}

aeRuleStateSnapshot::~aeRuleStateSnapshot(){
}



// Management
///////////////

void aeRuleStateSnapshot::SetUseLastState(bool useLastState){
	if(useLastState == pUseLastState){
		return;
	}
	
	pUseLastState = useLastState;
	
	if(GetEngineRule()){
		((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetUseLastState(useLastState);
		NotifyRuleChanged();
	}
}

void aeRuleStateSnapshot::SetID(int id){
	if(id == pID){
		return;
	}
	
	pID = id;
	
	if(GetEngineRule()){
		((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetID(id);
		NotifyRuleChanged();
	}
}

void aeRuleStateSnapshot::SetEnablePosition(bool enabled){
	if(enabled != pEnablePosition){
		pEnablePosition = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnablePosition(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleStateSnapshot::SetEnableOrientation(bool enabled){
	if(enabled != pEnableOrientation){
		pEnableOrientation = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableOrientation(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleStateSnapshot::SetEnableSize(bool enabled){
	if(enabled != pEnableSize){
		pEnableSize = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableSize(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleStateSnapshot::SetEnableVertexPositionSet(bool enabled){
	if(enabled != pEnableVertexPositionSet){
		pEnableVertexPositionSet = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableVertexPositionSet(enabled);
			NotifyRuleChanged();
		}
	}
}



deAnimatorRule *aeRuleStateSnapshot::CreateEngineRule(){
	deAnimatorRuleStateSnapshot::Ref engRule = NULL;
	
	try{
		engRule.TakeOver(new deAnimatorRuleStateSnapshot);
		
		InitEngineRule(engRule);
		
		engRule->SetUseLastState(pUseLastState);
		engRule->SetID(pID);
		
		engRule->SetEnablePosition(pEnablePosition);
		engRule->SetEnableOrientation(pEnableOrientation);
		engRule->SetEnableSize(pEnableSize);
		engRule->SetEnableVertexPositionSet(pEnableVertexPositionSet);
		
	}catch(const deException &){
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleStateSnapshot::CreateCopy() const{
	return new aeRuleStateSnapshot(*this);
}

void aeRuleStateSnapshot::ListLinks(aeLinkList &list){
	aeRule::ListLinks(list);
}



// Operators
///////////////

aeRuleStateSnapshot &aeRuleStateSnapshot::operator=(const aeRuleStateSnapshot &copy){
	SetUseLastState(copy.pUseLastState);
	SetID(copy.pID);
	SetEnablePosition(copy.pEnablePosition);
	SetEnableOrientation(copy.pEnableOrientation);
	SetEnableSize(copy.pEnableSize);
	SetEnableVertexPositionSet(copy.pEnableVertexPositionSet);
	aeRule::operator=(copy);
	return *this;
}
