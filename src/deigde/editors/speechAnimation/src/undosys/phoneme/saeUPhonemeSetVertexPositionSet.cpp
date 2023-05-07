/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeUPhonemeSetVertexPositionSet.h"
#include "../../sanimation/phoneme/saePhoneme.h"

#include <dragengine/common/exceptions.h>


// Class saeUPhonemeSetVertexPositionSet
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

saeUPhonemeSetVertexPositionSet::saeUPhonemeSetVertexPositionSet( saePhoneme *phoneme, const char *newName ) :
pPhoneme( nullptr ),
pNewName( newName )
{
	DEASSERT_NOTNULL( phoneme )
	
	SetShortInfo( "Phoneme Set Move Name" );
	
	pOldName = phoneme->GetVertexPositionSet();
	
	pPhoneme = phoneme;
	phoneme->AddReference();
}

saeUPhonemeSetVertexPositionSet::~saeUPhonemeSetVertexPositionSet(){
	if( pPhoneme ){
		pPhoneme->FreeReference();
	}
}



// Management
///////////////

void saeUPhonemeSetVertexPositionSet::Undo(){
	pPhoneme->SetVertexPositionSet( pOldName );
}

void saeUPhonemeSetVertexPositionSet::Redo(){
	pPhoneme->SetVertexPositionSet( pNewName );
}
