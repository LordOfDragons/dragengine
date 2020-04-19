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

#include "ceUCAASpeakSetActor.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakSetActor
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakSetActor::ceUCAASpeakSetActor( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, const char *newActorID ){
	if( ! topic || ! newActorID ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pOldActorID = actorSpeak->GetActor();
	pNewActorID = newActorID;
	
	SetShortInfo( "Actor Speak Set Actor ID" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakSetActor::~ceUCAASpeakSetActor(){
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakSetActor::Undo(){
	pActorSpeak->SetActor( pOldActorID.GetString() );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakSetActor::Redo(){
	pActorSpeak->SetActor( pNewActorID.GetString() );
	pTopic->NotifyActionChanged( pActorSpeak );
}
