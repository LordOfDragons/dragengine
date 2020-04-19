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

#include "ceUCAASpeakEyesLAFromHeadLA.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakEyesLAFromHeadLA
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakEyesLAFromHeadLA::ceUCAASpeakEyesLAFromHeadLA(
ceConversationTopic *topic, ceCAActorSpeak *actorSpeak ) :
pTopic( NULL ),
pActorSpeak( NULL )
{
	if( ! topic || ! actorSpeak ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Actor speak eyes look-at from head look-at" );
	
	pOldStrips = actorSpeak->GetEyesLookAtList();
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakEyesLAFromHeadLA::~ceUCAASpeakEyesLAFromHeadLA(){
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakEyesLAFromHeadLA::Undo(){
	pActorSpeak->GetEyesLookAtList() = pOldStrips;
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakEyesLAFromHeadLA::Redo(){
	pActorSpeak->GetEyesLookAtList().RemoveAll();
	pActorSpeak->GetEyesLookAtList().AddCopyFrom( pActorSpeak->GetHeadLookAtList() );
	pTopic->NotifyActionChanged( pActorSpeak );
}
