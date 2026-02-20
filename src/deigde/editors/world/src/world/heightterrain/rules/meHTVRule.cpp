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

#include "meHTVRule.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRule
////////////////////

// Constructor, destructor
////////////////////////////

meHTVRule::meHTVRule(int type, int slotCount){
	if(slotCount < 0 || type < 0 || type >= ERT_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	pType = type;
	pShowParameters = true;
	
	for(int i=0; i<slotCount; i++){
		pSlots.Add(meHTVRSlot::Ref::New());
	}
}

meHTVRule::meHTVRule(const meHTVRule &rule) :
pName(rule.pName),
pType(rule.pType),
pPosition(rule.pPosition),
pShowParameters(rule.pShowParameters)
{
	rule.pSlots.Visit([&](const meHTVRSlot &s){
		const meHTVRSlot::Ref newSlot = meHTVRSlot::Ref::New();
		newSlot->SetIsInput(s.GetIsInput());
		pSlots.Add(newSlot);
	});
}

meHTVRule::~meHTVRule(){
}



// Management
///////////////

void meHTVRule::SetName(const char *name){
	pName = name;
}

void meHTVRule::SetPosition(const decVector2 &position){
	pPosition = position;
}

void meHTVRule::SetShowParameters(bool showParameters){
	pShowParameters = showParameters;
}

bool meHTVRule::DependsOn(meHTVRule *rule) const{
	DEASSERT_NOTNULL(rule)
	
	// by definition we depend on ourself
	if(rule == this){
		return true;
	}
	
	// otherwise test all input slot nodes for dependance
	return pSlots.HasMatching([&](const meHTVRSlot &slot){
		return slot.GetIsInput() && slot.GetLinks().HasMatching([&](const meHTVRLink &link){
			return link.GetSourceRule()->DependsOn(rule);
		});
	});
}



void meHTVRule::Reset(){
}

void meHTVRule::Evaluate(meHTVEvaluationEnvironment&){
}

float meHTVRule::GetOutputSlotValueAt(int, meHTVEvaluationEnvironment&){
	DETHROW(deeInvalidParam);
}

decVector meHTVRule::GetOutputSlotVectorAt(int, meHTVEvaluationEnvironment&){
	DETHROW(deeInvalidParam);
}
