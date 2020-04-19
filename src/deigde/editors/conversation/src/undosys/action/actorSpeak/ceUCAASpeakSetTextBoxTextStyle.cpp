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

#include "ceUCAASpeakSetTextBoxTextStyle.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakSetTextBoxTextStyle
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakSetTextBoxTextStyle::ceUCAASpeakSetTextBoxTextStyle( ceConversationTopic *topic,
ceCAActorSpeak *actorSpeak, const char *newText ){
	if( ! topic ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pOldStyle = actorSpeak->GetTextBoxTextStyle();
	pNewStyle = newText;
	
	SetShortInfo( "Actor speak text box text style" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakSetTextBoxTextStyle::~ceUCAASpeakSetTextBoxTextStyle(){
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakSetTextBoxTextStyle::Undo(){
	pActorSpeak->SetTextBoxTextStyle( pOldStyle );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakSetTextBoxTextStyle::Redo(){
	pActorSpeak->SetTextBoxTextStyle( pNewStyle );
	pTopic->NotifyActionChanged( pActorSpeak );
}
