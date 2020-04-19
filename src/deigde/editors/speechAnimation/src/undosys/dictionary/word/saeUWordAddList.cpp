/* 
 * Drag[en]gine IGDE Speech Animation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "saeUWordAddList.h"
#include "../../../sanimation/saeSAnimation.h"
#include "../../../sanimation/dictionary/saeWord.h"

#include <dragengine/common/exceptions.h>



// Class saeUWordAddList
//////////////////////////

// Constructor, destructor
////////////////////////////

saeUWordAddList::saeUWordAddList( saeSAnimation *sanimation, const saeWordList &words ) :
pSAnimation( NULL ),
pWords( words )
{
	if( ! sanimation || words.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = words.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( sanimation->GetWordList().Has( words.GetAt( i ) ) ){
			DETHROW( deeInvalidParam );
		}
	}
	
	SetShortInfo( "Add Word" );
	
	pSAnimation = sanimation;
	sanimation->AddReference();
}

saeUWordAddList::~saeUWordAddList(){
	if( pSAnimation ){
		pSAnimation->FreeReference();
	}
}



// Management
///////////////

void saeUWordAddList::Undo(){
	const int count = pWords.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pSAnimation->RemoveWord( pWords.GetAt( i ) );
	}
}

void saeUWordAddList::Redo(){
	const int count = pWords.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pSAnimation->AddWord( pWords.GetAt( i ) );
	}
	pSAnimation->SetActiveWord( pWords.GetAt( count - 1 ) );
}
