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

#include "aeURuleIKSetGoalPosition.h"
#include "../../../animator/rule/aeRuleInverseKinematic.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleIKSetGoalPosition
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleIKSetGoalPosition::aeURuleIKSetGoalPosition( aeRuleInverseKinematic *rule, const decVector &newPos ){
	if( ! rule ) DETHROW( deeInvalidParam );
	
	pRule = NULL;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldPos = rule->GetGoalPosition();
		pNewPos = newPos;
		
		SetShortInfo( "Set inverse kinematic rule goal position" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeURuleIKSetGoalPosition::~aeURuleIKSetGoalPosition(){
	pCleanUp();
}



// Management
///////////////

void aeURuleIKSetGoalPosition::Undo(){
	pRule->SetGoalPosition( pOldPos );
}

void aeURuleIKSetGoalPosition::Redo(){
	pRule->SetGoalPosition( pNewPos );
}



// Private Functions
//////////////////////

void aeURuleIKSetGoalPosition::pCleanUp(){
	if( pRule ) pRule->FreeReference();
}
