/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeUOCSpeakerSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSpeakerSetPropertyName
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSpeakerSetPropertyName::gdeUOCSpeakerSetPropertyName( gdeObjectClass *objectClass,
gdeOCSpeaker *speaker, gdeOCSpeaker::eProperties property, const char *newValue ) :
pObjectClass( NULL ),
pSpeaker( NULL ),
pProperty( property )
{
	if( ! objectClass || ! speaker ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Speaker set property name" );
	
	pOldValue = speaker->GetPropertyName( property );
	pNewValue = newValue;
	
	pSpeaker = speaker;
	speaker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSpeakerSetPropertyName::~gdeUOCSpeakerSetPropertyName(){
	if( pSpeaker ){
		pSpeaker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSpeakerSetPropertyName::Undo(){
	pSpeaker->SetPropertyName( pProperty, pOldValue );
	pObjectClass->NotifySpeakerChanged( pSpeaker );
}

void gdeUOCSpeakerSetPropertyName::Redo(){
	pSpeaker->SetPropertyName( pProperty, pNewValue );
	pObjectClass->NotifySpeakerChanged( pSpeaker );
}
