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

#include "ceWPTTIMAPlayerChoice.h"
#include "ceWPTTIMAPlayerChoiceActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsAddAction.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsCopyActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsPasteActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsPasteSnippet.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsRemoveAllActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsRemoveAction.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsMoveAction.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceActionsCutAction.h"
#include "../../menuactions/action/ceWPTMACopyAction.h"
#include "../../ceWPTTreeItem.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAPlayerChoiceActions::ceWPTTIMAPlayerChoiceActions( ceWindowMain &windowMain,
ceConversation &conversation, const ceConversationActionList &actions ) :
ceWPTTIMActions( windowMain, conversation, etActionPlayerChoiceActions, actions )
{
	SetText( "Waiting Actions" );
}

ceWPTTIMAPlayerChoiceActions::~ceWPTTIMAPlayerChoiceActions(){
}



// Management
///////////////

ceWPTTIMAPlayerChoice *ceWPTTIMAPlayerChoiceActions::GetModelPlayerChoice() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionPlayerChoice ){
		return ( ceWPTTIMAPlayerChoice* )parent;
		
	}else{
		return NULL;
	}
}



void ceWPTTIMAPlayerChoiceActions::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = GetModelPlayerChoice();
	if( ! modelPlayerChoice ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	
	// add action to end of action list
	const int indexAppend = playerChoice.GetActions().GetCount();
	int i;
	
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Add Action",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceActionsAddAction( windowMain, conversation, *topic,
			playerChoice, ceWPTTIMAction::ListAddMenuActions[ i ], indexAppend ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsCopyActions(
		windowMain, playerChoice ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsPasteActions(
		windowMain, conversation, *topic, playerChoice, indexAppend ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsPasteSnippet(
		windowMain, conversation, *topic, playerChoice, indexAppend ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsRemoveAllActions(
		windowMain, conversation, *topic, playerChoice ), true );
}

void ceWPTTIMAPlayerChoiceActions::ContextMenuAction(
igdeMenuCascade &contextMenu, ceConversationAction *action ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = GetModelPlayerChoice();
	if( ! modelPlayerChoice ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	const ceConversationActionList &actions = playerChoice.GetActions();
	const int indexAction = action ? actions.IndexOf( action ) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsMoveAction( windowMain, conversation,
		*topic, playerChoice, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsMoveAction( windowMain, conversation,
		*topic, playerChoice, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMACopyAction( windowMain, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsCutAction(
		windowMain, conversation, *topic, playerChoice, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsPasteActions( windowMain, conversation,
		*topic, playerChoice, indexAction, "Paste Actions Before" ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsPasteActions( windowMain, conversation,
		*topic, playerChoice, indexAction + 1, "Paste Actions After" ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceActionsRemoveAction(
		windowMain, conversation, *topic, playerChoice, action ), true );
	helper.MenuSeparator( contextMenu );
	
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action Before",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceActionsAddAction( windowMain, conversation, *topic,
			playerChoice, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction ), true );
	}
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action After",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceActionsAddAction( windowMain, conversation, *topic,
			playerChoice, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction + 1 ), true );
	}
}

void ceWPTTIMAPlayerChoiceActions::Update(){
	ceWPTTIMAPlayerChoice * const playerChoice = GetModelPlayerChoice();
	if( playerChoice ){
		SetExpanded( playerChoice->GetActionPlayerChoice()->GetTIMActionsExpanded() );
	}
	
	ceWPTTIMActions::Update();
}

void ceWPTTIMAPlayerChoiceActions::OnExpandedChanged(){
	ceWPTTIMAPlayerChoice * const playerChoice = GetModelPlayerChoice();
	if( playerChoice ){
		playerChoice->GetActionPlayerChoice()->SetTIMActionsExpanded( GetExpanded() );
	}
}
