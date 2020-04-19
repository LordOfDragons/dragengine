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

#include "ceUCAASpeakHeadLAAdd.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakHeadLAAdd
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakHeadLAAdd::ceUCAASpeakHeadLAAdd( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *headLookAt, int index ){
	if( ! topic || ! actorSpeak || ! headLookAt ){
		DETHROW( deeInvalidParam );
	}
	if( index < 0 || index > actorSpeak->GetHeadLookAtList().GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pHeadLA = NULL;
	pIndex = index;
	
	SetShortInfo( "Actor Speak Add HeadLookAt" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pHeadLA = headLookAt;
	headLookAt->AddReference();
}

ceUCAASpeakHeadLAAdd::~ceUCAASpeakHeadLAAdd(){
	if( pHeadLA ){
		pHeadLA->FreeReference();
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

void ceUCAASpeakHeadLAAdd::Undo(){
	pActorSpeak->GetHeadLookAtList().Remove( pHeadLA );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakHeadLAAdd::Redo(){
	pActorSpeak->GetHeadLookAtList().InsertAt( pHeadLA, pIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}
