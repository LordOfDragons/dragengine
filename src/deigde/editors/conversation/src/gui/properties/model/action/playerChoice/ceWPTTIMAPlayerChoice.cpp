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
#include "ceWPTTIMAPlayerChoiceOption.h"
#include "ceWPTTIMAPlayerChoiceActions.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionAdd.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAPlayerChoice::ceWPTTIMAPlayerChoice(ceWindowMain &windowMain,
ceConversation &conversation, ceCAPlayerChoice *action) :
ceWPTTIMAction(windowMain, etActionPlayerChoice, conversation, action),
pActions(NULL)
{
	SetIcon(windowMain.GetIconActionPlayerChoice());
	SetText("Player Choice");
	
	try{
		pActions = new ceWPTTIMAPlayerChoiceActions(windowMain, conversation, action->GetActions());
		AddChild(pActions);
		
	}catch(const deException &){
		if(pActions){
			pActions->FreeReference();
		}
		throw;
	}
	
	pActions->FreeReference(); // held by super-class child list
}

ceWPTTIMAPlayerChoice::~ceWPTTIMAPlayerChoice(){
}



// Management
///////////////

ceWPTTIMAPlayerChoiceOption *ceWPTTIMAPlayerChoice::GetOptionChild(ceCAPlayerChoiceOption *option) const{
	const int count = GetChildCount();
	int i;
	
	for(i=0; i<count; i++){
		ceWPTTreeItemModel * const child = GetChildAt(i);
		if(child->GetType() != etActionPlayerChoiceOption){
			continue;
		}
		
		ceWPTTIMAPlayerChoiceOption * const childOption = (ceWPTTIMAPlayerChoiceOption*)child;
		if(childOption->GetOption() == option){
			return childOption;
		}
	}
	
	return NULL;
}

void ceWPTTIMAPlayerChoice::Update(){
	const ceCAPlayerChoice &playerChoice = *GetActionPlayerChoice();
	SetExpanded(playerChoice.GetTIMExpanded());
	
	pUpdateOptions();
	
	const int countOptions = GetActionPlayerChoice()->GetOptions().GetCount();
	if(GetChildAt(countOptions) != pActions){
		MoveChild(pActions, countOptions);
	}
	pActions->Update();
	
	const int count = countOptions + 1;
	while(GetChildCount() > count){
		RemoveChild(GetChildAt(GetChildCount() - 1));
	}
}

void ceWPTTIMAPlayerChoice::UpdateActionLists(){
	const int count = GetChildCount();
	int i;
	
	for(i=0; i<count; i++){
		ceWPTTreeItemModel * const child = GetChildAt(i);
		
		switch(child->GetType()){
		case etActionPlayerChoiceOption:
			((ceWPTTIMAPlayerChoiceOption*)child)->UpdateActionLists();
			break;
			
		case etActionPlayerChoiceActions:
			((ceWPTTIMAPlayerChoiceActions*)child)->UpdateActions();
			break;
			
		default:
			break;
		}
	}
}

void ceWPTTIMAPlayerChoice::OnExpandedChanged(){
	GetActionPlayerChoice()->SetTIMExpanded(GetExpanded());
}

void ceWPTTIMAPlayerChoice::OnContextMenu(igdeMenuCascade &contextMenu){
	if(!GetTreeItem()){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceCAPlayerChoice &playerChoice = *GetActionPlayerChoice();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if(!topic){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, new ceWPTMAPChoiceOptionAdd(
		windowMain, conversation, *topic, playerChoice), true);
	helper.MenuSeparator(contextMenu);
	
	ceWPTTIMAction::OnContextMenu(contextMenu);
}



// Private Functions
//////////////////////

void ceWPTTIMAPlayerChoice::pUpdateOptions(){
	ceCAPlayerChoice &playerChoice = *GetActionPlayerChoice();
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	
	// update if-cases
	const ceCAPlayerChoiceOptionList &options = playerChoice.GetOptions();
	const int optionCount = options.GetCount();
	int i, j;
	
	for(i=0; i<optionCount; i++){
		ceCAPlayerChoiceOption * const option = options.GetAt(i);
		
		// find item matching option if present
		ceWPTTIMAPlayerChoiceOption *model = NULL;
		const int childCount = GetChildCount();
		for(j=i; j<childCount; j++){
			ceWPTTreeItemModel * const child = GetChildAt(j);
			if(child->GetType() != etActionPlayerChoiceOption){
				continue;
			}
			
			ceWPTTIMAPlayerChoiceOption * const childOption = (ceWPTTIMAPlayerChoiceOption*)child;
			if(childOption->GetOption() == option){
				model = childOption;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		// in both cases the index is 
		if(model){
			if(j != i){
				MoveChild(j, i);
			}
			model->SetIndex(i);
			model->Update();
			
		}else{
			model = new ceWPTTIMAPlayerChoiceOption(windowMain, conversation, playerChoice, option, i);
			
			try{
				InsertChild(model, i);
				model->Update();
				
			}catch(const deException &){
				model->FreeReference();
				throw;
			}
			
			model->FreeReference();
		}
	}
}
