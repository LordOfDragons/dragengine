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

#include "ceWPTMACreateAction.h"
#include "../../action/ceWPTTIMAction.h"
#include "../../ceWPTTreeModel.h"
#include "../../../ceWPTopic.h"
#include "../../../ceWindowProperties.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceConversationActionReference.h"
#include "../../../../../conversation/action/ceCAActorAdd.h"
#include "../../../../../conversation/action/ceCAActorCommand.h"
#include "../../../../../conversation/action/ceCAActorRemove.h"
#include "../../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../../conversation/action/ceCACameraShot.h"
#include "../../../../../conversation/action/ceCAComment.h"
#include "../../../../../conversation/action/ceCACoordSystemAdd.h"
#include "../../../../../conversation/action/ceCACoordSystemRemove.h"
#include "../../../../../conversation/action/ceCAGameCommand.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../conversation/action/ceCAMusic.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../../../conversation/action/ceCASetActorParameter.h"
#include "../../../../../conversation/action/ceCASetVariable.h"
#include "../../../../../conversation/action/ceCASnippet.h"
#include "../../../../../conversation/action/ceCAStopConversation.h"
#include "../../../../../conversation/action/ceCAStopTopic.h"
#include "../../../../../conversation/action/ceCATrigger.h"
#include "../../../../../conversation/action/ceCAWait.h"

#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMACreateAction::ceWPTMACreateAction( ceWindowMain &windowMain,
ceConversation &conversation,
ceConversationAction::eActionTypes actionType ) :
ceWPTMenuAction( windowMain,
	ActionTypeText( windowMain, actionType ),
	ActionTypeIcon( windowMain, actionType ) ),
pConversation( &conversation ),
pActionType( actionType ){
}



// Management
///////////////

void ceWPTMACreateAction::OnAction(){
	ceConversationActionReference action;
	action.TakeOver( CreateAction() );
	
	igdeUndoReference undo;
	undo.TakeOver( CreateUndo( action ) );
	GetConversation().GetUndoSystem()->Add( undo );
}

igdeUndo *ceWPTMACreateAction::CreateUndo( ceConversationAction* ){
	// only not pure-virtual because FOX toolkit requires final classes. if the system
	// moves over to the IGDE ToolKit this will become a pure virtual again
	DETHROW( deeInvalidParam );
}



ceConversationAction *ceWPTMACreateAction::CreateAction(){
	switch( pActionType ){
	case ceConversationAction::eatCameraShot:
		return new ceCACameraShot;
		
	case ceConversationAction::eatMusic:
		return new ceCAMusic;
		
	case ceConversationAction::eatActorSpeak:
		return new ceCAActorSpeak( pConversation->GetEngine() );
		
	case ceConversationAction::eatSetVariable:
		return new ceCASetVariable;
		
	case ceConversationAction::eatSetActorParameter:
		return new ceCASetActorParameter;
		
	case ceConversationAction::eatTrigger:
		return new ceCATrigger;
		
	case ceConversationAction::eatIfElse:{
		ceCAIfElse *ifElse = NULL;
		ceCAIfElseCase *ifCase = NULL;
		
		try{
			ifElse = new ceCAIfElse;
			ifCase = new ceCAIfElseCase;
			ifElse->GetCases().Add( ifCase );
			ifCase->FreeReference();
			
		}catch( const deException & ){
			if( ifCase ){
				ifCase->FreeReference();
			}
			if( ifElse ){
				ifElse->FreeReference();
			}
			throw;
		}
		
		return ifElse;
		}
		
	case ceConversationAction::eatPlayerChoice:{
		ceCAPlayerChoice *playerChoice = NULL;
		ceCAPlayerChoiceOption *option = NULL;
		
		try{
			playerChoice = new ceCAPlayerChoice;
			option = new ceCAPlayerChoiceOption;
			playerChoice->GetOptions().Add( option );
			option->FreeReference();
			
		}catch( const deException & ){
			if( option ){
				option->FreeReference();
			}
			if( playerChoice ){
				playerChoice->FreeReference();
			}
			throw;
		}
		
		return playerChoice;
		}
		
	case ceConversationAction::eatWait:
		return new ceCAWait;
		
	case ceConversationAction::eatSnippet:
		return new ceCASnippet;
		
	case ceConversationAction::eatStopConversation:
		return new ceCAStopConversation;
		
	case ceConversationAction::eatStopTopic:
		return new ceCAStopTopic;
		
	case ceConversationAction::eatActorCommand:
		return new ceCAActorCommand;
		
	case ceConversationAction::eatGameCommand:
		return new ceCAGameCommand;
		
	case ceConversationAction::eatActorAdd:
		return new ceCAActorAdd;
		
	case ceConversationAction::eatActorRemove:
		return new ceCAActorRemove;
		
	case ceConversationAction::eatCoordSystemAdd:
		return new ceCACoordSystemAdd;
		
	case ceConversationAction::eatCoordSystemRemove:
		return new ceCACoordSystemRemove;
		
	case ceConversationAction::eatComment:
		return new ceCAComment;
		
	default:
		DETHROW( deeInvalidAction );
	}
}

