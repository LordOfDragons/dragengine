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

#include "aeUSetRuleVertexPositionSets.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleVertexPositionSets
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleVertexPositionSets::aeUSetRuleVertexPositionSets( aeRule *rule, const decStringSet &newValue ) :
pRule( nullptr ),
pNewValue( newValue )
{
	DEASSERT_NOTNULL( rule )
	
	pOldValue = rule->GetListVertexPositionSets();
	SetShortInfo( "Set rule vertex position sets" );
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleVertexPositionSets::~aeUSetRuleVertexPositionSets(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeUSetRuleVertexPositionSets::Undo(){
	pRule->SetListVertexPositionSets( pOldValue );
}

void aeUSetRuleVertexPositionSets::Redo(){
	pRule->SetListVertexPositionSets( pNewValue );
}
