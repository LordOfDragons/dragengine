/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ceUCFileSetID.h"
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



// Class ceUCFileSetID
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCFileSetID::ceUCFileSetID( const ceConversation &conversation,
ceConversationFile *file, const char *newID ) :
pFile( NULL ),
pNewID( newID )
{
	if( ! file || ! newID ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "File Set ID" );
	
	pOldID = file->GetID();
	
	const decString &matchGroupID = file->GetID();
	
	const ceConversationFileList &groups = conversation.GetFileList();
	const int groupCount = groups.GetCount();
	int i;
	
	for( i=0; i<groupCount; i++ ){
		const ceConversationFile &group = *groups.GetAt( i );
		const ceConversationTopicList &topics = group.GetTopicList();
		const int topicCount = topics.GetCount();
		int j;
		
		for( j=0; j<topicCount; j++ ){
			ceConversationTopic * const actionTopic = topics.GetAt( j );
			pAddSnippets( actionTopic, matchGroupID, actionTopic->GetActionList() );
		}
	}
	
	pFile = file;
	file->AddReference();
}

ceUCFileSetID::~ceUCFileSetID(){
	if( pFile ){
		pFile->FreeReference();
	}
}



// Management
///////////////

void ceUCFileSetID::Undo(){
	pSetID( pOldID );
}

void ceUCFileSetID::Redo(){
	pSetID( pNewID );
}



// Private Functions
//////////////////////

void ceUCFileSetID::pSetID( const char *id ){
	pFile->SetID( id );
	
	const int snippetCount = pSnippets.GetCount();
	int i;
	
	for( i=0; i<snippetCount; i++ ){
		const ceUndoCAction &undoCAction = *pSnippets.GetAt( i );
		( ( ceCASnippet* )undoCAction.GetAction() )->SetFile( id );
		undoCAction.GetTopic()->NotifyActionChanged( undoCAction.GetAction() );
	}
}

void ceUCFileSetID::pAddSnippets( ceConversationTopic *topic, const char *matchGroupID,
const ceConversationActionList &actions ){
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
				pAddSnippets( topic, matchGroupID, cases.GetAt( j )->GetActions() );
			}
			
			pAddSnippets( topic, matchGroupID, ifElse.GetElseActions() );
			}break;
			
		case ceConversationAction::eatPlayerChoice:{
			const ceCAPlayerChoice &playerChoice = *( ( const ceCAPlayerChoice* )action );
			const ceCAPlayerChoiceOptionList &options = playerChoice.GetOptions();
			const int optionCount = options.GetCount();
			int j;
			for( j=0; j<optionCount; j++ ){
				pAddSnippets( topic, matchGroupID, options.GetAt( j )->GetActions() );
			}
			}break;
			
		case ceConversationAction::eatWait:
			pAddSnippets( topic, matchGroupID, ( ( const ceCAWait* )action )->GetActions() );
			break;
			
		case ceConversationAction::eatSnippet:{
			ceCASnippet * const snippet = ( ceCASnippet* )action;
			if( snippet->GetFile() == matchGroupID ){
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
