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

#include "aeURuleLimitSetTargetVertexPositionSet.h"
#include "../../../animator/rule/aeRuleLimit.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleLimitSetTargetVertexPositionSet
/////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleLimitSetTargetVertexPositionSet::aeURuleLimitSetTargetVertexPositionSet(
	aeRuleLimit *rule, const char *newVertexPositionSet )
{
	DEASSERT_NOTNULL( rule )
	
	pRule = nullptr;
	
	pOldVertexPositionSet = rule->GetTargetVertexPositionSet();
	pNewVertexPositionSet = newVertexPositionSet;
	
	SetShortInfo( "Limit set target vertex position set" );
	
	pRule = rule;
	pRule->AddReference();
}

aeURuleLimitSetTargetVertexPositionSet::~aeURuleLimitSetTargetVertexPositionSet(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeURuleLimitSetTargetVertexPositionSet::Undo(){
	pRule->SetTargetVertexPositionSet( pOldVertexPositionSet );
}

void aeURuleLimitSetTargetVertexPositionSet::Redo(){
	pRule->SetTargetVertexPositionSet( pNewVertexPositionSet );
}
