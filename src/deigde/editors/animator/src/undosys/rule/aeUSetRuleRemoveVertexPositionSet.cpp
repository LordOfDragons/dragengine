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

#include "aeUSetRuleRemoveVertexPositionSet.h"
#include "../../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleRemoveVertexPositionSet
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleRemoveVertexPositionSet::aeUSetRuleRemoveVertexPositionSet( aeRule* rule, const char* pattern ){
	DEASSERT_NOTNULL( rule )
	DEASSERT_NOTNULL( pattern )
	
	pRule = nullptr;
	
	const decStringSet &sets = rule->GetListVertexPositionSets();
	const int setCount = sets.GetCount();
	int i;
	
	for( i=0; i<setCount; i++ ){
		const decString &set = sets.GetAt( i );
		
		if( set.MatchesPattern( pattern ) ){
			pVertexPositionSets.Add( set );
		}
	}
	
	if( pVertexPositionSets.GetCount() == 1 ){
		SetShortInfo( "Remove vertex position set from rule" );
		
	}else{
		SetShortInfo( "Remove vertex position sets from rule" );
	}
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleRemoveVertexPositionSet::~aeUSetRuleRemoveVertexPositionSet(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

bool aeUSetRuleRemoveVertexPositionSet::HasVertexPositionSets() const{
	return pVertexPositionSets.GetCount() > 0;
}

void aeUSetRuleRemoveVertexPositionSet::Undo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->AddVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}

void aeUSetRuleRemoveVertexPositionSet::Redo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->RemoveVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}
