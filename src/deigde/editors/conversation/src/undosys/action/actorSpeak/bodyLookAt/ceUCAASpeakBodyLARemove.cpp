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

#include "ceUCAASpeakBodyLARemove.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakBodyLARemove
//////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakBodyLARemove::ceUCAASpeakBodyLARemove( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *bodyLookAt ){
	if( ! topic || ! actorSpeak || ! bodyLookAt ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pBodyLA = NULL;
	pIndex = actorSpeak->GetBodyLookAtList().IndexOf( bodyLookAt );
	
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove BodyLookAt" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pBodyLA = bodyLookAt;
	bodyLookAt->AddReference();
}

ceUCAASpeakBodyLARemove::~ceUCAASpeakBodyLARemove(){
	if( pBodyLA ){
		pBodyLA->FreeReference();
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

void ceUCAASpeakBodyLARemove::Undo(){
	pActorSpeak->GetBodyLookAtList().InsertAt( pBodyLA, pIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakBodyLARemove::Redo(){
	pActorSpeak->GetBodyLookAtList().Remove( pBodyLA );
	pTopic->NotifyActionChanged( pActorSpeak );
}
