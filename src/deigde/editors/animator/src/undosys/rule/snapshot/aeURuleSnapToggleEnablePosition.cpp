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

#include "aeURuleSnapToggleEnablePosition.h"
#include "../../../animator/rule/aeRuleStateSnapshot.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleSnapToggleEnablePosition
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleSnapToggleEnablePosition::aeURuleSnapToggleEnablePosition( aeRuleStateSnapshot *rule ){
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	pRule = NULL;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		SetShortInfo( "Rule state snapshot toggle enable position" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeURuleSnapToggleEnablePosition::~aeURuleSnapToggleEnablePosition(){
	pCleanUp();
}



// Management
///////////////

void aeURuleSnapToggleEnablePosition::Undo(){
	pRule->SetEnablePosition( ! pRule->GetEnablePosition() );
}

void aeURuleSnapToggleEnablePosition::Redo(){
	pRule->SetEnablePosition( ! pRule->GetEnablePosition() );
}



// Private Functions
//////////////////////

void aeURuleSnapToggleEnablePosition::pCleanUp(){
	if( pRule ){
		pRule->FreeReference();
	}
}
