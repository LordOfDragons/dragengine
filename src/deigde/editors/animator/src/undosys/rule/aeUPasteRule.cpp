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
#include <string.h>
#include <stdlib.h>

#include "aeUPasteRule.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeUPasteRule
///////////////////////

// Constructor, destructor
////////////////////////////

aeUPasteRule::aeUPasteRule(aeAnimator::Ref animator, const aeRuleList &ruleList, int index) :

pIndex(index){
	const int ruleCount = ruleList.GetCount();
	
	if(!animator || ruleCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	aeRule::Ref rule = NULL;
	int i;
	
	try{
		for(i=0; i<ruleCount; i++){
			rule = ruleList.GetAt(i)->CreateCopy();
			pRuleList.Add(rule);
			rule = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
	
	pAnimator = animator;
}

aeUPasteRule::~aeUPasteRule(){
	pCleanUp();
}



// Management
///////////////

void aeUPasteRule::Undo(){
	const int ruleCount = pRuleList.GetCount();
	int i;
	
	// remove the rules
	for(i=0; i<ruleCount; i++){
		pAnimator->RemoveRule(pRuleList.GetAt(i));
	}
	
	// remove links added in a prior redo
	const int linkCount = pRemoveLinkList.GetCount();
	for(i=0; i<linkCount; i++){
		pAnimator->RemoveLink(pRemoveLinkList.GetAt(i));
	}
	pRemoveLinkList.RemoveAll();
	
	// remove controller added in a prior redo
	const int controllerCount = pRemoveControllerList.GetCount();
	for(i=0; i<controllerCount; i++){
		pAnimator->RemoveController(pRemoveControllerList.GetAt(i));
	}
	pRemoveControllerList.RemoveAll();
}

void aeUPasteRule::Redo(){
	const aeControllerList &controllers = pAnimator->GetControllers();
	const aeLinkList &links = pAnimator->GetLinks();
	const int ruleCount = pRuleList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	pRemoveControllerList.RemoveAll();
	
	for(i=0; i<ruleCount; i++){
		aeRule * const rule = pRuleList.GetAt(i);
		
		// check if links exist in the animator. if not add them and mark them to remove during undo
		aeLinkList linkList;
		rule->ListLinks(linkList);
		
		const int linkCount = linkList.GetCount();
		for(j=0; j<linkCount; j++){
			aeLink * const link = linkList.GetAt(j);
			if(links.Has(link)){
				continue;
			}
			
			// check if controllers exist in the animator. if not add them and mark them to
			// remove during undo. controllers are matched by name. if controller has been
			// added modify all links. this should be safe since only pasted links not present
			// in the animator can be potentially modified
			aeController * const controller = link->GetController();
			if(controller && !controllers.Has(controller)){
				aeController * const sameNameController = controllers.GetNamed(controller->GetName());
				
				if(sameNameController){
					link->SetController(sameNameController, false);
					
				}else{
					link->SetController(controller, false);
					pRemoveControllerList.Add(controller);
					pAnimator->AddController(controller);
				}
			}
			
			pRemoveLinkList.Add(link);
			pAnimator->AddLink(link);
		}
		
		// insert the rule
		pAnimator->InsertRuleAt(rule, pIndex + i);
	}
}



// Private Functions
//////////////////////

void aeUPasteRule::pCleanUp(){
}
