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

#include "meHTVRuleConstant.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleConstant
////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleConstant::meHTVRuleConstant() : meHTVRule(ertConstant, 4){
	GetSlotAt(eosVector).SetIsInput(false);
	GetSlotAt(eosX).SetIsInput(false);
	GetSlotAt(eosY).SetIsInput(false);
	GetSlotAt(eosZ).SetIsInput(false);
}

meHTVRuleConstant::meHTVRuleConstant(const meHTVRuleConstant &rule) :
meHTVRule(rule),
pVector(rule.pVector){
}

meHTVRuleConstant::~meHTVRuleConstant(){
}



// Management
///////////////

void meHTVRuleConstant::SetVector(const decVector &vector){
	pVector = vector;
}



float meHTVRuleConstant::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 3) DETHROW(deeInvalidParam);
	
	if(slot == eosVector){ // incorrect usage, use x value
		return pVector.x;
		
	}else if(slot == eosX){
		return pVector.x;
		
	}else if(slot == eosY){
		return pVector.y;
		
	}else{ // slot == eosZ
		return pVector.z;
	}
}

decVector meHTVRuleConstant::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 3) DETHROW(deeInvalidParam);
	
	if(slot == eosVector){
		return pVector;
		
	}else if(slot == eosX){
		return decVector(pVector.x, pVector.x, pVector.x);
		
	}else if(slot == eosY){
		return decVector(pVector.y, pVector.y, pVector.y);
		
	}else{ // slot == eosZ
		return decVector(pVector.z, pVector.z, pVector.z);
	}
}

meHTVRule *meHTVRuleConstant::Copy() const{
	return new meHTVRuleConstant(*this);
}
