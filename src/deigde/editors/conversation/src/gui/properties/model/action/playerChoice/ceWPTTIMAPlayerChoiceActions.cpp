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
ceConversation &conversation, const ceConversationAction::List &actions) :
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
		return nullptr;
	}
	
	if(parent->GetType() == etActionPlayerChoice){
		return (ceWPTTIMAPlayerChoice*)parent;
		
	}else{
		return nullptr;
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
		? conversation.GetActiveFile()->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	
	// add action to end of action list
	const int indexAppend = playerChoice.GetActions().GetCount();
	int i;
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(
		environment, "Add Action", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMAPChoiceActionsAddAction::Ref::New(windowMain, conversation, *topic,
			playerChoice, ceWPTTIMAction::ListAddMenuActions[i], indexAppend));
	}
	
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsCopyActions::Ref::New(
		windowMain, playerChoice));
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsPasteActions::Ref::New(
		windowMain, conversation, *topic, playerChoice, indexAppend));
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsPasteSnippet::Ref::New(
		windowMain, conversation, *topic, playerChoice, indexAppend));
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsRemoveAllActions::Ref::New(
		windowMain, conversation, *topic, playerChoice));
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
		? conversation.GetActiveFile()->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	const int indexAction = action ? playerChoice.GetActions().IndexOf(action) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsMoveAction::Ref::New(windowMain, conversation,
		*topic, playerChoice, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp)));
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsMoveAction::Ref::New(windowMain, conversation,
		*topic, playerChoice, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown)));
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, ceWPTMACopyAction::Ref::New(windowMain, action));
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsCutAction::Ref::New(
		windowMain, conversation, *topic, playerChoice, action));
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsPasteActions::Ref::New(
		windowMain, conversation, *topic, playerChoice, indexAction, "Paste Actions Before"));
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsPasteActions::Ref::New(
		windowMain, conversation, *topic, playerChoice, indexAction + 1, "Paste Actions After"));
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceActionsRemoveAction::Ref::New(
		windowMain, conversation, *topic, playerChoice, action));
	helper.MenuSeparator(contextMenu);
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(
		environment, "Insert Action Before", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMAPChoiceActionsAddAction::Ref::New(
			windowMain, conversation, *topic, playerChoice,
			ceWPTTIMAction::ListAddMenuActions[i], indexAction));
	}
	
	subMenu = igdeMenuCascade::Ref::New(environment, "Insert Action After",
		environment.GetStockIcon(igdeEnvironment::esiPlus));
	contextMenu.AddChild(subMenu);
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMAPChoiceActionsAddAction::Ref::New(
			windowMain, conversation, *topic, playerChoice,
			ceWPTTIMAction::ListAddMenuActions[i], indexAction + 1));
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
