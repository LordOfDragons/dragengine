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
#include <stdlib.h>
#include <string.h>

#include "ceConversationTopic.h"
#include "../ceConversation.h"
#include "../actor/ceConversationActor.h"
#include "../actor/speechAnimation/ceSAWordList.h"
#include "../actor/speechAnimation/ceSpeechAnimation.h"
#include "../action/ceConversationAction.h"
#include "../action/ceCAActorSpeak.h"
#include "../action/ceCAIfElse.h"
#include "../action/ceCAIfElseCase.h"
#include "../action/ceCAPlayerChoice.h"
#include "../action/ceCAPlayerChoiceOption.h"
#include "../action/ceCAWait.h"
#include "../file/ceConversationFile.h"
#include "../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>



// Class ceConversationTopic
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationTopic::ceConversationTopic( const char *id ) :
pFile( NULL ),
pID( id ),
pActiveCondition( NULL ){
}

ceConversationTopic::ceConversationTopic( const ceConversationTopic &topic ) :
pFile( NULL ),
pID( topic.pID ),
pActiveCondition( NULL )
{
	const int count = topic.pActions.GetCount();
	ceConversationActionReference action;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			action.TakeOver( topic.pActions.GetAt( i )->CreateCopy() );
			pActions.Add( action );
		}
		
	}catch( const deException & ){
		pActions.RemoveAll();
		throw;
	}
}

ceConversationTopic::~ceConversationTopic(){
	SetActive( NULL, NULL );
	pActions.RemoveAll();
}



// Management
///////////////

void ceConversationTopic::SetFile( ceConversationFile *file ){
	pFile = file;
}

void ceConversationTopic::SetID( const char *id ){
	if( pID == id ){
		return;
	}
	
	if( pFile && pFile->GetTopicList().HasWithID( id ) ){
		DETHROW( deeInvalidParam );
	}
	
	pID = id;
	
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyTopicChanged( pFile, this );
	}
}

void ceConversationTopic::NotifyActionStructureChanged( ceConversationAction *action ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyActionStructureChanged( pFile, this, action );
	}
}

void ceConversationTopic::NotifyActionChanged( ceConversationAction *action ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyActionChanged( pFile, this, action );
	}
}

void ceConversationTopic::SetActive( ceConversationAction *action, ceConversationCondition *condition ){
	if( pActiveAction == action && pActiveCondition == condition ){
		return;
	}
	
	pActiveAction = action;
	pActiveCondition = condition;
	
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyActiveChanged( pFile, this );
	}
}

void ceConversationTopic::FindMissingWords( decStringSet &missingWords ) const{
	FindMissingWords( pActions, missingWords );
}

void ceConversationTopic::FindMissingWords( const ceConversationActionList &actions,
decStringSet &missingWords ) const{
	if( ! pFile || ! pFile->GetConversation() ){
		return;
	}
	
	const ceConversation &conversation = *pFile->GetConversation();
	const ceConversationActorList &actorList = conversation.GetActorList();
	const int count = actions.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		const ceConversationAction &action = *actions.GetAt( i );
		
		switch( action.GetType() ){
		case ceConversationAction::eatActorSpeak:{
			const ceCAActorSpeak &speak = ( ceCAActorSpeak& )action;
			if( ! speak.GetUseSpeechAnimation() ){
				continue;
			}
			
			const ceStripList &words = speak.GetWordList();
			const decString &actorID = speak.GetActor();
			const int wordCount = words.GetCount();
			if( wordCount == 0 ){
				continue;
			}
			
			const ceConversationActor * const conversationActor = actorList.GetWithIDOrAliasID( actorID );
			if( ! conversationActor || ! conversationActor->GetSpeechAnimation() ){
				continue;
			}
			
			const ceSpeechAnimation &speechAnimation = *conversationActor->GetSpeechAnimation();
			const ceSAWordList &saWordList = speechAnimation.GetWordList();
			
			for( j=0; j<wordCount; j++ ){
				const decString &word = words.GetAt( j )->GetID();
				if( ! word.IsEmpty() && ! saWordList.HasNamed( word ) ){
					missingWords.Add( word );
				}
			}
			} break;
			
		case ceConversationAction::eatIfElse:{
			const ceCAIfElse &ifelse = ( ceCAIfElse& )action;
			const ceCAIfElseCaseList &cases = ifelse.GetCases();
			const int caseCount = cases.GetCount();
			
			for( j=0; j<caseCount; j++ ){
				FindMissingWords( cases.GetAt( j )->GetActions(), missingWords );
			}
			
			FindMissingWords( ifelse.GetElseActions(), missingWords );
			} break;
			
		case ceConversationAction::eatPlayerChoice:{
			const ceCAPlayerChoice &playerChoice = ( ceCAPlayerChoice& )action;
			const ceCAPlayerChoiceOptionList &options = playerChoice.GetOptions();
			const int optionCount = options.GetCount();
			
			FindMissingWords( playerChoice.GetActions(), missingWords );
			
			for( j=0; j<optionCount; j++ ){
				FindMissingWords( options.GetAt( j )->GetActions(), missingWords );
			}
			} break;
			
		case ceConversationAction::eatWait:{
			const ceCAWait &wait = ( ceCAWait& )action;
			FindMissingWords( wait.GetActions(), missingWords );
			} break;
			
		default:
			break;
		}
	}
}



void ceConversationTopic::NotifyConditionChanged( ceConversationAction *action,
ceConversationCondition *condition ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyConditionChanged( pFile, this, action, condition );
	}
}

void ceConversationTopic::NotifyConditionStructureChanged( ceConversationAction *action ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyConditionStructureChanged( pFile, this, action );
	}
}
