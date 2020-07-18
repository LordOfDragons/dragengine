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

#include "aeURuleTargetRemoveAllLinks.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/controller/aeControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleTargetRemoveAllLinks
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleTargetRemoveAllLinks::aeURuleTargetRemoveAllLinks( aeRule *rule,
aeControllerTarget *target, const aeLinkList &links ) :
pRule( NULL ),
pTarget( NULL ),
pLinks( links )
{
	if( ! rule || ! target || links.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Rule Target Remove All Links" );
	
	pRule = rule;
	pRule->AddReference();
	
	pTarget = target;
}

aeURuleTargetRemoveAllLinks::~aeURuleTargetRemoveAllLinks(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeURuleTargetRemoveAllLinks::Undo(){
	const int count = pLinks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pTarget->AddLink( pLinks.GetAt( i ) );
	}
	pRule->NotifyRuleChanged();
}

void aeURuleTargetRemoveAllLinks::Redo(){
	const int count = pLinks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pTarget->RemoveLink( pLinks.GetAt( i ) );
	}
	pRule->NotifyRuleChanged();
}
