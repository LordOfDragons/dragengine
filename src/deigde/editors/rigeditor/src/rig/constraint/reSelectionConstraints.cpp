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

#include "reSelectionConstraints.h"
#include "reRigConstraint.h"
#include "../reRig.h"

#include <dragengine/common/exceptions.h>



// Class reSelectionConstraints
/////////////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionConstraints::reSelectionConstraints(reRig *rig){
	DEASSERT_NOTNULL(rig)
	pRig = rig;
}

reSelectionConstraints::~reSelectionConstraints(){
	Reset();
}



// Management
///////////////
int reSelectionConstraints::IndexOfConstraintWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	
	return pConstraints.IndexOfMatching([collider](const reRigConstraint &c){
		return c.GetCollider() == collider;
	});
}

void reSelectionConstraints::AddConstraint(reRigConstraint *constraint){
	DEASSERT_FALSE(pConstraints.Has(constraint))
	
	pConstraints.Add(constraint);
	constraint->SetSelected(true);
	
	pRig->NotifyConstraintSelectedChanged(constraint);
	
	if(!pActiveConstraint){
		SetActiveConstraint(constraint);
	}
}

void reSelectionConstraints::RemoveConstraint(reRigConstraint *constraint){
	const reRigConstraint::Ref guard(constraint);
	pConstraints.Remove(constraint);
	constraint->SetSelected(false);
	
	if(constraint == pActiveConstraint){
		if(pConstraints.IsNotEmpty()){
			SetActiveConstraint(pConstraints.First());
			
		}else{
			SetActiveConstraint(nullptr);
		}
	}
	
	pRig->NotifyConstraintSelectedChanged(constraint);
}

void reSelectionConstraints::RemoveAllConstraints(){
	SetActiveConstraint(nullptr);
	
	pRig->NotifyAllConstraintsDeselected();
	
	pConstraints.Visit([](reRigConstraint &c){
		c.SetSelected(false);
	});
	pConstraints.RemoveAll();
}



bool reSelectionConstraints::HasActiveConstraint() const{
	return pActiveConstraint.IsNotNull();
}

void reSelectionConstraints::SetActiveConstraint(reRigConstraint *constraint){
	if(constraint != pActiveConstraint){
		if(constraint){
			DEASSERT_TRUE(pConstraints.Has(constraint))
		}
		
		if(pActiveConstraint){
			pActiveConstraint->SetActive(false);
		}
		
		pActiveConstraint = constraint;
		
		if(constraint){
			constraint->SetActive(true);
		}
		
		pRig->NotifyActiveConstraintChanged();
	}
}

void reSelectionConstraints::Reset(){
	RemoveAllConstraints();
}
void reSelectionConstraints::AddVisibleConstraintsTo(reRigConstraint::List &list) const{
	pConstraints.Visit([&list](reRigConstraint *c){
		if(c->IsVisible()){
			list.Add(c);
		}
	});
}
