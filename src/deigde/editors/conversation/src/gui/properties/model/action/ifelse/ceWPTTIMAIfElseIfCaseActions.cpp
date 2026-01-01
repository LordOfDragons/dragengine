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

#include "ceWPTTIMAIfElse.h"
#include "ceWPTTIMAIfElseIfCase.h"
#include "ceWPTTIMAIfElseIfCaseActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseAddAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseCopyActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteSnippet.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseRemoveAllActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseRemoveAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseMoveAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseCutAction.h"
#include "../../menuactions/action/ceWPTMACopyAction.h"
#include "../../ceWPTTreeItem.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElseIfCaseActions::ceWPTTIMAIfElseIfCaseActions(ceWindowMain &windowMain,
ceConversation &conversation, const ceConversationAction::List &actions) :
ceWPTTIMActions(windowMain, conversation, etActionIfElseCaseActions, actions)
{
	SetText("Actions");
}

ceWPTTIMAIfElseIfCaseActions::~ceWPTTIMAIfElseIfCaseActions(){
}



// Management
///////////////

ceWPTTIMAIfElseIfCase *ceWPTTIMAIfElseIfCaseActions::GetModelIfCase() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if(!parent){
		return nullptr;
	}
	
	if(parent->GetType() == etActionIfElseCase){
		return (ceWPTTIMAIfElseIfCase*)parent;
		
	}else{
		return nullptr;
	}
}

ceWPTTIMAIfElse *ceWPTTIMAIfElseIfCaseActions::GetModelIfElse() const{
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if(ifCase){
		return ifCase->GetModelIfElse();
		
	}else{
		return nullptr;
	}
}



void ceWPTTIMAIfElseIfCaseActions::OnContextMenu(igdeMenuCascade &contextMenu){
	if(!GetTreeItem()){
		return;
	}
	
	ceWPTTIMAIfElseIfCase * const modelIfCase = GetModelIfCase();
	if(!modelIfCase){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = modelIfCase->GetModelIfElse();
	if(!modelIfElse){
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
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	ceCAIfElseCase &ifCase = *modelIfCase->GetIfCase();
	
	// add action to end of action list
	const int indexAppend = ifCase.GetActions().GetCount();
	int i;
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(
		environment, "Add Action", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMAIfElseCaseAddAction::Ref::New(
			windowMain, conversation, *topic, ifElse, ifCase,
			ceWPTTIMAction::ListAddMenuActions[i], indexAppend));
	}
	
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCaseCopyActions::Ref::New(windowMain, ifCase));
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCasePasteActions::Ref::New(
		windowMain, conversation, *topic, ifElse, ifCase, indexAppend));
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCasePasteSnippet::Ref::New(
		windowMain, conversation, *topic, ifElse, ifCase, indexAppend));
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCaseRemoveAllActions::Ref::New(
		windowMain, conversation, *topic, ifElse, ifCase));
}

void ceWPTTIMAIfElseIfCaseActions::ContextMenuAction(igdeMenuCascade &contextMenu, ceConversationAction *action){
	if(!GetTreeItem()){
		return;
	}
	
	ceWPTTIMAIfElseIfCase * const modelIfCase = GetModelIfCase();
	if(!modelIfCase){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = modelIfCase->GetModelIfElse();
	if(!modelIfElse){
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
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	ceCAIfElseCase &ifCase = *modelIfCase->GetIfCase();
	const int indexAction = action ? ifCase.GetActions().IndexOf(action) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCaseMoveAction::Ref::New(windowMain, conversation,
		*topic, ifElse, ifCase, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp)));
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCaseMoveAction::Ref::New(windowMain, conversation,
		*topic, ifElse, ifCase, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown)));
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, ceWPTMACopyAction::Ref::New(windowMain, action));
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCaseCutAction::Ref::New(
		windowMain, conversation, *topic, ifElse, ifCase, action));
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCasePasteActions::Ref::New(
		windowMain, conversation, *topic, ifElse, ifCase, indexAction, "Paste Actions Before"));
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCasePasteActions::Ref::New(
		windowMain, conversation, *topic, ifElse, ifCase, indexAction + 1, "Paste Actions After"));
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCaseRemoveAction::Ref::New(
		windowMain, conversation, *topic, ifElse, ifCase, action));
	helper.MenuSeparator(contextMenu);
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(
		environment, "Insert Action Before", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMAIfElseCaseAddAction::Ref::New(
			windowMain, conversation, *topic, ifElse, ifCase,
			ceWPTTIMAction::ListAddMenuActions[i], indexAction));
	}
	
	subMenu = igdeMenuCascade::Ref::New(environment, "Insert Action After",
		environment.GetStockIcon(igdeEnvironment::esiPlus));
	contextMenu.AddChild(subMenu);
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMAIfElseCaseAddAction::Ref::New(
			windowMain, conversation, *topic, ifElse, ifCase,
			ceWPTTIMAction::ListAddMenuActions[i], indexAction + 1));
	}
}

void ceWPTTIMAIfElseIfCaseActions::Update(){
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if(ifCase){
		SetExpanded(ifCase->GetIfCase()->GetTIMActionsExpanded());
	}
	
	ceWPTTIMActions::Update();
}

void ceWPTTIMAIfElseIfCaseActions::OnExpandedChanged(){
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if(ifCase){
		ifCase->GetIfCase()->SetTIMActionsExpanded(GetExpanded());
	}
}
