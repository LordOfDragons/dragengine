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
#include "reRigPush.h"
#include "reRigPushList.h"
#include "dragengine/common/exceptions.h"



// Class reRigPushList
////////////////////////

// Constructor, destructor
////////////////////////////

reRigPushList::reRigPushList(){
	pPushes = NULL;
	pPushCount = 0;
	pPushSize = 0;
}

reRigPushList::~reRigPushList(){
	RemoveAllPushes();
	if(pPushes) delete [] pPushes;
}



// Management
///////////////

reRigPush *reRigPushList::GetPushAt(int index) const{
	if(index < 0 || index >= pPushCount) DETHROW(deeInvalidParam);
	
	return pPushes[index];
}

int reRigPushList::IndexOfPush(reRigPush *push) const{
	if(!push) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(push == pPushes[i]) return i;
	}
	
	return -1;
}

bool reRigPushList::HasPush(reRigPush *push) const{
	if(!push) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(push == pPushes[i]) return true;
	}
	
	return false;
}

void reRigPushList::AddPush(reRigPush *push){
	if(HasPush(push)) DETHROW(deeInvalidParam);
	
	if(pPushCount == pPushSize){
		int newSize = pPushSize * 3 / 2 + 1;
		reRigPush **newArray = new reRigPush*[newSize];
		if(pPushes){
			memcpy(newArray, pPushes, sizeof(reRigPush*) * pPushSize);
			delete [] pPushes;
		}
		pPushes = newArray;
		pPushSize = newSize;
	}
	
	pPushes[pPushCount] = push;
	pPushCount++;
	
	push->AddReference();
}

void reRigPushList::RemovePush(reRigPush *push){
	int i, index = IndexOfPush(push);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pPushCount; i++){
		pPushes[i - 1] = pPushes[i];
	}
	pPushCount--;
	
	push->FreeReference();
}

void reRigPushList::RemoveAllPushes(){
	while(pPushCount > 0){
		pPushCount--;
		pPushes[pPushCount]->FreeReference();
	}
}
