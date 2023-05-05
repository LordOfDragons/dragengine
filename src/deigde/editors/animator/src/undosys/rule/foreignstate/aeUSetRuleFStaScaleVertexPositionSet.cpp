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

#include "aeUSetRuleFStaScaleVertexPositionSet.h"
#include "../../../animator/rule/aeRuleForeignState.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleFStaScaleVertexPositionSet
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleFStaScaleVertexPositionSet::aeUSetRuleFStaScaleVertexPositionSet(
	aeRuleForeignState *rule, float newScale ) :
pRule( nullptr )
{
	DEASSERT_NOTNULL( rule )
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldScale = rule->GetScaleVertexPositionSet();
		pNewScale = newScale;
		
		SetShortInfo( "Set foreign state rule vertex position set scale" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeUSetRuleFStaScaleVertexPositionSet::~aeUSetRuleFStaScaleVertexPositionSet(){
	pCleanUp();
}



// Management
///////////////

void aeUSetRuleFStaScaleVertexPositionSet::Undo(){
	pRule->SetScaleVertexPositionSet( pOldScale );
}

void aeUSetRuleFStaScaleVertexPositionSet::Redo(){
	pRule->SetScaleVertexPositionSet( pNewScale );
}



// Private Functions
//////////////////////

void aeUSetRuleFStaScaleVertexPositionSet::pCleanUp(){
	if( pRule ){
		pRule->FreeReference();
	}
}
