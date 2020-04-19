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

#include "ceUCAASpeakWordSet.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakWordSet
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakWordSet::ceUCAASpeakWordSet( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *word, ceStrip *newStrip ){
	if( ! topic || ! actorSpeak || ! word || ! newStrip ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pWord = NULL;
	pOldStrip = NULL;
	pNewStrip = NULL;
	
	SetShortInfo( "Set Word" );
	
	pOldStrip = new ceStrip( *word );
	
	pNewStrip = newStrip;
	newStrip->AddReference();
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pWord = word;
	word->AddReference();
}

ceUCAASpeakWordSet::~ceUCAASpeakWordSet(){
	if( pNewStrip ){
		pNewStrip->FreeReference();
	}
	if( pOldStrip ){
		pOldStrip->FreeReference();
	}
	if( pWord ){
		pWord->FreeReference();
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

void ceUCAASpeakWordSet::Undo(){
	*pWord = *pOldStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakWordSet::Redo(){
	*pWord = *pNewStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}
