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

#include "aeUSetRuleAddVertexPositionSet.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>



// Class aeUSetRuleAddVertexPositionSet
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleAddVertexPositionSet::aeUSetRuleAddVertexPositionSet( aeRule *rule, const char *pattern ){
	DEASSERT_NOTNULL( rule )
	DEASSERT_NOTNULL( pattern )
	
	pRule = nullptr;
	
	const decStringSet &sets = rule->GetListVertexPositionSets();
	const deComponent * const engComponent = rule->GetAnimator()->GetEngineComponent();
	const deModel * const engModel = engComponent ? engComponent->GetModel() : nullptr;
	
	if( engModel ){
		const int setCount = engModel->GetVertexPositionSetCount();
		int i;
		
		for( i=0; i<setCount; i++ ){
			const decString &set = engModel->GetVertexPositionSetAt( i )->GetName();
			
			if( set.MatchesPattern( pattern ) && ! sets.Has( set ) ){
				pVertexPositionSets.Add( set );
			}
		}
		
	}else{
		if( ! sets.Has( pattern ) ){
			pVertexPositionSets.Add( pattern );
		}
	}
	
	if( pVertexPositionSets.GetCount() == 1 ){
		SetShortInfo( "Add vertex position set to rule" );
		
	}else{
		SetShortInfo( "Add vertex position sets to rule" );
	}
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleAddVertexPositionSet::~aeUSetRuleAddVertexPositionSet(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeUSetRuleAddVertexPositionSet::Undo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->RemoveVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}

void aeUSetRuleAddVertexPositionSet::Redo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->AddVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}
