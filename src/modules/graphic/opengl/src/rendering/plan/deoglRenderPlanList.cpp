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
#include <string.h>

#include "deoglRenderPlan.h"
#include "deoglRenderPlanList.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderPlanList
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanList::deoglRenderPlanList(){
	pPlans = NULL;
	pPlanCount = 0;
	pPlanSize = 0;
}

deoglRenderPlanList::~deoglRenderPlanList(){
	if(pPlans){
		delete [] pPlans;
	}
}



// Management
///////////////

deoglRenderPlan *deoglRenderPlanList::GetAt(int index) const{
	if(index < 0 || index >= pPlanCount){
		DETHROW(deeInvalidParam);
	}
	
	return pPlans[index];
}

int deoglRenderPlanList::IndexOf(deoglRenderPlan *plan) const{
	if(! plan){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pPlanCount; i++){
		if(plan == pPlans[i]){
			return i;
		}
	}
	
	return -1;
}

bool deoglRenderPlanList::Has(deoglRenderPlan *plan) const{
	if(! plan){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pPlanCount; i++){
		if(plan == pPlans[i]){
			return true;
		}
	}
	
	return false;
}

void deoglRenderPlanList::Add(deoglRenderPlan *plan){
	if(Has(plan)){
		DETHROW(deeInvalidParam);
	}
	
	pAdd(plan);
}

bool deoglRenderPlanList::AddIfMissing(deoglRenderPlan *plan){
	if(Has(plan)){
		return false;
	}
	
	pAdd(plan);
	
	return true;
}

void deoglRenderPlanList::Remove(deoglRenderPlan *plan){
	const int index = IndexOf(plan);
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(index < pPlanCount - 1){
		pPlans[index] = pPlans[pPlanCount - 1];
	}
	pPlanCount--;
}

bool deoglRenderPlanList::RemoveIfExisting(deoglRenderPlan *plan){
	const int index = IndexOf(plan);
	
	if(index == -1){
		return false;
	}
	
	if(index < pPlanCount - 1){
		pPlans[index] = pPlans[pPlanCount - 1];
	}
	pPlanCount--;
	
	return true;
}

void deoglRenderPlanList::RemoveFrom(int index){
	if(index < 0 || index >= pPlanCount){
		DETHROW(deeInvalidParam);
	}
	
	if(index < pPlanCount - 1){
		pPlans[index] = pPlans[pPlanCount - 1];
	}
	pPlanCount--;
}

void deoglRenderPlanList::RemoveAll(){
	pPlanCount = 0;
}



// Private Functions
//////////////////////

void deoglRenderPlanList::pAdd(deoglRenderPlan *plan){
	if(pPlanCount == pPlanSize){
		const int newSize = pPlanCount + 10; // * 3 / 2 + 1;
		deoglRenderPlan ** const newArray = new deoglRenderPlan*[newSize];
		
		if(pPlans){
			memcpy(newArray, pPlans, sizeof(deoglRenderPlan*) * pPlanSize);
			delete [] pPlans;
		}
		
		pPlans = newArray;
		pPlanSize = newSize;
	}
	
	pPlans[pPlanCount++] = plan;
}
