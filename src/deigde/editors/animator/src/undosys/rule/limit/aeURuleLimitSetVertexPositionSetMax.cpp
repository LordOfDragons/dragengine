/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "aeURuleLimitSetVertexPositionSetMax.h"
#include "../../../animator/rule/aeRuleLimit.h"

#include <dragengine/common/exceptions.h>


// Class aeURuleLimitSetVertexPositionSetMax
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleLimitSetVertexPositionSetMax::aeURuleLimitSetVertexPositionSetMax(
	aeRuleLimit *rule, float newMax )
{
	DEASSERT_NOTNULL( rule )
	
	pRule = nullptr;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldMax = rule->GetMaximumVertexPositionSet();
		pNewMax = newMax;
		
		SetShortInfo( "Limit Set maximum vertex position set" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeURuleLimitSetVertexPositionSetMax::~aeURuleLimitSetVertexPositionSetMax(){
	pCleanUp();
}



// Management
///////////////

void aeURuleLimitSetVertexPositionSetMax::Undo(){
	pRule->SetMaximumVertexPositionSet( pOldMax );
}

void aeURuleLimitSetVertexPositionSetMax::Redo(){
	pRule->SetMaximumVertexPositionSet( pNewMax );
}



// Private Functions
//////////////////////

void aeURuleLimitSetVertexPositionSetMax::pCleanUp(){
	if( pRule ){
		pRule->FreeReference();
	}
}
