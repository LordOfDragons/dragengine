/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "ceUCAASpeakSetTextBoxTextTranslate.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakSetTextBoxTextTranslate
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakSetTextBoxTextTranslate::ceUCAASpeakSetTextBoxTextTranslate(
ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, const char *newText ) :
pTopic( nullptr ),
pActorSpeak( nullptr ),
pNewText( newText )
{
	DEASSERT_NOTNULL( topic )
	
	pOldText = actorSpeak->GetTextBoxTextTranslate();
	
	SetShortInfo( "Actor speak set text box text translate" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakSetTextBoxTextTranslate::~ceUCAASpeakSetTextBoxTextTranslate(){
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakSetTextBoxTextTranslate::Undo(){
	pActorSpeak->SetTextBoxTextTranslate( pOldText );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakSetTextBoxTextTranslate::Redo(){
	pActorSpeak->SetTextBoxTextTranslate( pNewText );
	pTopic->NotifyActionChanged( pActorSpeak );
}
