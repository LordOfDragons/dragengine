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

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAPlayerChoiceActions::ceWPTTIMAPlayerChoiceActions(ceWindowMain &windowMain,
ceConversation &conversation, const ceConversationActionList &actions) :
ceWPTTIMActions(windowMain, conversation, etActionPlayerChoiceActions, actions)
{
	SetText("Waiting Actions");
}

ceWPTTIMAPlayerChoiceActions::~ceWPTTIMAPlayerChoiceActions(){
}



// Management
///////////////

ceWPTTIMAPlayerChoice *ceWPTTIMAPlayerChoiceActions::GetModelPlayerChoice() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if(!parent){
		return NULL;
	}
	
	if(parent->GetType() == etActionPlayerChoice){
		return (ceWPTTIMAPlayerChoice*)parent;
		
	}else{
		return NULL;
	}
}



void ceWPTTIMAPlayerChoiceActions::OnContextMenu(igdeMenuCascade &contextMenu){
	if(!GetTreeItem()){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = GetModelPlayerChoice();
	if(!modelPlayerChoice){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if(!topic){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	
	// add action to end of action list
	const int indexAppend = playerChoice.GetActions().GetCount();
	int i;
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Add Action", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, new ceWPTMAPChoiceActionsAddAction(windowMain, conversation, *topic,
			playerChoice, ceWPTTIMAction::ListAddMenuActions[i], indexAppend), true);
	}
	
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsCopyActions(
		windowMain, playerChoice), true);
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsPasteActions(
		windowMain, conversation, *topic, playerChoice, indexAppend), true);
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsPasteSnippet(
		windowMain, conversation, *topic, playerChoice, indexAppend), true);
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsRemoveAllActions(
		windowMain, conversation, *topic, playerChoice), true);
}

void ceWPTTIMAPlayerChoiceActions::ContextMenuAction(
igdeMenuCascade &contextMenu, ceConversationAction *action){
	if(!GetTreeItem()){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = GetModelPlayerChoice();
	if(!modelPlayerChoice){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if(!topic){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	const ceConversationActionList &actions = playerChoice.GetActions();
	const int indexAction = action ? actions.IndexOf(action) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsMoveAction(windowMain, conversation,
		*topic, playerChoice, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp)), true);
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsMoveAction(windowMain, conversation,
		*topic, playerChoice, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown)), true);
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, new ceWPTMACopyAction(windowMain, action), true);
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsCutAction(
		windowMain, conversation, *topic, playerChoice, action), true);
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsPasteActions(windowMain, conversation,
		*topic, playerChoice, indexAction, "Paste Actions Before" ), true );
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsPasteActions(windowMain, conversation,
		*topic, playerChoice, indexAction + 1, "Paste Actions After" ), true );
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceActionsRemoveAction(
		windowMain, conversation, *topic, playerChoice, action), true);
	helper.MenuSeparator(contextMenu);
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Insert Action Before", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, new ceWPTMAPChoiceActionsAddAction(windowMain, conversation, *topic,
			playerChoice, ceWPTTIMAction::ListAddMenuActions[i], indexAction), true);
	}
	
	subMenu.TakeOver(new igdeMenuCascade(environment, "Insert Action After",
		environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, new ceWPTMAPChoiceActionsAddAction(windowMain, conversation, *topic,
			playerChoice, ceWPTTIMAction::ListAddMenuActions[i], indexAction + 1), true);
	}
}

void ceWPTTIMAPlayerChoiceActions::Update(){
	ceWPTTIMAPlayerChoice * const playerChoice = GetModelPlayerChoice();
	if(playerChoice){
		SetExpanded(playerChoice->GetActionPlayerChoice()->GetTIMActionsExpanded());
	}
	
	ceWPTTIMActions::Update();
}

void ceWPTTIMAPlayerChoiceActions::OnExpandedChanged(){
	ceWPTTIMAPlayerChoice * const playerChoice = GetModelPlayerChoice();
	if(playerChoice){
		playerChoice->GetActionPlayerChoice()->SetTIMActionsExpanded(GetExpanded());
	}
}
