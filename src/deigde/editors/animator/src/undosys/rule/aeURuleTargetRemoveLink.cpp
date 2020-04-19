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
#include <stdlib.h>
#include <string.h>

#include "aeURuleTargetRemoveLink.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/controller/aeControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleTargetRemoveLink
//////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleTargetRemoveLink::aeURuleTargetRemoveLink( aeRule *rule, aeControllerTarget *target, aeLink *link ){
	if( ! rule || ! target || ! link ) DETHROW( deeInvalidParam );
	
	pRule = NULL;
	pTarget = NULL;
	pLink = NULL;
	
	SetShortInfo( "Rule Target Remove Link" );
	
	pRule = rule;
	pRule->AddReference();
	
	pTarget = target;
	
	pLink = link;
	pLink->AddReference();
}

aeURuleTargetRemoveLink::~aeURuleTargetRemoveLink(){
	if( pLink ) pLink->FreeReference();
	if( pRule ) pRule->FreeReference();
}



// Management
///////////////

void aeURuleTargetRemoveLink::Undo(){
	pTarget->AddLink( pLink );
	pRule->NotifyRuleChanged();
}

void aeURuleTargetRemoveLink::Redo(){
	pTarget->RemoveLink( pLink );
	pRule->NotifyRuleChanged();
}
