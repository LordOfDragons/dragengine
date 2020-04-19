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

#include "gdeUOCSpeakerSetTriggerName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSpeakerSetTriggerName
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSpeakerSetTriggerName::gdeUOCSpeakerSetTriggerName( gdeObjectClass *objectClass,
gdeOCSpeaker *speaker, gdeOCSpeaker::eTriggers trigger, const char *newValue ) :
pObjectClass( NULL ),
pSpeaker( NULL ),
pTrigger( trigger )
{
	if( ! objectClass || ! speaker ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Speaker set trigger name" );
	
	pOldValue = speaker->GetTriggerName( trigger );
	pNewValue = newValue;
	
	pSpeaker = speaker;
	speaker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSpeakerSetTriggerName::~gdeUOCSpeakerSetTriggerName(){
	if( pSpeaker ){
		pSpeaker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSpeakerSetTriggerName::Undo(){
	pSpeaker->SetTriggerName( pTrigger, pOldValue );
	pObjectClass->NotifySpeakerChanged( pSpeaker );
}

void gdeUOCSpeakerSetTriggerName::Redo(){
	pSpeaker->SetTriggerName( pTrigger, pNewValue );
	pObjectClass->NotifySpeakerChanged( pSpeaker );
}
