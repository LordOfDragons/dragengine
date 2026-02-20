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

#include "deAnimatorRuleSubAnimator.h"
#include "deAnimatorRuleVisitor.h"
#include "../deAnimator.h"
#include "../controller/deAnimatorController.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/collection/decTList.h"



// Class deAnimatorRuleSubAnimator
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleSubAnimator::deAnimatorRuleSubAnimator() :
pEnableVertexPositionSet(true)
{
	pEnablePosition = true;
	pEnableOrientation = true;
	pEnableSize = true;
}

deAnimatorRuleSubAnimator::~deAnimatorRuleSubAnimator(){
}



// Management
///////////////

void deAnimatorRuleSubAnimator::SetSubAnimator(deAnimator *animator){
	if(animator == pSubAnimator){
		return;
	}
	
	pSubAnimator = animator;
	UpdateConnectionCount();
}

void deAnimatorRuleSubAnimator::SetEnablePosition(bool enabled){
	pEnablePosition = enabled;
}

void deAnimatorRuleSubAnimator::SetEnableOrientation(bool enabled){
	pEnableOrientation = enabled;
}

void deAnimatorRuleSubAnimator::SetEnableSize(bool enabled){
	pEnableSize = enabled;
}

void deAnimatorRuleSubAnimator::SetEnableVertexPositionSet(bool enabled){
	pEnableVertexPositionSet = enabled;
}



void deAnimatorRuleSubAnimator::UpdateConnectionCount(){
	pConnections.RemoveAll();
	
	if(!pSubAnimator){
		return;
	}
	
	pConnections.AddRange(pSubAnimator->GetControllers().GetCount(), -1);
}

int deAnimatorRuleSubAnimator::GetConnectionAt(int targetController) const{
	return pConnections.GetAt(targetController);
}

void deAnimatorRuleSubAnimator::SetConnectionAt(int targetController, int localController){
	DEASSERT_TRUE(localController >= -1)
	
	pConnections.GetAt(targetController) = localController;
}

void deAnimatorRuleSubAnimator::SetMatchingConnections(const deAnimator &animator){
	if(!pSubAnimator){
		ClearConnections();
		return;
	}
	
	int i;
	for(i=0; i<pConnections.GetCount(); i++){
		pConnections.SetAt(i, animator.GetControllers().IndexOfNamed(
			pSubAnimator->GetControllers().GetAt(i)->GetName()));
	}
}

void deAnimatorRuleSubAnimator::ClearConnections(){
	pConnections.SetRangeAt(0, pConnections.GetCount(), -1);
}



// Visiting
/////////////

void deAnimatorRuleSubAnimator::Visit(deAnimatorRuleVisitor &visitor){
	visitor.VisitSubAnimator(*this);
}
