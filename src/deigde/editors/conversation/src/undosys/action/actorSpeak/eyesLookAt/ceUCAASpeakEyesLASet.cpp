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

#include "ceUCAASpeakEyesLASet.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakEyesLASet
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakEyesLASet::ceUCAASpeakEyesLASet( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *eyesLookAt, ceStrip *newStrip ){
	if( ! topic || ! actorSpeak || ! eyesLookAt || ! newStrip ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pEyesLA = NULL;
	pOldStrip = NULL;
	pNewStrip = NULL;
	
	SetShortInfo( "Set eyes look-at" );
	
	pOldStrip = new ceStrip( *eyesLookAt );
	
	pNewStrip = newStrip;
	newStrip->AddReference();
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pEyesLA = eyesLookAt;
	eyesLookAt->AddReference();
}

ceUCAASpeakEyesLASet::~ceUCAASpeakEyesLASet(){
	if( pNewStrip ){
		pNewStrip->FreeReference();
	}
	if( pOldStrip ){
		pOldStrip->FreeReference();
	}
	if( pEyesLA ){
		pEyesLA->FreeReference();
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

void ceUCAASpeakEyesLASet::Undo(){
	*pEyesLA = *pOldStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakEyesLASet::Redo(){
	*pEyesLA = *pNewStrip;
	pTopic->NotifyActionChanged( pActorSpeak );
}
