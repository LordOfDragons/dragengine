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

#include "deoglRComponent.h"
#include "deoglComponentSet.h"

#include <dragengine/common/exceptions.h>



// Class deoglComponentSet
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponentSet::deoglComponentSet(){
	pComponents = NULL;
	pComponentCount = 0;
	pComponentSize = 0;
}

deoglComponentSet::~deoglComponentSet(){
	if(pComponents){
		delete [] pComponents;
	}
}



// Management
///////////////

deoglRComponent *deoglComponentSet::GetAt(int index) const{
	if(index < 0 || index >= pComponentCount){
		DETHROW(deeInvalidParam);
	}
	
	return pComponents[index];
}

int deoglComponentSet::IndexOfComponent(deoglRComponent *component) const{
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pComponentCount; i++){
		if(component == pComponents[i]){
			return i;
		}
	}
	
	return -1;
}

bool deoglComponentSet::Has(deoglRComponent *component) const{
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pComponentCount; i++){
		if(component == pComponents[i]){
			return true;
		}
	}
	
	return false;
}

void deoglComponentSet::Add(deoglRComponent *component){
	if(Has(component)){
		DETHROW(deeInvalidParam);
	}
	
	pAddComponent(component);
}

bool deoglComponentSet::AddIfMissing(deoglRComponent *component){
	if(Has(component)){
		return false;
	}
	
	pAddComponent(component);
	return true;
}

void deoglComponentSet::Remove(deoglRComponent *component){
	const int index = IndexOfComponent(component);
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(index < pComponentCount - 1){
		pComponents[index] = pComponents[pComponentCount - 1];
	}
	pComponentCount--;
}

bool deoglComponentSet::RemoveIfExisting(deoglRComponent *component){
	const int index = IndexOfComponent(component);
	
	if(index == -1){
		return false;
	}
	
	if(index < pComponentCount - 1){
		pComponents[index] = pComponents[pComponentCount - 1];
	}
	pComponentCount--;
	return true;
}

void deoglComponentSet::RemoveFrom(int index){
	if(index < 0 || index >= pComponentCount){
		DETHROW(deeInvalidParam);
	}
	
	if(index < pComponentCount - 1){
		pComponents[index] = pComponents[pComponentCount - 1];
	}
	pComponentCount--;
}

void deoglComponentSet::RemoveAll(){
	pComponentCount = 0;
}



void deoglComponentSet::MarkAll(bool mark){
	int i;
	
	for(i=0; i<pComponentCount; i++){
		pComponents[i]->SetMarked(mark);
	}
}

void deoglComponentSet::RemoveAllMarked(bool mark){
	int last = 0;
	int i;
	
	for(i=0; i<pComponentCount; i++){
		if(pComponents[i]->GetMarked() != mark){
			if(i != last){
				deoglRComponent * const swap = pComponents[last];
				pComponents[last] = pComponents[i];
				pComponents[i] = swap;
			}
			last++;
		}
	}
	pComponentCount = last;
}



// Private Functions
//////////////////////

void deoglComponentSet::pAddComponent(deoglRComponent *component){
	if(pComponentCount == pComponentSize){
		int newSize = pComponentCount + 10; // * 3 / 2 + 1;
		deoglRComponent **newArray = new deoglRComponent*[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		
		if(pComponents){
			memcpy(newArray, pComponents, sizeof(deoglRComponent*) * pComponentSize);
			delete [] pComponents;
		}
		
		pComponents = newArray;
		pComponentSize = newSize;
	}
	
	pComponents[pComponentCount++] = component;
}
