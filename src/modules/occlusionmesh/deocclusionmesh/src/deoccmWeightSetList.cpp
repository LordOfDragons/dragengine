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

#include "deoccmWeightSet.h"
#include "deoccmWeightSetList.h"

#include <dragengine/common/exceptions.h>



// Class deoccmWeightSetList
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoccmWeightSetList::deoccmWeightSetList(){
}

deoccmWeightSetList::~deoccmWeightSetList(){
	RemoveAll();
}



// Management
///////////////

int deoccmWeightSetList::GetCount() const{
	return pList.GetCount();
}

deoccmWeightSet *deoccmWeightSetList::GetAt(int index) const{
	return (deoccmWeightSet*)pList.GetAt(index);
}

void deoccmWeightSetList::Add(deoccmWeightSet *weightSet){
	if(!weightSet){
		DETHROW(deeInvalidParam);
	}
	
	pList.Add(weightSet);
}

void deoccmWeightSetList::RemoveAll(){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (deoccmWeightSet*)pList.GetAt(i);
	}
	
	pList.RemoveAll();
}

int deoccmWeightSetList::IndexOfEqual(const deoccmWeightSet &weightSet) const{
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((deoccmWeightSet*)pList.GetAt(i))->Equals(weightSet)){
			return i;
		}
	}
	
	return -1;
}

int deoccmWeightSetList::GetLargestWeightCount() const{
	const int count = pList.GetCount();
	int weightCount = 0;
	int i;
	
	for(i=0; i<count; i++){
		const deoccmWeightSet &weightSet = *((deoccmWeightSet*)pList.GetAt(i));
		
		if(weightSet.GetCount() > weightCount){
			weightCount = weightSet.GetCount();
		}
	}
	
	return weightCount;
}
