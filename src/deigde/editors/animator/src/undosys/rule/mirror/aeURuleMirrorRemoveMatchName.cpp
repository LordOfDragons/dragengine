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

#include "aeURuleMirrorRemoveMatchName.h"

#include <dragengine/common/exceptions.h>


// Class aeURuleMirrorRemoveMatchName
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleMirrorRemoveMatchName::aeURuleMirrorRemoveMatchName(
aeRuleMirror *rule, aeRuleMirror::cMatchName *matchName ) :
pRule( rule ),
pMatchName( matchName )
{
	if( ! rule || ! matchName ){
		DETHROW( deeInvalidAction );
	}
	
	pIndex = rule->IndexOfMatchName( matchName );
	if( pIndex == -1 ){
		DETHROW( deeInvalidAction );
	}
	
	SetShortInfo( "Mirror rule remove match name" );
}

aeURuleMirrorRemoveMatchName::~aeURuleMirrorRemoveMatchName(){
}



// Management
///////////////

void aeURuleMirrorRemoveMatchName::Undo(){
	pRule->InsertMatchName( pMatchName, pIndex );
}

void aeURuleMirrorRemoveMatchName::Redo(){
	pRule->RemoveMatchName( pMatchName );
}
