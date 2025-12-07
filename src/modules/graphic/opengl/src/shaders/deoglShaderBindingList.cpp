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

#include "deoglShaderBindingList.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderBindingList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderBindingList::deoglShaderBindingList() :
pBindings(nullptr),
pCount(0),
pSize(0){
}

deoglShaderBindingList::~deoglShaderBindingList(){
	RemoveAll();
	if(pBindings){
		delete [] pBindings;
	}
}



// Management
///////////////

bool deoglShaderBindingList::HasNamed(const char *name){
	int i;
	for(i=0; i<pCount; i++){
		if(pBindings[i].name == name){
			return true;
		}
	}
	
	return false;
}

int deoglShaderBindingList::IndexOfNamed(const char *name) const{
	int i;
	for(i=0; i<pCount; i++){
		if(pBindings[i].name == name){
			return i;
		}
	}
	
	return -1;
}

const decString &deoglShaderBindingList::GetNameAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pCount)
	
	return pBindings[index].name;
}

int deoglShaderBindingList::GetTargetAt(int index) const{
	if(index < 0 || index >= pCount){
		DETHROW(deeInvalidParam);
	}
	
	return pBindings[index].target;
}

void deoglShaderBindingList::Add(const char *name, int target){
	DEASSERT_FALSE(HasNamed(name))
	
	if(pCount == pSize){
		const int newSize = pSize * 3 / 2 + 1;
		sBinding * const newArray = new sBinding[newSize];
		if(pBindings){
			int i;
			for(i=0; i<pSize; i++){
				newArray[i] = pBindings[i];
			}
			delete [] pBindings;
		}
		pBindings = newArray;
		pSize = newSize;
	}
	
	pBindings[pCount].name = name;
	pBindings[pCount].target = target;
	pCount++;
}

void deoglShaderBindingList::RemoveAll(){
	pCount = 0;
}
