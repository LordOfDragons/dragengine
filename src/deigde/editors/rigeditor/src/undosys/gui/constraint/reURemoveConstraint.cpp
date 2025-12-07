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

#include "reURemoveConstraint.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reRigConstraintList.h"
#include "../../../rig/constraint/reSelectionConstraints.h"

#include <dragengine/common/exceptions.h>



// Class reURemoveConstraint
//////////////////////////////

// Constructor, destructor
////////////////////////////

reURemoveConstraint::reURemoveConstraint(reRigConstraintList &list){
	int constraintCount = list.GetConstraintCount();
	reRigConstraint *constraint;
	reRigBone *bone;
	
	if(constraintCount == 0) DETHROW(deeInvalidParam);
	
	pRig = list.GetConstraintAt(0)->GetRig();
	if(! pRig) DETHROW(deeInvalidParam);
	pRig->AddReference();
	
	pEntries = NULL;
	pEntryCount = 0;
	
	try{
		pEntries = new sEntry[constraintCount];
		if(! pEntries) DETHROW(deeOutOfMemory);
		
		while(pEntryCount < constraintCount){
			constraint = list.GetConstraintAt(pEntryCount);
			bone = constraint->GetRigBone();
			
			if(constraint->GetRig() != pRig) DETHROW(deeInvalidParam);
			
			pEntries[pEntryCount].constraint = constraint;
			constraint->AddReference();
			
			pEntries[pEntryCount].bone = bone;
			if(bone) bone->AddReference();
			
			pEntryCount++;
		}
		
		SetShortInfo("Remove Constraints");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reURemoveConstraint::~reURemoveConstraint(){
	pCleanUp();
}



// Management
///////////////

void reURemoveConstraint::Undo(){
	reSelectionConstraints *selection = pRig->GetSelectionConstraints();
	int e;
	
	selection->RemoveAllConstraints();
	
	for(e=0; e<pEntryCount; e++){
		if(pEntries[e].bone){
			pEntries[e].bone->AddConstraint(pEntries[e].constraint);
			
		}else{
			pRig->AddConstraint(pEntries[e].constraint);
		}
		
		selection->AddConstraint(pEntries[e].constraint);
	}
	
}

void reURemoveConstraint::Redo(){
	reSelectionConstraints *selection = pRig->GetSelectionConstraints();
	int e;
	
	for(e=0; e<pEntryCount; e++){
		if(pEntries[e].constraint->GetSelected()){
			selection->RemoveConstraint(pEntries[e].constraint);
		}
		
		if(pEntries[e].bone){
			pEntries[e].bone->RemoveConstraint(pEntries[e].constraint);
			
		}else{
			pRig->RemoveConstraint(pEntries[e].constraint);
		}
	}
}



// Private Functions
//////////////////////

void reURemoveConstraint::pCleanUp(){
	if(pEntries){
		while(pEntryCount > 0){
			pEntryCount--;
			
			if(pEntries[pEntryCount].bone){
				pEntries[pEntryCount].bone->FreeReference();
			}
			pEntries[pEntryCount].constraint->FreeReference();
		}
		
		delete [] pEntries;
	}
	
	if(pRig){
		pRig->FreeReference();
	}
}
