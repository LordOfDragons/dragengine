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

#include "aeURuleMirrorRemoveAllMatchNames.h"

#include <dragengine/common/exceptions.h>


// Class aeURuleMirrorRemoveAllMatchNames
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleMirrorRemoveAllMatchNames::aeURuleMirrorRemoveAllMatchNames( aeRuleMirror *rule ) :
pRule( rule )
{
	if( ! rule || rule->GetMatchNameCount() == 0 ){
		DETHROW( deeInvalidAction );
	}
	
	const int count = rule->GetMatchNameCount();
	int i;
	for( i=0; i<count; i++ ){
		pMatchNames.Add( rule->GetMatchNameAt( i ) );
	}
	
	SetShortInfo( "Mirror rule remove all match names" );
}

aeURuleMirrorRemoveAllMatchNames::~aeURuleMirrorRemoveAllMatchNames(){
}



// Management
///////////////

void aeURuleMirrorRemoveAllMatchNames::Undo(){
	const int count = pMatchNames.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pRule->AddMatchName( ( aeRuleMirror::cMatchName* )pMatchNames.GetAt( i ) );
	}
}

void aeURuleMirrorRemoveAllMatchNames::Redo(){
	pRule->RemoveAllMatchNames();
}
