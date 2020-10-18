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

#include "ceUCTargetSetName.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/target/ceTarget.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTargetSetName
////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTargetSetName::ceUCTargetSetName( ceTarget *target, const char *newName ){
	if( ! target || ! newName ) DETHROW( deeInvalidParam );
	
	const ceCameraShotList &cameraShotList = target->GetConversation()->GetCameraShotList();
	const ceConversationFileList &fileList = target->GetConversation()->GetFileList();
	const int cameraShotCount = cameraShotList.GetCount();
	const int fileCount = fileList.GetCount();
	ceConversationTopic *topic;
	int i, j;
	
	pTarget = NULL;
	
	SetShortInfo( "Target Set Name" );
	
	pOldName = target->GetName();
	pNewName = newName;
	
	try{
		pTarget = target;
		target->AddReference();
		
		for( i=0; i<fileCount; i++ ){
			const ceConversationFile &file = *fileList.GetAt( i );
			const ceConversationTopicList &topicList = file.GetTopicList();
			const int topicCount = topicList.GetCount();
			
			for( j=0; j<topicCount; j++ ){
				topic = topicList.GetAt( j );
				
				pAddActions( topic, topic->GetActionList() );
			}
		}
		
		for( i=0; i<cameraShotCount; i++ ){
			ceCameraShot * const cameraShot = cameraShotList.GetAt( i );
			
			if( cameraShot->GetCameraTarget() == pOldName || cameraShot->GetLookAtTarget() == pOldName ){
				pCameraShotList.Add( cameraShot );
			}
		}
		
	}catch( const deException & ){
		pActionList.RemoveAll();
		
		if( pTarget ){
			pTarget->FreeReference();
		}
		
		throw;
	}
}

ceUCTargetSetName::~ceUCTargetSetName(){
	pCameraShotList.RemoveAll();
	pActionList.RemoveAll();
	
	if( pTarget ){
		pTarget->FreeReference();
	}
}



// Management
///////////////

void ceUCTargetSetName::Undo(){
	pSetName( pNewName, pOldName );
}

void ceUCTargetSetName::Redo(){
	pSetName( pOldName, pNewName );
}



// Private Functions
//////////////////////

void ceUCTargetSetName::pSetName( const char *oldName, const char *newName ){
	ceConversationAction *action;
	int i, count;
	
	pTarget->SetName( newName );
	
	count = pActionList.GetCount();
	for( i=0; i<count; i++ ){
		const ceUndoCAction &undoCAction = *pActionList.GetAt( i );
		
		action = undoCAction.GetAction();
		
		if( action->GetType() == ceConversationAction::eatCameraShot ){
			ceCACameraShot &actionCShot = *( ( ceCACameraShot* )action );
			
			if( actionCShot.GetCameraTarget() == oldName ){
				actionCShot.SetCameraTarget( newName );
			}
			if( actionCShot.GetLookAtTarget() == oldName ){
				actionCShot.SetLookAtTarget( newName );
			}
		}
		
		undoCAction.GetTopic()->NotifyActionChanged( action );
	}
	
	count = pCameraShotList.GetCount();
	for( i=0; i<count; i++ ){
		ceCameraShot &cameraShot = *pCameraShotList.GetAt( i );
		
		if( cameraShot.GetCameraTarget() == oldName ){
			cameraShot.SetCameraTarget( newName );
		}
		if( cameraShot.GetLookAtTarget() == oldName ){
			cameraShot.SetLookAtTarget( newName );
		}
	}
}

void ceUCTargetSetName::pAddActions( ceConversationTopic *topic, const ceConversationActionList &list ){
	const int count = list.GetCount();
	ceUndoCAction *undoCAction = NULL;
	ceConversationAction *action;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			action = list.GetAt( i );
			
			if( action->GetType() == ceConversationAction::eatCameraShot ){
				const ceCACameraShot &target = *( ( ceCACameraShot* )action );
				
				if( target.GetCameraTarget() == pOldName || target.GetLookAtTarget() == pOldName ){
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
