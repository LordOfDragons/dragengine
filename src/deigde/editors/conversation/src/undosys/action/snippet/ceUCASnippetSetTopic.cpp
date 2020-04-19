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

#include "ceUCASnippetSetTopic.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCASnippet.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCASnippetSetTopic
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCASnippetSetTopic::ceUCASnippetSetTopic( ceConversationTopic *topic, ceCASnippet *snippet, const char *newTopic ){
	if( ! topic || ! newTopic ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pSnippet = NULL;
	pOldTopic = snippet->GetTopic();
	pNewTopic = newTopic;
	
	SetShortInfo( "Snippet Set Topic" );
	
	pTopic = topic;
	topic->AddReference();
	
	pSnippet = snippet;
	snippet->AddReference();
}

ceUCASnippetSetTopic::~ceUCASnippetSetTopic(){
	if( pSnippet ){
		pSnippet->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCASnippetSetTopic::Undo(){
	pSnippet->SetTopic( pOldTopic.GetString() );
	pTopic->NotifyActionChanged( pSnippet );
}

void ceUCASnippetSetTopic::Redo(){
	pSnippet->SetTopic( pNewTopic.GetString() );
	pTopic->NotifyActionChanged( pSnippet );
}
