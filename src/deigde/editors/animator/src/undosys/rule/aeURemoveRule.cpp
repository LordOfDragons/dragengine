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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aeURemoveRule.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeURemoveRule
////////////////////////

// Constructor, destructor
////////////////////////////

aeURemoveRule::aeURemoveRule( aeAnimator *animator, aeRule *rule ){
	if( ! animator || ! rule ) DETHROW( deeInvalidParam );
	
	pAnimator = NULL;
	pRule = NULL;
	
	pIndex = animator->GetRules().IndexOf( rule );
	if( pIndex == -1 ) DETHROW( deeInvalidParam );
	
	pAnimator = animator;
	animator->AddReference();
	
	pRule = rule;
	rule->AddReference();
}

aeURemoveRule::~aeURemoveRule(){
	pCleanUp();
}



// Management
///////////////

void aeURemoveRule::Undo(){
	pAnimator->InsertRuleAt( pRule, pIndex );
}

void aeURemoveRule::Redo(){
	pAnimator->RemoveRule( pRule );
}



// Private Functions
//////////////////////

void aeURemoveRule::pCleanUp(){
	if( pRule ) pRule->FreeReference();
	if( pAnimator ) pAnimator->FreeReference();
}
