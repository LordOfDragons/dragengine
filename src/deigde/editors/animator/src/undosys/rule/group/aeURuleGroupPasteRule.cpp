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

#include "aeURuleGroupPasteRule.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/link/aeLink.h"
#include "../../../animator/rule/aeRuleGroup.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleGroupPasteRule
////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleGroupPasteRule::aeURuleGroupPasteRule(aeRuleGroup::Ref group, const aeRuleList &ruleList, int index) :

pIndex(index){
	const int ruleCount = ruleList.GetCount();
	
	if(!group || ruleCount == 0){
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
	
	pGroup = group;
}

aeURuleGroupPasteRule::~aeURuleGroupPasteRule(){
	pCleanUp();
}



// Management
///////////////

void aeURuleGroupPasteRule::Undo(){
	const int ruleCount = pRuleList.GetCount();
	int i;
	
	for(i=0; i<ruleCount; i++){
		pGroup->RemoveRule(pRuleList.GetAt(i));
	}
	
	// remove links added in a prior redo
	aeAnimator * const animator = pGroup->GetAnimator();
	
	if(animator){
		const int linkCount = pRemoveLinkList.GetCount();
		for(i=0; i<linkCount; i++){
			animator->RemoveLink(pRemoveLinkList.GetAt(i));
		}
		pRemoveLinkList.RemoveAll();
	}
	
	// remove controller added in a prior redo
	if(animator){
		const int controllerCount = pRemoveControllerList.GetCount();
		for(i=0; i<controllerCount; i++){
			animator->RemoveController(pRemoveControllerList.GetAt(i));
		}
		pRemoveControllerList.RemoveAll();
	}
}

void aeURuleGroupPasteRule::Redo(){
	aeAnimator * const animator = pGroup->GetAnimator();
	const int ruleCount = pRuleList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	
	for(i=0; i<ruleCount; i++){
		aeRule * const rule = pRuleList.GetAt(i);
		
		// check if links exist in the animator. if not add them and mark them to remove
		// during undo. the same for controllers but with a difference. controllers are
		// named so if a controller with the same name exists already reuse it
		if(animator){
			aeLinkList linkList;
			rule->ListLinks(linkList);
			
			const int linkCount = linkList.GetCount();
			for(j=0; j<linkCount; j++){
				aeLink * const link = linkList.GetAt(j);
				if(!animator->GetLinks().Has(link)){
					aeController * const controller = link->GetController();
					if(controller && !animator->GetControllers().HasNamed(controller->GetName())){
						pRemoveControllerList.Add(controller);
						animator->AddController(controller);
					}
					
					pRemoveLinkList.Add(link);
					animator->AddLink(link);
				}
			}
		}
		
		// insert the rule
		pGroup->InsertRuleAt(rule, pIndex + i);
	}
}



// Private Functions
//////////////////////

void aeURuleGroupPasteRule::pCleanUp(){
}
