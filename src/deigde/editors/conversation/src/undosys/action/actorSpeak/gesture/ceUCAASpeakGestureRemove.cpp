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

#include "ceUCAASpeakGestureRemove.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakGestureRemove
///////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakGestureRemove::ceUCAASpeakGestureRemove( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *gesture ){
	if( ! topic || ! actorSpeak || ! gesture ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pGesture = NULL;
	pIndex = actorSpeak->GetGestureList().IndexOf( gesture );
	
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove Gesture" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pGesture = gesture;
	gesture->AddReference();
}

ceUCAASpeakGestureRemove::~ceUCAASpeakGestureRemove(){
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

void ceUCAASpeakGestureRemove::Undo(){
	pActorSpeak->GetGestureList().InsertAt( pGesture, pIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakGestureRemove::Redo(){
	pActorSpeak->GetGestureList().Remove( pGesture );
	pTopic->NotifyActionChanged( pActorSpeak );
}
