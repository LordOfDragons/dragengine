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
#include "reRigConstraint.h"
#include "reRigConstraintList.h"
#include "dragengine/common/exceptions.h"



// Class reRigConstraintList
/////////////////////////

// Constructor, destructor
////////////////////////////

reRigConstraintList::reRigConstraintList(){
	pConstraints = NULL;
	pConstraintCount = 0;
	pConstraintSize = 0;
}

reRigConstraintList::~reRigConstraintList(){
	RemoveAllConstraints();
	if(pConstraints) delete [] pConstraints;
}



// Management
///////////////

reRigConstraint *reRigConstraintList::GetConstraintAt(int index) const{
	if(index < 0 || index >= pConstraintCount) DETHROW(deeInvalidParam);
	
	return pConstraints[index];
}

int reRigConstraintList::IndexOfConstraint(reRigConstraint::Ref constraint) const{
	if(!constraint) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(constraint == pConstraints[i]) return i;
	}
	
	return -1;
}

bool reRigConstraintList::HasConstraint(reRigConstraint::Ref constraint) const{
	if(!constraint) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(constraint == pConstraints[i]) return true;
	}
	
	return false;
}

void reRigConstraintList::AddConstraint(reRigConstraint::Ref constraint){
	if(HasConstraint(constraint)) DETHROW(deeInvalidParam);
	
	if(pConstraintCount == pConstraintSize){
		int newSize = pConstraintSize * 3 / 2 + 1;
		reRigConstraint **newArray = new reRigConstraint*[newSize];
		if(pConstraints){
			memcpy(newArray, pConstraints, sizeof(reRigConstraint*) * pConstraintSize);
			delete [] pConstraints;
		}
		pConstraints = newArray;
		pConstraintSize = newSize;
	}
	
	pConstraints[pConstraintCount] = constraint;
	pConstraintCount++;
}

void reRigConstraintList::RemoveConstraint(reRigConstraint::Ref constraint){
	int i, index = IndexOfConstraint(constraint);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pConstraintCount; i++){
		pConstraints[i - 1] = pConstraints[i];
	}
	pConstraintCount--;
}

void reRigConstraintList::RemoveAllConstraints(){
	while(pConstraintCount > 0){
		pConstraintCount--;
		pConstraints[pConstraintCount]->FreeReference();
	}
}
