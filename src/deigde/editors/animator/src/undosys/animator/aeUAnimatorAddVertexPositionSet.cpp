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

#include "aeUAnimatorAddVertexPositionSet.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>



// Class aeUAnimatorAddVertexPositionSet
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeUAnimatorAddVertexPositionSet::aeUAnimatorAddVertexPositionSet(aeAnimator *animator, const char *pattern) :
pAnimator(animator)
{
	DEASSERT_NOTNULL(pattern)
	
	const decStringSet &sets = animator->GetListVertexPositionSets();
	const deComponent * const engComponent = animator->GetEngineComponent();
	const deModel * const engModel = engComponent ? engComponent->GetModel() : nullptr;
	
	if(engModel){
		const int setCount = engModel->GetVertexPositionSetCount();
		int i;
		
		for(i=0; i<setCount; i++){
			const decString &set = engModel->GetVertexPositionSetAt(i)->GetName();
			
			if(set.MatchesPattern(pattern) && ! sets.Has(set)){
				pVertexPositionSets.Add(set);
			}
		}
		
	}else{
		if(! sets.Has(pattern)){
			pVertexPositionSets.Add(pattern);
		}
	}
	
	if(pVertexPositionSets.GetCount() == 1){
		SetShortInfo("Animator add vertex position set");
		
	}else{
		SetShortInfo("Animator add vertex position sets");
	}
}

aeUAnimatorAddVertexPositionSet::~aeUAnimatorAddVertexPositionSet(){
}



// Management
///////////////

void aeUAnimatorAddVertexPositionSet::Undo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pAnimator->RemoveVertexPositionSet(pVertexPositionSets.GetAt(i));
	}
}

void aeUAnimatorAddVertexPositionSet::Redo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pAnimator->AddVertexPositionSet(pVertexPositionSets.GetAt(i));
	}
}
