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

#include "ceUCLookAtSetName.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/lookat/ceLookAt.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCLookAtSetName
////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCLookAtSetName::ceUCLookAtSetName( ceLookAt *lookat, const char *newName ){
	if( ! lookat || ! newName ) DETHROW( deeInvalidParam );
	
	const ceConversationFileList &fileList = lookat->GetConversation()->GetFileList();
	const int fileCount = fileList.GetCount();
	ceConversationTopic *topic;
	int e, t;
	
	pLookAt = NULL;
	
	SetShortInfo( "LookAt Set Name" );
	
	pOldName = lookat->GetName();
	pNewName = newName;
	
	try{
		pLookAt = lookat;
		lookat->AddReference();
		
		for( e=0; e<fileCount; e++ ){
			const ceConversationFile &file = *fileList.GetAt( e );
			const ceConversationTopicList &topicList = file.GetTopicList();
			const int topicCount = topicList.GetCount();
			
			for( t=0; t<topicCount; t++ ){
				topic = topicList.GetAt( t );
				
				pAddActions( topic, topic->GetActionList() );
			}
		}
		
	}catch( const deException & ){
		pActionList.RemoveAll();
		
		if( pLookAt ){
			pLookAt->FreeReference();
		}
		
		throw;
	}
}

ceUCLookAtSetName::~ceUCLookAtSetName(){
	pActionList.RemoveAll();
	
	if( pLookAt ){
		pLookAt->FreeReference();
	}
}



// Management
///////////////

void ceUCLookAtSetName::Undo(){
	pSetName( pNewName.GetString(), pOldName.GetString() );
}

void ceUCLookAtSetName::Redo(){
	pSetName( pOldName.GetString(), pNewName.GetString() );
}



// Private Functions
//////////////////////

void ceUCLookAtSetName::pSetName( const char *oldName, const char *newName ){
	ceConversationAction *action;
	int i, count;
	
	pLookAt->SetName( newName );
	
	count = pActionList.GetCount();
	for( i=0; i<count; i++ ){
		const ceUndoCAction &undoCAction = *pActionList.GetAt( i );
		
		action = undoCAction.GetAction();
		
		if( action->GetType() == ceConversationAction::eatActorSpeak ){
			/*
			ceCAActorSpeak &actionASpeak = *( ( ceCAActorSpeak* )action );
			
			if( actionCShot.GetCameraLookAt() == oldName ){
				actionCShot.SetCameraLookAt( newName );
			}
			*/
		}
		
		undoCAction.GetTopic()->NotifyActionChanged( action );
	}
}

void ceUCLookAtSetName::pAddActions( ceConversationTopic *topic, const ceConversationActionList &list ){
	const int count = list.GetCount();
	ceUndoCAction *undoCAction = NULL;
	ceConversationAction *action;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			action = list.GetAt( i );
			
			if( action->GetType() == ceConversationAction::eatActorSpeak ){
				/*
				const ceCAActorSpeak &actorSpeak = *( ( ceCAActorSpeak* )action );
				
				if( lookat.GetCameraLookAt() == pOldName || lookat.GetLookAtLookAt() == pOldName ){
					undoCAction = new ceUndoCAction( action, topic );
					pActionList.Add( undoCAction );
					undoCAction->FreeReference();
					undoCAction = NULL;
				}
				*/
				
			}else if( action->GetType() == ceConversationAction::eatIfElse ){
				const ceCAIfElse &ifElse = *( ( ceCAIfElse* )action );
				const ceCAIfElseCaseList &caseList = ifElse.GetCases();
				const int count = caseList.GetCount();
				int j;
				
				for( j=0; j<count; j++ ){
					pAddActions( topic, caseList.GetAt( j )->GetActions() );
				}
				pAddActions( topic, ifElse.GetElseActions() );
				
			}else if( action->GetType() == ceConversationAction::eatPlayerChoice ){
				const ceCAPlayerChoice &playerChoice = *( ( ceCAPlayerChoice* )action );
				const ceCAPlayerChoiceOptionList &optionList = playerChoice.GetOptions();
				const int count = optionList.GetCount();
				int j;
				
				for( j=0; j<count; j++ ){
					pAddActions( topic, optionList.GetAt( j )->GetActions() );
				}
				
				pAddActions( topic, playerChoice.GetActions() );
				
			}else if( action->GetType() == ceConversationAction::eatWait ){
				const ceCAWait &wait = *( ( ceCAWait* )action );
				
				pAddActions( topic, wait.GetActions() );
			}
		}
		
	}catch( const deException & ){
		if( undoCAction ){
			undoCAction->FreeReference();
		}
		throw;
	}
}
