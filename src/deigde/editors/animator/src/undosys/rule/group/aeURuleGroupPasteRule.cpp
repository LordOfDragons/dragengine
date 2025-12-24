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

aeURuleGroupPasteRule::aeURuleGroupPasteRule(aeRuleGroup *group, const aeRule::List &ruleList, int index) :

pIndex(index){
	DEASSERT_NOTNULL(group)
	DEASSERT_TRUE(ruleList.IsNotEmpty())
	
	ruleList.Visit([&](aeRule &rule){
		pRuleList.Add(rule.CreateCopy());
	});
	
	pGroup = group;
}

aeURuleGroupPasteRule::~aeURuleGroupPasteRule(){
	pCleanUp();
}



// Management
///////////////

void aeURuleGroupPasteRule::Undo(){
	pRuleList.Visit([&](aeRule *rule){
		pGroup->RemoveRule(rule);
	});
	
	// remove links added in a prior redo
	aeAnimator * const animator = pGroup->GetAnimator();
	
	if(animator){
		pRemoveLinkList.Visit([&](aeLink *link){
			animator->RemoveLink(link);
		});
		pRemoveLinkList.RemoveAll();
	}
	
	// remove controller added in a prior redo
	if(animator){
		pRemoveControllerList.Visit([&](aeController *controller){
			animator->RemoveController(controller);
		});
		pRemoveControllerList.RemoveAll();
	}
}

void aeURuleGroupPasteRule::Redo(){
	aeAnimator * const animator = pGroup->GetAnimator();
	
	pRemoveLinkList.RemoveAll();
	
	pRuleList.VisitIndexed([&](int i, aeRule *rule){
		// check if links exist in the animator. if not add them and mark them to remove
		// during undo. the same for controllers but with a difference. controllers are
		// named so if a controller with the same name exists already reuse it
		if(animator){
			aeLink::List linkList;
			rule->ListLinks(linkList);
			
			linkList.Visit([&](aeLink *link){
				if(!animator->GetLinks().Has(link)){
					aeController * const controller = link->GetController();
					if(controller && !animator->GetControllers().HasMatching([&](const aeController &c){
						return c.GetName() == controller->GetName();
					})){
						pRemoveControllerList.Add(controller);
						animator->AddController(controller);
					}
					
					pRemoveLinkList.Add(link);
					animator->AddLink(link);
				}
			});
		}
		
		// insert the rule
		pGroup->InsertRuleAt(rule, pIndex + i);
	});
}



// Private Functions
//////////////////////

void aeURuleGroupPasteRule::pCleanUp(){
}
