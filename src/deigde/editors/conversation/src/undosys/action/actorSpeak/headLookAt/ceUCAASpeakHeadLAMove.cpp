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

#include "ceUCAASpeakHeadLAMove.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakHeadLAMove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakHeadLAMove::ceUCAASpeakHeadLAMove( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak,
ceStrip *headLookAt, int newIndex ){
	if( ! topic || ! actorSpeak || ! headLookAt ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pHeadLA = NULL;
	pNewIndex = newIndex;
	pOldIndex = actorSpeak->GetHeadLookAtList().IndexOf( headLookAt );
	
	if( pOldIndex == -1 ) DETHROW( deeInvalidParam );
	if( pNewIndex < 0 || pNewIndex >= actorSpeak->GetHeadLookAtList().GetCount() ) DETHROW( deeInvalidParam );
	if( pNewIndex == pOldIndex ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Move HeadLookAt" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pHeadLA = headLookAt;
	headLookAt->AddReference();
}

ceUCAASpeakHeadLAMove::~ceUCAASpeakHeadLAMove(){
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

void ceUCAASpeakHeadLAMove::Undo(){
	pActorSpeak->GetHeadLookAtList().MoveTo( pHeadLA, pOldIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakHeadLAMove::Redo(){
	pActorSpeak->GetHeadLookAtList().MoveTo( pHeadLA, pNewIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}
