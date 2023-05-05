/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

aeWPRuleListener::aeWPRuleListener( aeWPRule &panel ) :
pPanel( panel ){
}

aeWPRuleListener::~aeWPRuleListener(){
}



// Notifications
//////////////////

void aeWPRuleListener::ModelChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateRuleVertexPositionSetList();
}

void aeWPRuleListener::RigChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateRuleBoneList();
}

void aeWPRuleListener::AnimationChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateRuleMoveList();
}



void aeWPRuleListener::ControllerNameChanged( aeAnimator *animator, aeController* ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void aeWPRuleListener::ControllerStructureChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateControllerList();
}



void aeWPRuleListener::LinkNameChanged( aeAnimator *animator, aeLink* ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateLinkList();
}

void aeWPRuleListener::LinkStructureChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateLinkList();
}



void aeWPRuleListener::ActiveRuleChanged( aeAnimator *animator, aeRule* ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.SelectActiveRule();
	pPanel.ShowActiveSourcePanel();
	pPanel.UpdateRule();
}

void aeWPRuleListener::RuleChanged( aeAnimator*, aeRule *rule ){
	if( pPanel.GetRule() != rule ){
		return;
	}
	
	pPanel.UpdateRule();
}

void aeWPRuleListener::RuleNameChanged( aeAnimator *animator, aeRule* ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateRuleTree();
	pPanel.UpdateRule();
}

void aeWPRuleListener::RuleStructureChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateRuleTree();
}
