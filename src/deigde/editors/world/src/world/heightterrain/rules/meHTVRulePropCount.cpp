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

#include "meHTVRulePropCount.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../../meWorld.h"
#include "../../object/meObject.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRulePropCount
///////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRulePropCount::meHTVRulePropCount() : meHTVRule(ertPropCount, 1){
	pSearchRadius = 1.0f;
	
	GetSlotAt(eosCount).SetIsInput(false);
	
	Reset();
}

meHTVRulePropCount::meHTVRulePropCount(const meHTVRulePropCount &rule) :
meHTVRule(rule),
pPropClass(rule.pPropClass),
pSearchRadius(rule.pSearchRadius),
pCount(rule.pCount),
pDirty(false){
}

meHTVRulePropCount::~meHTVRulePropCount(){
}



// Management
///////////////

void meHTVRulePropCount::SetPropClass(const char *propClass){
	if(! propClass) DETHROW(deeInvalidParam);
	
	pPropClass = propClass;
	
	Reset();
}

void meHTVRulePropCount::SetSearchRadius(float searchRadius){
	pSearchRadius = searchRadius;
	
	Reset();
}

void meHTVRulePropCount::UpdateResult(meHTVEvaluationEnvironment &evalEnv){
	if(pDirty){
		const decDVector &ipos = evalEnv.GetPosition();
		int o, objectCount = evalEnv.GetObjectCount();
		meObject *object;
		int count = 0;
		
		for(o=0; o<objectCount; o++){
			object = evalEnv.GetObjectAt(o);
			
			if(pPropClass.Equals(object->GetClassName())){
				if((float)(object->GetPosition() - ipos).Length() <= pSearchRadius){
					count++;
				}
			}
		}
		
		pCount = (float)count;
		
		// no more dirty
		pDirty = false;
	}
}



void meHTVRulePropCount::Reset(){
	pCount = 0.0f;
	pDirty = true;
}

float meHTVRulePropCount::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != 0) DETHROW(deeInvalidParam);
	
	UpdateResult(evalEnv);
	
	return pCount;
}

decVector meHTVRulePropCount::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != 0) DETHROW(deeInvalidParam);
	
	UpdateResult(evalEnv);
	
	return decVector(pCount, pCount, pCount);
}

meHTVRule *meHTVRulePropCount::Copy() const{
	return new meHTVRulePropCount(*this);
}
