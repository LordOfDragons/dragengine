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
#include "deDecalList.h"
#include "../decal/deDecal.h"
#include "../../common/exceptions.h"



// Class deDecalList
//////////////////////

// Constructor, destructor
////////////////////////////

deDecalList::deDecalList(){
	pDecals = NULL;
	pDecalCount = 0;
	pDecalSize = 0;
}

deDecalList::~deDecalList(){
	RemoveAllDecals();
	if(pDecals) delete [] pDecals;
}



// Management
///////////////

deDecal *deDecalList::GetDecalAt(int index) const{
	if(index < 0 || index >= pDecalCount) DETHROW(deeInvalidParam);
	
	return pDecals[index];
}

bool deDecalList::HasDecal(deDecal::Ref decal) const{
	if(!decal) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pDecalCount; i++){
		if(decal == pDecals[i]){
			return true;
		}
	}
	
	return false;
}

int deDecalList::IndexOfDecal(deDecal::Ref decal) const{
	if(!decal) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pDecalCount; i++){
		if(decal == pDecals[i]){
			return i;
		}
	}
	
	return -1;
}

void deDecalList::AddDecal(deDecal::Ref decal){
	if(HasDecal(decal)) DETHROW(deeInvalidParam);
	
	if(pDecalCount == pDecalSize){
		int newSize = pDecalSize * 3 / 2 + 1;
		deDecal **newArray = new deDecal*[newSize];
		if(pDecals){
			memcpy(newArray, pDecals, sizeof(deDecal*) * pDecalSize);
			delete [] pDecals;
		}
		pDecals = newArray;
		pDecalSize = newSize;
	}
	
	pDecals[pDecalCount] = decal;
	pDecalCount++;
}

void deDecalList::RemoveAllDecals(){
	while(pDecalCount > 0){
		pDecalCount--;
		pDecals[pDecalCount]->FreeReference();
	}
}
