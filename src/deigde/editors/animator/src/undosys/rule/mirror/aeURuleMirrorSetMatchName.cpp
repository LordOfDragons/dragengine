/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "aeURuleMirrorSetMatchName.h"

#include <dragengine/common/exceptions.h>


// Class aeURuleMirrorSetMatchName
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleMirrorSetMatchName::aeURuleMirrorSetMatchName( aeRuleMirror *rule,
int index, aeRuleMirror::cMatchName *newValue ) :
pRule( rule ),
pIndex( index ),
pNewValue( newValue )
{
	if( ! rule || ! newValue || rule->HasMatchName( newValue ) ){
		DETHROW( deeInvalidParam );
	}
	
	pOldValue = rule->GetMatchNameAt( index );
	
	SetShortInfo( "Mirror rule set match name" );
}

aeURuleMirrorSetMatchName::~aeURuleMirrorSetMatchName(){
}



// Management
///////////////

void aeURuleMirrorSetMatchName::Undo(){
	pRule->SetMatchNameAt( pIndex, pOldValue );
}

void aeURuleMirrorSetMatchName::Redo(){
	pRule->SetMatchNameAt( pIndex, pNewValue );
}
