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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "saeWord.h"
#include "../saeSAnimation.h"

#include <dragengine/common/exceptions.h>



// Class saeWord
//////////////////

// Constructor, destructor
////////////////////////////

saeWord::saeWord(){
	pSAnimation = NULL;
	
	pName = "word";
	
	pActive = false;
}

saeWord::saeWord( const char *name ) :
pSAnimation( NULL ),
pName( name ),
pActive( false )
{
	if( pName.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
}

saeWord::saeWord( const char *name, const decUnicodeString &phonetics ) :
pSAnimation( NULL ),
pName( name ),
pPhonetics( phonetics ),
pActive( false )
{
	if( pName.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
}

saeWord::~saeWord(){
	SetSAnimation( NULL );
}



// Management
///////////////

void saeWord::SetSAnimation( saeSAnimation *sanimation ){
	pSAnimation = sanimation;
}



void saeWord::SetName( const char *name ){
	if( pName.Equals( name ) ){
		return;
	}
	
	if( pSAnimation && pSAnimation->GetWordList().HasNamed( name ) ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
	
	if( pSAnimation ){
		pSAnimation->NotifyWordNameChanged( this );
	}
}

void saeWord::SetPhonetics( const decUnicodeString &phonetics ){
	if( pPhonetics != phonetics ){
		pPhonetics = phonetics;
		
		if( pSAnimation ){
			pSAnimation->NotifyWordChanged( this );
		}
	}
}

void saeWord::SetActive( bool active ){
	pActive = active;
}
