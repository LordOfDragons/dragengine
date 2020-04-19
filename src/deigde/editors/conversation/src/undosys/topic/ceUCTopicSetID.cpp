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

#include "ceUCTopicSetID.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCASnippet.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTopicSetID
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTopicSetID::ceUCTopicSetID( const ceConversation &conversation,
ceConversationTopic *topic, const char *newID ) :
pTopic( NULL ),
pNewID( newID )
{
	if( ! topic || ! topic->GetFile() ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Topic Set ID" );
	
	pOldID = topic->GetID();
	
	const decString &matchGroupID = topic->GetFile()->GetID();
	const decString &matchTopicID = topic->GetID();
	
	const ceConversationFileList &groups = conversation.GetFileList();
	const int groupCount = groups.GetCount();
	ceConversationActionList snippets;
	int i;
	
	for( i=0; i<groupCount; i++ ){
		const ceConversationFile &group = *groups.GetAt( i );
		const ceConversationTopicList &topics = group.GetTopicList();
		const int topicCount = topics.GetCount();
		int j;
		
		for( j=0; j<topicCount; j++ ){
			ceConversationTopic * const actionTopic = topics.GetAt( j );
			pAddSnippets( actionTopic, matchGroupID, matchTopicID, actionTopic->GetActionList() );
		}
	}
	
	pTopic = topic;
	topic->AddReference();
}

ceUCTopicSetID::~ceUCTopicSetID(){
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCTopicSetID::Undo(){
	pSetID( pOldID );
}

void ceUCTopicSetID::Redo(){
	pSetID( pNewID );
}



// Private Functions
//////////////////////

void ceUCTopicSetID::pSetID( const char *id ){
	pTopic->SetID( id );
	
	const int snippetCount = pSnippets.GetCount();
	int i;
	
	for( i=0; i<snippetCount; i++ ){
		const ceUndoCAction &undoCAction = *pSnippets.GetAt( i );
		( ( ceCASnippet* )undoCAction.GetAction() )->SetTopic( id );
		undoCAction.GetTopic()->NotifyActionChanged( undoCAction.GetAction() );
	}
}

void ceUCTopicSetID::pAddSnippets( ceConversationTopic *topic, const char *matchGroupID,
const char *matchTopicID, const ceConversationActionList &actions ){
	const int count = actions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceConversationAction * const action = actions.GetAt( i );
		
		switch( action->GetType() ){
		case ceConversationAction::eatIfElse:{
			const ceCAIfElse &ifElse = *( ( const ceCAIfElse* )action );
			
			const ceCAIfElseCaseList &cases = ifElse.GetCases();
			const int countCases = cases.GetCount();
			int j;
			for( j=0; j<countCases; j++ ){
				pAddSnippets( topic, matchGroupID, matchTopicID, cases.GetAt( j )->GetActions() );
			}
			
			pAddSnippets( topic, matchGroupID, matchTopicID, ifElse.GetElseActions() );
			}break;
			
		case ceConversationAction::eatPlayerChoice:{
			const ceCAPlayerChoice &playerChoice = *( ( const ceCAPlayerChoice* )action );
			const ceCAPlayerChoiceOptionList &options = playerChoice.GetOptions();
			const int optionCount = options.GetCount();
			int j;
			for( j=0; j<optionCount; j++ ){
				pAddSnippets( topic, matchGroupID, matchTopicID, options.GetAt( j )->GetActions() );
			}
			}break;
			
		case ceConversationAction::eatWait:
			pAddSnippets( topic, matchGroupID, matchTopicID, ( ( const ceCAWait* )action )->GetActions() );
			break;
			
		case ceConversationAction::eatSnippet:{
			ceCASnippet * const snippet = ( ceCASnippet* )action;
			if( snippet->GetFile() == matchGroupID && snippet->GetTopic() == matchTopicID ){
				ceUndoCAction *undoCAction = NULL;
				try{
					undoCAction = new ceUndoCAction( action, topic );
					pSnippets.Add( undoCAction );
					undoCAction->FreeReference();
					undoCAction = NULL;
					
				}catch( const deException & ){
					if( undoCAction ){
						undoCAction->FreeReference();
					}
					throw;
				}
			}
			}break;
			
		default:
			break;
		}
	}
}
