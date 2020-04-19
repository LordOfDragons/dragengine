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

#include "aeUSetRuleSModMaxPosition.h"
#include "../../../animator/rule/aeRuleStateManipulator.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleSModMaxPosition
////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleSModMaxPosition::aeUSetRuleSModMaxPosition( aeRuleStateManipulator *rule, const decVector &newMax ){
	if( ! rule ) DETHROW( deeInvalidParam );
	
	pRule = NULL;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldMax = rule->GetMaximumPosition();
		pNewMax = newMax;
		
		SetShortInfo( "Set state manipulator rule maximum" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeUSetRuleSModMaxPosition::~aeUSetRuleSModMaxPosition(){
	pCleanUp();
}



// Management
///////////////

void aeUSetRuleSModMaxPosition::Undo(){
	pRule->SetMaximumPosition( pOldMax );
}

void aeUSetRuleSModMaxPosition::Redo(){
	pRule->SetMaximumPosition( pNewMax );
}



// Private Functions
//////////////////////

void aeUSetRuleSModMaxPosition::pCleanUp(){
	if( pRule ) pRule->FreeReference();
}
