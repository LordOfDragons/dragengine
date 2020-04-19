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

#include "ceUCAASpeakFaceMove.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakFaceMove
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakFaceMove::ceUCAASpeakFaceMove( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak,
ceStrip *facePose, int newIndex ){
	if( ! topic || ! actorSpeak || ! facePose ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pFacePose = NULL;
	pNewIndex = newIndex;
	pOldIndex = actorSpeak->GetFacePoseList().IndexOf( facePose );
	
	if( pOldIndex == -1 ) DETHROW( deeInvalidParam );
	if( pNewIndex < 0 || pNewIndex >= actorSpeak->GetFacePoseList().GetCount() ) DETHROW( deeInvalidParam );
	if( pNewIndex == pOldIndex ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Move Face Pose" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
	
	pFacePose = facePose;
	facePose->AddReference();
}

ceUCAASpeakFaceMove::~ceUCAASpeakFaceMove(){
	if( pFacePose ){
		pFacePose->FreeReference();
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

void ceUCAASpeakFaceMove::Undo(){
	pActorSpeak->GetFacePoseList().MoveTo( pFacePose, pOldIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakFaceMove::Redo(){
	pActorSpeak->GetFacePoseList().MoveTo( pFacePose, pNewIndex );
	pTopic->NotifyActionChanged( pActorSpeak );
}
