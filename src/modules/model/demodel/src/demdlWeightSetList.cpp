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

#include "demdlWeightSet.h"
#include "demdlWeightSetList.h"

#include <dragengine/common/exceptions.h>



// Class demdlWeightSetList
/////////////////////////////

// Constructor, destructor
////////////////////////////

demdlWeightSetList::demdlWeightSetList(){
}

demdlWeightSetList::~demdlWeightSetList(){
	RemoveAll();
}



// Management
///////////////

int demdlWeightSetList::GetCount() const{
	return pList.GetCount();
}

demdlWeightSet *demdlWeightSetList::GetAt(int index) const{
	return (demdlWeightSet*)pList.GetAt(index);
}

void demdlWeightSetList::Add(demdlWeightSet *weightSet){
	if(!weightSet){
		DETHROW(deeInvalidParam);
	}
	
	pList.Add(weightSet);
}

void demdlWeightSetList::RemoveAll(){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (demdlWeightSet*)pList.GetAt(i);
	}
	
	pList.RemoveAll();
}

int demdlWeightSetList::IndexOfEqual(const demdlWeightSet &weightSet) const{
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((demdlWeightSet*)pList.GetAt(i))->Equals(weightSet)){
			return i;
		}
	}
	
	return -1;
}

int demdlWeightSetList::GetLargestWeightCount() const{
	const int count = pList.GetCount();
	int weightCount = 0;
	int i;
	
	for(i=0; i<count; i++){
		const demdlWeightSet &weightSet = *((demdlWeightSet*)pList.GetAt(i));
		
		if(weightSet.GetCount() > weightCount){
			weightCount = weightSet.GetCount();
		}
	}
	
	return weightCount;
}
