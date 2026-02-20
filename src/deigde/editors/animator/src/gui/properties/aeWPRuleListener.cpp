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

#include <stdlib.h>
#include <string.h>

#include "aeWPRule.h"
#include "aeWPRuleListener.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeWPRuleListener
///////////////////////////

// Constructor, destructor
////////////////////////////

aeWPRuleListener::aeWPRuleListener(aeWPRule &panel) :
pPanel(panel){
}

aeWPRuleListener::~aeWPRuleListener(){
}



// Notifications
//////////////////

void aeWPRuleListener::ModelChanged(aeAnimator *animator){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateRuleVertexPositionSetList();
}

void aeWPRuleListener::RigChanged(aeAnimator *animator){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateRuleBoneList();
}

void aeWPRuleListener::AnimationChanged(aeAnimator *animator){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateRuleMoveList();
}



void aeWPRuleListener::ControllerNameChanged(aeAnimator *animator, aeController*){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void aeWPRuleListener::ControllerStructureChanged(aeAnimator *animator){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateControllerList();
}



void aeWPRuleListener::LinkNameChanged(aeAnimator *animator, aeLink*){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateLinkList();
}

void aeWPRuleListener::LinkStructureChanged(aeAnimator *animator){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateLinkList();
}



void aeWPRuleListener::ActiveRuleChanged(aeAnimator *animator, aeRule*){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.SelectActiveRule();
	pPanel.ShowActiveSourcePanel();
	pPanel.UpdateRule();
}

void aeWPRuleListener::RuleChanged(aeAnimator*, aeRule *rule){
	if(pPanel.GetRule() != rule){
		return;
	}
	
	pPanel.UpdateRule();
}

void aeWPRuleListener::RuleNameChanged(aeAnimator *animator, aeRule*){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateRuleTree();
	pPanel.UpdateRule();
}

void aeWPRuleListener::RuleStructureChanged(aeAnimator *animator){
	if(pPanel.GetAnimator() != animator){
		return;
	}
	
	pPanel.UpdateRuleTree();
}
