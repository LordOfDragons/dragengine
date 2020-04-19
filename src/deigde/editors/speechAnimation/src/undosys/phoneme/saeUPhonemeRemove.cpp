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

#include "saeUPhonemeRemove.h"
#include "../../sanimation/saeSAnimation.h"
#include "../../sanimation/phoneme/saePhoneme.h"

#include <dragengine/common/exceptions.h>



// Class saeUPhonemeRemove
////////////////////////////

// Constructor, destructor
////////////////////////////

saeUPhonemeRemove::saeUPhonemeRemove( saePhoneme *phoneme ){
	if( ! phoneme ) DETHROW( deeInvalidParam );
	
	saeSAnimation *sanimation = phoneme->GetSAnimation();
	if( ! sanimation ) DETHROW( deeInvalidParam );
	
	pSAnimation = NULL;
	pPhoneme = NULL;
	
	SetShortInfo( "Remove Phoneme" );
	
	pSAnimation = sanimation;
	sanimation->AddReference();
	
	pPhoneme = phoneme;
	phoneme->AddReference();
}

saeUPhonemeRemove::~saeUPhonemeRemove(){
	if( pPhoneme ){
		pPhoneme->FreeReference();
	}
	if( pSAnimation ){
		pSAnimation->FreeReference();
	}
}



// Management
///////////////

void saeUPhonemeRemove::Undo(){
	pSAnimation->AddPhoneme( pPhoneme );
}

void saeUPhonemeRemove::Redo(){
	pSAnimation->RemovePhoneme( pPhoneme );
}
