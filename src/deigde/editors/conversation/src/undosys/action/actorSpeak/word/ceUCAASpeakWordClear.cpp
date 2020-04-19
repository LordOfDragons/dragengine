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

#include "ceUCAASpeakWordClear.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakWordClear
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakWordClear::ceUCAASpeakWordClear( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak ) :
pTopic( NULL ),
pActorSpeak( NULL )
{
	if( ! topic || ! actorSpeak ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Actor speak clear words" );
	
	pOldWords = actorSpeak->GetWordList();
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakWordClear::~ceUCAASpeakWordClear(){
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakWordClear::Undo(){
	pActorSpeak->GetWordList() = pOldWords;
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakWordClear::Redo(){
	pActorSpeak->GetWordList().RemoveAll();
	pTopic->NotifyActionChanged( pActorSpeak );
}
