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

#include "ceWPTTIMAction.h"
#include "ceWPTTIMActions.h"
#include "ceWPTTIMACameraShot.h"
#include "ceWPTTIMAMusic.h"
#include "ceWPTTIMAActorSpeak.h"
#include "ceWPTTIMAStopConversation.h"
#include "ceWPTTIMAStopTopic.h"
#include "ceWPTTIMASnippet.h"
#include "ceWPTTIMASetVariable.h"
#include "ceWPTTIMASetActorParameter.h"
#include "ceWPTTIMAActorCommand.h"
#include "ceWPTTIMAGameCommand.h"
#include "ceWPTTIMATrigger.h"
#include "ceWPTTIMAActorAdd.h"
#include "ceWPTTIMAActorRemove.h"
#include "ceWPTTIMACoordSystemAdd.h"
#include "ceWPTTIMACoordSystemRemove.h"
#include "ceWPTTIMAComment.h"
#include "ifelse/ceWPTTIMAIfElse.h"
#include "playerChoice/ceWPTTIMAPlayerChoice.h"
#include "wait/ceWPTTIMAWait.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceConversationAction.h"
#include "../../../../conversation/action/ceConversationActionList.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/playback/cePlayback.h"
#include "../../../../conversation/playback/cePlaybackActionStackEntry.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMActions::ceWPTTIMActions( ceWindowMain &windowMain, ceConversation &conversation,
eTypes type, const ceConversationActionList &actions ) :
ceWPTTreeItemModel( windowMain, conversation, type ),
pActions( actions ){
}

ceWPTTIMActions::~ceWPTTIMActions(){
}



// Management
///////////////

ceWPTTIMAction *ceWPTTIMActions::GetChildWith( ceConversationAction *action ) const{
	const int count = GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTIMAction * const child = ( ceWPTTIMAction* )GetChildAt( i );
		if( child->GetAction() == action ){
			return child;
		}
	}
	
	return NULL;
}

void ceWPTTIMActions::StructureChanged(){
	Update();
}



void ceWPTTIMActions::Update(){
	UpdateActions();
}

void ceWPTTIMActions::UpdateActions(){
	const int count = pActions.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		ceConversationAction * const action = pActions.GetAt( i );
		
		// find item matching action if present
		ceWPTTIMAction *model = NULL;
		const int childCount = GetChildCount();
		for( j=i; j<childCount; j++ ){
			ceWPTTIMAction * const child = ( ceWPTTIMAction* )GetChildAt( j );
			if( child->GetAction() == action ){
				model = child;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if( model ){
			if( j != i ){
				MoveChild( j, i );
			}
			model->Update();
			
		}else{
			model = CreateActionModel( GetWindowMain(), GetConversation(), action );
			
			try{
				InsertChild( model, i );
				model->Update();
				
			}catch( const deException & ){
				model->FreeReference();
				throw;
			}
			
			model->FreeReference();
		}
	}
	
	// remove non-matching nodes
	while( GetChildCount() > count ){
		RemoveChild( GetChildAt( GetChildCount() - 1 ) );
	}
}

void ceWPTTIMActions::BuildPlaybackFromHere() const{
	ceWPTTreeItemModel::BuildPlaybackFromHere();
	
	if( pActions.GetCount() == 0 ){
		GetConversation().GetPlayback()->AdvanceToNextAction();
		
	}else{
		GetConversation().GetPlayback()->GetActionStack().Push(
			NULL, GetOwnerAction(), &pActions, 0 );
	}
}



ceWPTTIMAction *ceWPTTIMActions::CreateActionModel( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction *action ){
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	
	switch( action->GetType() ){
	case ceConversationAction::eatCameraShot:
		return new ceWPTTIMACameraShot( windowMain, conversation, ( ceCACameraShot* )action );
		
	case ceConversationAction::eatMusic:
		return new ceWPTTIMAMusic( windowMain, conversation, ( ceCAMusic* )action );
		
	case ceConversationAction::eatActorSpeak:
		return new ceWPTTIMAActorSpeak( windowMain, conversation, ( ceCAActorSpeak* )action );
		
	case ceConversationAction::eatIfElse:
		return new ceWPTTIMAIfElse( windowMain, conversation, ( ceCAIfElse* )action );
		
	case ceConversationAction::eatPlayerChoice:
		return new ceWPTTIMAPlayerChoice( windowMain, conversation, ( ceCAPlayerChoice* )action );
		
	case ceConversationAction::eatStopConversation:
		return new ceWPTTIMAStopConversation( windowMain, conversation, ( ceCAStopConversation* )action );
		
	case ceConversationAction::eatStopTopic:
		return new ceWPTTIMAStopTopic( windowMain, conversation, ( ceCAStopTopic* )action );
		
	case ceConversationAction::eatSnippet:
		return new ceWPTTIMASnippet( windowMain, conversation, ( ceCASnippet* )action );
		
	case ceConversationAction::eatSetVariable:
		return new ceWPTTIMASetVariable( windowMain, conversation, ( ceCASetVariable* )action );
		
	case ceConversationAction::eatSetActorParameter:
		return new ceWPTTIMASetActorParameter( windowMain, conversation, ( ceCASetActorParameter* )action );
		
	case ceConversationAction::eatActorCommand:
		return new ceWPTTIMAActorCommand( windowMain, conversation, ( ceCAActorCommand* )action );
		
	case ceConversationAction::eatGameCommand:
		return new ceWPTTIMAGameCommand( windowMain, conversation, ( ceCAGameCommand* )action );
		
	case ceConversationAction::eatWait:
		return new ceWPTTIMAWait( windowMain, conversation, ( ceCAWait* )action );
		
	case ceConversationAction::eatTrigger:
		return new ceWPTTIMATrigger( windowMain, conversation, ( ceCATrigger* )action );
		
	case ceConversationAction::eatActorAdd:
		return new ceWPTTIMAActorAdd( windowMain, conversation, ( ceCAActorAdd* )action );
		
	case ceConversationAction::eatActorRemove:
		return new ceWPTTIMAActorRemove( windowMain, conversation, ( ceCAActorRemove* )action );
		
	case ceConversationAction::eatCoordSystemAdd:
		return new ceWPTTIMACoordSystemAdd( windowMain, conversation, ( ceCACoordSystemAdd* )action );
		
	case ceConversationAction::eatCoordSystemRemove:
		return new ceWPTTIMACoordSystemRemove( windowMain, conversation, ( ceCACoordSystemRemove* )action );
		
	case ceConversationAction::eatComment:
		return new ceWPTTIMAComment( windowMain, conversation, ( ceCAComment* )action );
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	DETHROW( deeInvalidParam );
}
