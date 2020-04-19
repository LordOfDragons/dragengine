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

#include "saeUWordRemove.h"
#include "../../../sanimation/saeSAnimation.h"
#include "../../../sanimation/dictionary/saeWord.h"

#include <dragengine/common/exceptions.h>



// Class saeUWordRemove
/////////////////////////

// Constructor, destructor
////////////////////////////

saeUWordRemove::saeUWordRemove( saeWord *word ){
	if( ! word ) DETHROW( deeInvalidParam );
	
	saeSAnimation *sanimation = word->GetSAnimation();
	if( ! sanimation ) DETHROW( deeInvalidParam );
	
	pSAnimation = NULL;
	pWord = NULL;
	
	SetShortInfo( "Remove Word" );
	
	pSAnimation = sanimation;
	sanimation->AddReference();
	
	pWord = word;
	word->AddReference();
}

saeUWordRemove::~saeUWordRemove(){
	if( pWord ){
		pWord->FreeReference();
	}
	if( pSAnimation ){
		pSAnimation->FreeReference();
	}
}



// Management
///////////////

void saeUWordRemove::Undo(){
	pSAnimation->AddWord( pWord );
}

void saeUWordRemove::Redo(){
	pSAnimation->RemoveWord( pWord );
}
