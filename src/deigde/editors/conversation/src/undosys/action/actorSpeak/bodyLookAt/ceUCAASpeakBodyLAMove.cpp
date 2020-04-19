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

#include "ceUCAASpeakBodyLAMove.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakBodyLAMove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakBodyLAMove::ceUCAASpeakBodyLAMove( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak,
ceStrip *bodyLookAt, int newIndex ){
	if( ! topic || ! actorSpeak || ! bodyLookAt ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pBodyLA = NULL;
	pNewIndex = newIndex;
	pOldIndex = actorSpeak->GetBodyLookAtList().IndexOf( bodyLookAt );
	
	if( pOldIndex == -1 ) DETHROW( deeInvalidParam );
	if( pNewIndex < 0 || pNewIndex >= actorSpeak->GetBodyLookAtList().GetCount() ) DETHROW( deeInvalidParam );
	if( pNewIndex == pOldIndex ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Move BodyLookAt" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pBodyLA = bodyLookAt;
	bodyLookAt->AddReference();
}

ceUCAASpeakBodyLAMove::~ceUCAASpeakBodyLAMove(){
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

void ceUCAASpeakBodyLAMove::Undo(){
	pActorSpeak->GetBodyLookAtList().MoveTo( pBodyLA, pOldIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakBodyLAMove::Redo(){
	pActorSpeak->GetBodyLookAtList().MoveTo( pBodyLA, pNewIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}
