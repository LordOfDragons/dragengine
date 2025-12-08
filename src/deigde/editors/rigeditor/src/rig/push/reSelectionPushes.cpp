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
#include "reSelectionPushes.h"
#include "reRigPush.h"
#include "reRigPushList.h"
#include "../reRig.h"
#include "dragengine/common/exceptions.h"



// Class reSelectionPushes
////////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionPushes::reSelectionPushes(reRig *rig){
	if(!rig) DETHROW(deeInvalidParam);
	
	pRig = rig;
	
	pPushes = NULL;
	pPushCount = 0;
	pPushSize = 0;
	pActivePush = NULL;
}

reSelectionPushes::~reSelectionPushes(){
	Reset();
	if(pPushes) delete [] pPushes;
}



// Management
///////////////

reRigPush *reSelectionPushes::GetPushAt(int index) const{
	if(index < 0 || index >= pPushCount) DETHROW(deeOutOfBoundary);
	
	return pPushes[index];
}

bool reSelectionPushes::HasPush(reRigPush::Ref push) const{
	if(!push) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(push == pPushes[i]){
			return true;
		}
	}
	
	return false;
}
	
int reSelectionPushes::IndexOfPush(reRigPush::Ref push) const{
	if(!push) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(push == pPushes[i]){
			return i;
		}
	}
	
	return -1;
}

int reSelectionPushes::IndexOfPushWith(deColliderVolume *collider) const{
	if(!collider) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(collider == pPushes[i]->GetCollider()){
			return i;
		}
	}
	
	return -1;
}

void reSelectionPushes::AddPush(reRigPush::Ref push){
	if(HasPush(push)) DETHROW(deeInvalidParam);
	
	if(pPushCount == pPushSize){
		int newSize = pPushSize * 3 / 2 + 1;
		reRigPush **newArray = new reRigPush*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pPushes){
			memcpy(newArray, pPushes, sizeof(reRigPush*) * pPushSize);
			delete [] pPushes;
		}
		pPushes = newArray;
		pPushSize = newSize;
	}
	
	pPushes[pPushCount] = push;
	pPushCount++;
	push->SetSelected(true);
	
	pRig->NotifyPushSelectedChanged(push);
	
	if(pActivePush == NULL){
		SetActivePush(push);
	}
}

void reSelectionPushes::RemovePush(reRigPush::Ref push){
	int i, index = IndexOfPush(push);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pPushCount; i++){
		pPushes[i - 1] = pPushes[i];
	}
	pPushes[pPushCount - 1] = NULL;
	pPushCount--;
	
	push->SetSelected(false);
	
	if(push == pActivePush){
		if(pPushCount > 0){
			SetActivePush(pPushes[0]);
			
		}else{
			SetActivePush(NULL);
		}
	}
	
	pRig->NotifyPushSelectedChanged(push);
}

void reSelectionPushes::RemoveAllPushes(){
	SetActivePush(NULL);
	
	pRig->NotifyAllPushesDeselected();
	
	while(pPushCount > 0){
		pPushCount--;
		
		pPushes[pPushCount]->SetSelected(false);
		pPushes[pPushCount]->FreeReference();
	}
}



bool reSelectionPushes::HasActivePush() const{
	return pActivePush != NULL;
}

void reSelectionPushes::SetActivePush(reRigPush::Ref push){
	if(push != pActivePush){
		if(push && !HasPush(push)) DETHROW(deeInvalidParam);
		
		if(pActivePush){
			pActivePush->SetActive(false);
		}
		
		pActivePush = push;
		
		if(push){
			push->SetActive(true);
		}
		
		pRig->NotifyActivePushChanged();
	}
}

void reSelectionPushes::Reset(){
	RemoveAllPushes();
}

void reSelectionPushes::AddVisiblePushesTo(reRigPushList &list) const{
	int c;
	
	for(c=0; c<pPushCount; c++){
		if(pPushes[c]->IsVisible()){
			list.AddPush(pPushes[c]);
		}
	}
}
