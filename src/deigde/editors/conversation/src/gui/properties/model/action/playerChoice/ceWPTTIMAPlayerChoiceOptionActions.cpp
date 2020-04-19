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
#include "ceWPTTIMAPlayerChoiceOption.h"
#include "ceWPTTIMAPlayerChoiceOptionActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionAddAction.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionCopyActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionPasteActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionPasteSnippet.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionRemoveAllActions.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionRemoveAction.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionMoveAction.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionCutAction.h"
#include "../../menuactions/action/ceWPTMACopyAction.h"
#include "../../ceWPTTreeItem.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAPlayerChoiceOptionActions::ceWPTTIMAPlayerChoiceOptionActions( ceWindowMain &windowMain,
ceConversation &conversation, const ceConversationActionList &actions ) :
ceWPTTIMActions( windowMain, conversation, etActionPlayerChoiceOptionActions, actions )
{
	SetText( "Actions" );
}

ceWPTTIMAPlayerChoiceOptionActions::~ceWPTTIMAPlayerChoiceOptionActions(){
}



// Management
///////////////

ceWPTTIMAPlayerChoiceOption *ceWPTTIMAPlayerChoiceOptionActions::GetModelOption() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionPlayerChoiceOption ){
		return ( ceWPTTIMAPlayerChoiceOption* )parent;
		
	}else{
		return NULL;
	}
}

ceWPTTIMAPlayerChoice *ceWPTTIMAPlayerChoiceOptionActions::GetModelPlayerChoice() const{
	ceWPTTIMAPlayerChoiceOption * const option = GetModelOption();
	if( option ){
		return option->GetModelPlayerChoice();
		
	}else{
		return NULL;
	}
}



void ceWPTTIMAPlayerChoiceOptionActions::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAPlayerChoiceOption * const modelOption = GetModelOption();
	if( ! modelOption ){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = modelOption->GetModelPlayerChoice();
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
	ceCAPlayerChoiceOption &option = *modelOption->GetOption();
	
	// add action to end of action list
	const int indexAppend = option.GetActions().GetCount();
	int i;
	
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Add Action",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceOptionAddAction( windowMain, conversation, *topic,
			playerChoice, option, ceWPTTIMAction::ListAddMenuActions[ i ], indexAppend ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionCopyActions( windowMain, option ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionPasteActions(
		windowMain, conversation, *topic, playerChoice, option, indexAppend ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionPasteSnippet(
		windowMain, conversation, *topic, playerChoice, option, indexAppend ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionRemoveAllActions(
		windowMain, conversation, *topic, playerChoice, option ), true );
}

void ceWPTTIMAPlayerChoiceOptionActions::ContextMenuAction(
igdeMenuCascade &contextMenu, ceConversationAction *action ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAPlayerChoiceOption * const modelOption = GetModelOption();
	if( ! modelOption ){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = modelOption->GetModelPlayerChoice();
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
	ceCAPlayerChoiceOption &option = *modelOption->GetOption();
	const ceConversationActionList &actions = option.GetActions();
	const int indexAction = action ? actions.IndexOf( action ) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionMoveAction( windowMain, conversation,
		*topic, playerChoice, option, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionMoveAction( windowMain, conversation,
		*topic, playerChoice, option, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMACopyAction( windowMain, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionCutAction(
		windowMain, conversation, *topic, playerChoice, option, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionPasteActions( windowMain, conversation,
		*topic, playerChoice, option, indexAction, "Paste Actions Before" ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionPasteActions( windowMain, conversation,
		*topic, playerChoice, option, indexAction + 1, "Paste Actions After" ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionRemoveAction(
		windowMain, conversation, *topic, playerChoice, option, action ), true );
	helper.MenuSeparator( contextMenu );
	
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action Before",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceOptionAddAction( windowMain, conversation, *topic,
			playerChoice, option, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction ), true );
	}
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action After",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceOptionAddAction( windowMain, conversation, *topic,
			playerChoice, option, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction + 1 ), true );
	}
}

void ceWPTTIMAPlayerChoiceOptionActions::Update(){
	ceWPTTIMAPlayerChoiceOption * const option = GetModelOption();
	if( option ){
		SetExpanded( option->GetOption()->GetTIMActionsExpanded() );
	}
	
	ceWPTTIMActions::Update();
}

void ceWPTTIMAPlayerChoiceOptionActions::OnExpandedChanged(){
	ceWPTTIMAPlayerChoiceOption * const option = GetModelOption();
	if( option ){
		option->GetOption()->SetTIMActionsExpanded( GetExpanded() );
	}
}