const char *ceWPTMACreateAction::ActionTypeText( ceWindowMain &windowMain,
ceConversationAction::eActionTypes actionType ){
	switch( actionType ){
	case ceConversationAction::eatCameraShot:
		return "Camera Shot";
		
	case ceConversationAction::eatMusic:
		return "Music";
		
	case ceConversationAction::eatActorSpeak:
		return "Actor Speak";
		
	case ceConversationAction::eatSetVariable:
		return "Set Variable";
		
	case ceConversationAction::eatSetActorParameter:
		return "Set Actor Parameter";
		
	case ceConversationAction::eatTrigger:
		return "Trigger";
		
	case ceConversationAction::eatIfElse:
		return "If-Else";
		
	case ceConversationAction::eatPlayerChoice:
		return "Player Choice";
		
	case ceConversationAction::eatWait:
		return "Wait";
		
	case ceConversationAction::eatSnippet:
		return "Snippet";
		
	case ceConversationAction::eatStopConversation:
		return "Stop Conversation";
		
	case ceConversationAction::eatStopTopic:
		return "Stop Topic";
		
	case ceConversationAction::eatActorCommand:
		return "Actor Command";
		
	case ceConversationAction::eatGameCommand:
		return "Game Command";
		
	case ceConversationAction::eatActorAdd:
		return "Actor Add";
		
	case ceConversationAction::eatActorRemove:
		return "Actor Remove";
		
	case ceConversationAction::eatCoordSystemAdd:
		return "Coordinate System Add";
		
	case ceConversationAction::eatCoordSystemRemove:
		return "Coordinate System Remove";
		
	case ceConversationAction::eatComment:
		return "Comment";
		
	default:
		return "??";
	}
}

igdeIcon *ceWPTMACreateAction::ActionTypeIcon( ceWindowMain &windowMain,
ceConversationAction::eActionTypes actionType ){
	switch( actionType ){
	case ceConversationAction::eatCameraShot:
		return windowMain.GetIconActionCameraShot();
		
	case ceConversationAction::eatMusic:
		return windowMain.GetIconActionMusic();
		
	case ceConversationAction::eatActorSpeak:
		return windowMain.GetIconActionActorSpeak();
		
	case ceConversationAction::eatSetVariable:
		return windowMain.GetIconActionVariable();
		
	case ceConversationAction::eatSetActorParameter:
		return windowMain.GetIconActionVariable();
		
	case ceConversationAction::eatTrigger:
		return windowMain.GetIconActionTrigger();
		
	case ceConversationAction::eatIfElse:
		return windowMain.GetIconActionIfElse();
		
	case ceConversationAction::eatPlayerChoice:
		return windowMain.GetIconActionPlayerChoice();
		
	case ceConversationAction::eatWait:
		return windowMain.GetIconActionWait();
		
	case ceConversationAction::eatSnippet:
		return windowMain.GetIconActionSnippet();
		
	case ceConversationAction::eatStopConversation:
		return windowMain.GetIconActionStop();
		
	case ceConversationAction::eatStopTopic:
		return windowMain.GetIconActionStop();
		
	case ceConversationAction::eatActorCommand:
		return windowMain.GetIconActionActorCommand();
		
	case ceConversationAction::eatGameCommand:
		return windowMain.GetIconActionCommand();
		
	case ceConversationAction::eatActorAdd:
		return windowMain.GetIconActionActorAdd();
		
	case ceConversationAction::eatActorRemove:
		return windowMain.GetIconActionActorRemove();
		
	case ceConversationAction::eatCoordSystemAdd:
		return windowMain.GetIconActionCoordSysAdd();
		
	case ceConversationAction::eatCoordSystemRemove:
		return windowMain.GetIconActionCoordSysRemove();
		
	case ceConversationAction::eatComment:
		return windowMain.GetIconActionComment();
		
	default:
		return NULL;
	}
}
