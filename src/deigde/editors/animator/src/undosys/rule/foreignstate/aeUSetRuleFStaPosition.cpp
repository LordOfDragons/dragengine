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

#include "aeUSetRuleFStaPosition.h"
#include "../../../animator/rule/aeRuleForeignState.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleFStaPosition
/////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleFStaPosition::aeUSetRuleFStaPosition( aeRuleForeignState *rule, float newScale ){
	if( ! rule ) DETHROW( deeInvalidParam );
	
	pRule = NULL;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldScale = rule->GetScalePosition();
		pNewScale = newScale;
		
		SetShortInfo( "Set foreign state rule position scale" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeUSetRuleFStaPosition::~aeUSetRuleFStaPosition(){
	pCleanUp();
}



// Management
///////////////

void aeUSetRuleFStaPosition::Undo(){
	pRule->SetScalePosition( pOldScale );
}

void aeUSetRuleFStaPosition::Redo(){
	pRule->SetScalePosition( pNewScale );
}



// Private Functions
//////////////////////

void aeUSetRuleFStaPosition::pCleanUp(){
	if( pRule ) pRule->FreeReference();
}
