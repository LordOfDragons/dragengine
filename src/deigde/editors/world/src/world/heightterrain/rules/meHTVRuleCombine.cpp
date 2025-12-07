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

#include "meHTVRuleCombine.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleCombine
///////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleCombine::meHTVRuleCombine() : meHTVRule(ertCombine, 4){
	pX = 0.0f;
	pY = 0.0f;
	pZ = 0.0f;
	
	GetSlotAt(eisX).SetIsInput(true);
	GetSlotAt(eisY).SetIsInput(true);
	GetSlotAt(eisZ).SetIsInput(true);
	
	GetSlotAt(eosVector).SetIsInput(false);
}

meHTVRuleCombine::meHTVRuleCombine(const meHTVRuleCombine &rule) :
meHTVRule(rule),
pX(rule.pX),
pY(rule.pY),
pZ(rule.pZ){
}

meHTVRuleCombine::~meHTVRuleCombine(){
}



// Management
///////////////

void meHTVRuleCombine::SetX(float x){
	pX = x;
}

void meHTVRuleCombine::SetY(float y){
	pY = y;
}

void meHTVRuleCombine::SetZ(float z){
	pZ = z;
}



float meHTVRuleCombine::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != eosVector) DETHROW(deeInvalidParam);
	
	// Incorrect usage, we yield the x component in this case
	meHTVRSlot &inputX = GetSlotAt(eisX);
	
	if(inputX.GetLinkCount() > 0){
		meHTVRLink &link = *inputX.GetLinkAt(0);
		
		return link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
		
	}else{
		return 0.0f;
	}
}

decVector meHTVRuleCombine::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != eosVector) DETHROW(deeInvalidParam);
	
	meHTVRSlot &inputX = GetSlotAt(eisX);
	meHTVRSlot &inputY = GetSlotAt(eisY);
	meHTVRSlot &inputZ = GetSlotAt(eisZ);
	
	decVector vector(pX, pY, pZ);
	
	if(inputX.GetLinkCount() > 0){
		meHTVRLink &link = *inputX.GetLinkAt(0);
		
		vector.x = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(inputY.GetLinkCount() > 0){
		meHTVRLink &link = *inputY.GetLinkAt(0);
		
		vector.y = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(inputZ.GetLinkCount() > 0){
		meHTVRLink &link = *inputZ.GetLinkAt(0);
		
		vector.z = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	return vector;
}

meHTVRule *meHTVRuleCombine::Copy() const{
	return new meHTVRuleCombine(*this);
}
