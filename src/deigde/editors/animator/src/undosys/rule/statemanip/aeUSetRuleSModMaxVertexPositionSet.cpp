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

#include "aeUSetRuleSModMaxVertexPositionSet.h"
#include "../../../animator/rule/aeRuleStateManipulator.h"

#include <dragengine/common/exceptions.h>


// Class aeUSetRuleSModMaxVertexPositionSet
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleSModMaxVertexPositionSet::aeUSetRuleSModMaxVertexPositionSet(
	aeRuleStateManipulator *rule, float newMax )
{
	DEASSERT_NOTNULL( rule )
	
	pRule = nullptr;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldMax = rule->GetMaximumVertexPositionSet();
		pNewMax = newMax;
		
		SetShortInfo( "Set state manipulator rule maximum vertex position set" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeUSetRuleSModMaxVertexPositionSet::~aeUSetRuleSModMaxVertexPositionSet(){
	pCleanUp();
}



// Management
///////////////

void aeUSetRuleSModMaxVertexPositionSet::Undo(){
	pRule->SetMaximumVertexPositionSet( pOldMax );
}

void aeUSetRuleSModMaxVertexPositionSet::Redo(){
	pRule->SetMaximumVertexPositionSet( pNewMax );
}



// Private Functions
//////////////////////

void aeUSetRuleSModMaxVertexPositionSet::pCleanUp(){
	if( pRule ){
		pRule->FreeReference();
	}
}
