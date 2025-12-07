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

#include "reURemovePush.h"
#include "../../../rig/reRig.h"
#include "../../../rig/push/reRigPush.h"
#include "../../../rig/push/reRigPushList.h"
#include "../../../rig/push/reSelectionPushes.h"

#include "dragengine/common/exceptions.h"



// Class reURemovePush
////////////////////////

// Constructor, destructor
////////////////////////////

reURemovePush::reURemovePush(reRigPushList &list){
	int pushCount = list.GetPushCount();
	
	if(pushCount == 0) DETHROW(deeInvalidParam);
	
	pPushes = NULL;
	pPushCount = 0;
	
	try{
		pPushes = new reRigPush*[pushCount];
		if(! pPushes) DETHROW(deeOutOfMemory);
		
		while(pPushCount < pushCount){
			pPushes[pPushCount] = list.GetPushAt(pPushCount);
			pPushes[pPushCount]->AddReference();
			pPushCount++;
		}
		
		SetShortInfo("Remove Pushes");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reURemovePush::~reURemovePush(){
	pCleanUp();
}



// Management
///////////////

void reURemovePush::Undo(){
	reRig *rig = pPushes[0]->GetRig();
	if(! rig) DETHROW(deeInvalidParam);
	
	reSelectionPushes *selection = rig->GetSelectionPushes();
	int p;
	
	selection->RemoveAllPushes();
	
	for(p=0; p<pPushCount; p++){
		rig->AddPush(pPushes[p]);
		
		selection->AddPush(pPushes[p]);
	}
	
}

void reURemovePush::Redo(){
	reRig *rig = pPushes[0]->GetRig();
	if(! rig) DETHROW(deeInvalidParam);
	
	reSelectionPushes *selection = rig->GetSelectionPushes();
	int p;
	
	for(p=0; p<pPushCount; p++){
		if(pPushes[p]->GetSelected()){
			selection->RemovePush(pPushes[p]);
		}
		
		rig->RemovePush(pPushes[p]);
	}
}



// Private Functions
//////////////////////

void reURemovePush::pCleanUp(){
	if(pPushes){
		while(pPushCount > 0){
			pPushCount--;
			pPushes[pPushCount]->FreeReference();
		}
		
		delete [] pPushes;
	}
}
