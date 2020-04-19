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

#include "ceUCACommentSetComment.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAComment.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCACommentSetComment
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCACommentSetComment::ceUCACommentSetComment( ceConversationTopic *topic, ceCAComment *action, const char *newComment ){
	if( ! topic || ! newComment ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pComment = NULL;
	pOldComment = action->GetComment();
	pNewComment = newComment;
	
	SetShortInfo( "Comment set comment" );
	
	pTopic = topic;
	topic->AddReference();
	
	pComment = action;
	action->AddReference();
}

ceUCACommentSetComment::~ceUCACommentSetComment(){
	if( pComment ){
		pComment->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCACommentSetComment::Undo(){
	pComment->SetComment( pOldComment.GetString() );
	pTopic->NotifyActionChanged( pComment );
}

void ceUCACommentSetComment::Redo(){
	pComment->SetComment( pNewComment.GetString() );
	pTopic->NotifyActionChanged( pComment );
}
