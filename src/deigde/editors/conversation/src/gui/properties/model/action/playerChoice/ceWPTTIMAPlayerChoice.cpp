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
ceWPTTIMAction(windowMain, etActionPlayerChoice, conversation, action)
{
	SetIcon(windowMain.GetIconActionPlayerChoice());
	SetText("Player Choice");
	
	pActions = ceWPTTIMAPlayerChoiceActions::Ref::New(windowMain, conversation, action->GetActions());
	AddChild(pActions);
}

ceWPTTIMAPlayerChoice::~ceWPTTIMAPlayerChoice(){
}



// Management
///////////////

ceWPTTIMAPlayerChoiceOption *ceWPTTIMAPlayerChoice::GetOptionChild(ceCAPlayerChoiceOption *option) const{
	for(const auto &c : GetChildren()){
		if(c->GetType() != etActionPlayerChoiceOption){
			continue;
		}
		
		const ceWPTTIMAPlayerChoiceOption::Ref childOption(c.DynamicCast<ceWPTTIMAPlayerChoiceOption>());
		if(childOption->GetOption() == option){
			return childOption;
		}
	}
	
	return nullptr;
}

void ceWPTTIMAPlayerChoice::Update(){
	const ceCAPlayerChoice &playerChoice = *GetActionPlayerChoice();
	SetExpanded(playerChoice.GetTIMExpanded());
	
	pUpdateOptions();
	
	const int countOptions = GetActionPlayerChoice()->GetOptions().GetCount();
	if(GetChildren().GetAt(countOptions) != pActions){
		MoveChild(pActions.Pointer(), countOptions);
	}
	pActions->Update();
	
	const int count = countOptions + 1;
	while(GetChildren().GetCount() > count){
		RemoveChild(GetChildren().Last());
	}
}

void ceWPTTIMAPlayerChoice::UpdateActionLists(){
	GetChildren().Visit([](const ceWPTTreeItemModel::Ref &c){
		switch(c->GetType()){
		case etActionPlayerChoiceOption:
			c.DynamicCast<ceWPTTIMAPlayerChoiceOption>()->UpdateActionLists();
			break;
			
		case etActionPlayerChoiceActions:
			c.DynamicCast<ceWPTTIMAPlayerChoiceActions>()->UpdateActions();
			break;
			
		default:
			break;
		}
	});
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
		? conversation.GetActiveFile()->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, ceWPTMAPChoiceOptionAdd::Ref::New(
		windowMain, conversation, *topic, playerChoice));
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
	const ceCAPlayerChoiceOption::List &options = playerChoice.GetOptions();
	const int optionCount = options.GetCount();
	int i, j;
	
	for(i=0; i<optionCount; i++){
		ceCAPlayerChoiceOption * const option = options.GetAt(i);
		
		// find item matching option if present
		ceWPTTIMAPlayerChoiceOption::Ref model;
		const int childCount = GetChildren().GetCount();
		for(j=i; j<childCount; j++){
			const ceWPTTreeItemModel::Ref &child = GetChildren().GetAt(j);
			if(child->GetType() != etActionPlayerChoiceOption){
				continue;
			}
			
			const ceWPTTIMAPlayerChoiceOption::Ref childOption(child.DynamicCast<ceWPTTIMAPlayerChoiceOption>());
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
			model = ceWPTTIMAPlayerChoiceOption::Ref::New(windowMain, conversation, playerChoice, option, i);
			
			try{
				InsertChild(model, i);
				model->Update();
				
			}catch(const deException &){
				throw;
			}
		}
	}
}
