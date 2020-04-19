/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceUCAASpeakGestureSet.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakGestureSet
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakGestureSet::ceUCAASpeakGestureSet( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *gesture, ceStrip *newStrip ){
	if( ! topic || ! actorSpeak || ! gesture || ! newStrip ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pGesture = NULL;
	pOldStrip = NULL;
	pNewStrip = NULL;
	
	SetShortInfo( "Set gesture" );
	
	pOldStrip = new ceStrip( *gesture );
	
	pNewStrip = newStrip;
	newStrip->AddReference();
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pGesture = gesture;
	gesture->AddReference();
}

ceUCAASpeakGestureSet::~ceUCAASpeakGestureSet(){
	if( pNewStrip ){
		pNewStrip->FreeReference();
	}
	if( pOldStrip ){
		pOldStrip->FreeReference();
	}
	if( pGesture ){
		pGesture->FreeReference();
	}
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakGestureSet::Undo(){
	*pGesture = *pOldStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakGestureSet::Redo(){
	*pGesture = *pNewStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}
