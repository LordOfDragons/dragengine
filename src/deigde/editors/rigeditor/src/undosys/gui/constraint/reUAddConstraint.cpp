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

#include "reUAddConstraint.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reSelectionConstraints.h"

#include <dragengine/common/exceptions.h>

// Class reUAddConstraint
///////////////////////////

// Constructor, destructor
////////////////////////////

reUAddConstraint::reUAddConstraint(reRig *rig, reRigBone *bone, reRigConstraint *constraint){
	if(!constraint || (!rig && !bone)) DETHROW(deeInvalidParam);
	
	pRig = rig;
	if(rig) rig->AddReference();
	
	pBone = bone;
	if(bone) bone->AddReference();
	
	pConstraint = constraint;
	constraint->AddReference();
	
	try{
		if(bone){
			SetShortInfo("Add Bone Constraint");
			
		}else{
			SetShortInfo("Add Rig Constraint");
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reUAddConstraint::~reUAddConstraint(){
	pCleanUp();
}

// Management
///////////////

void reUAddConstraint::Undo(){
	reRig *rig = pGetRig();
	
	reSelectionConstraints *selection = rig->GetSelectionConstraints();
	
	if(pConstraint->GetSelected()){
		selection->RemoveConstraint(pConstraint);
	}
	
	if(pBone){
		pBone->RemoveConstraint(pConstraint);
		
	}else{
		rig->RemoveConstraint(pConstraint);
	}
}

void reUAddConstraint::Redo(){
	reRig *rig = pGetRig();
	
	reSelectionConstraints *selection = rig->GetSelectionConstraints();
	
	if(pBone){
		pBone->AddConstraint(pConstraint);
		
	}else{
		rig->AddConstraint(pConstraint);
	}
	
	selection->RemoveAllConstraints();
	selection->AddConstraint(pConstraint);
}

// Private Functions
//////////////////////

void reUAddConstraint::pCleanUp(){

}

reRig *reUAddConstraint::pGetRig(){
	if(pRig) return pRig;
	
	reRig *rig = pBone->GetRig();
	if(!rig) DETHROW(deeInvalidParam);
	
	return rig;
}
