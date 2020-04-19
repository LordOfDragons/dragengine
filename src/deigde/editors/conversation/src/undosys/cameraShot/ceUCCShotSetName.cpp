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

#include "ceUCCShotSetName.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCShotSetName
///////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCShotSetName::ceUCCShotSetName( ceCameraShot *cameraShot, const char *newName ){
	if( ! cameraShot || ! newName ) DETHROW( deeInvalidParam );
	
	const ceConversationFileList &fileList = cameraShot->GetConversation()->GetFileList();
	const int fileCount = fileList.GetCount();
	ceConversationTopic *topic;
	int e, t;
	
	pCameraShot = NULL;
	
	SetShortInfo( "Camera Shot Set Name" );
	
	pOldName = cameraShot->GetName();
	pNewName = newName;
	
	try{
		pCameraShot = cameraShot;
		cameraShot->AddReference();
		
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
		
		if( pCameraShot ){
			pCameraShot->FreeReference();
		}
		
		throw;
	}
}

ceUCCShotSetName::~ceUCCShotSetName(){
	pActionList.RemoveAll();
	
	if( pCameraShot ){
		pCameraShot->FreeReference();
	}
}



// Management
///////////////

void ceUCCShotSetName::Undo(){
	pSetName( pOldName.GetString() );
}

void ceUCCShotSetName::Redo(){
	pSetName( pNewName.GetString() );
}



// Private Functions
//////////////////////

void ceUCCShotSetName::pSetName( const char *name ){
	const int count = pActionList.GetCount();
	ceConversationAction *action;
	int i;
	
	pCameraShot->SetName( name );
	
	for( i=0; i<count; i++ ){
		const ceUndoCAction &undoCAction = *pActionList.GetAt( i );
		
		action = undoCAction.GetAction();
		
		( ( ceCACameraShot* )action )->SetName( name );
		undoCAction.GetTopic()->NotifyActionChanged( action );
	}
}

void ceUCCShotSetName::pAddActions( ceConversationTopic *topic, const ceConversationActionList &list ){
	const int count = list.GetCount();
	ceUndoCAction *undoCAction = NULL;
	ceConversationAction *action;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			action = list.GetAt( i );
			if( action->GetType() == ceConversationAction::eatCameraShot ){
				const ceCACameraShot &cameraShot = *( ( ceCACameraShot* )action );
				
				if( cameraShot.GetName() == pOldName ){
					undoCAction = new ceUndoCAction( action, topic );
					pActionList.Add( undoCAction );
					undoCAction->FreeReference();
					undoCAction = NULL;
				}
				
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
