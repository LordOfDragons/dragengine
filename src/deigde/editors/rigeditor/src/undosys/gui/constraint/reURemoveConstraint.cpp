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
#include "../../../rig/constraint/reSelectionConstraints.h"

#include <dragengine/common/exceptions.h>



// Class reURemoveConstraint
//////////////////////////////

// Constructor, destructor
////////////////////////////

reURemoveConstraint::reURemoveConstraint(reRigConstraint::List &list){
	DEASSERT_TRUE(list.IsNotEmpty())
	
	pRig = list.First()->GetRig();
	DEASSERT_NOTNULL(pRig)
	
	list.Visit([&](reRigConstraint *constraint){
		DEASSERT_TRUE(constraint->GetRig() == pRig)
		const cEntry::Ref entry(cEntry::Ref::New());
		entry->constraint = constraint;
		entry->bone = constraint->GetRigBone();
		pEntries.Add(entry);
	});
	
	SetShortInfo("Remove Constraints");
}

reURemoveConstraint::~reURemoveConstraint(){
}



// Management
///////////////

void reURemoveConstraint::Undo(){
	reSelectionConstraints &selection = *pRig->GetSelectionConstraints();
	
	selection.RemoveAllConstraints();
	
	pEntries.Visit([&](const cEntry &e){
		if(e.bone){
			e.bone->AddConstraint(e.constraint);
			
		}else{
			pRig->AddConstraint(e.constraint);
		}
		
		selection.AddConstraint(e.constraint);
	});
}

void reURemoveConstraint::Redo(){
	reSelectionConstraints &selection = *pRig->GetSelectionConstraints();
	
	pEntries.Visit([&](const cEntry &e){
		if(e.constraint->GetSelected()){
			selection.RemoveConstraint(e.constraint);
		}
		
		if(e.bone){
			e.bone->RemoveConstraint(e.constraint);
			
		}else{
			pRig->RemoveConstraint(e.constraint);
		}
	});
}
