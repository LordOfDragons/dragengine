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
#include "ceWPTTIMAIfElseElse.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseAdd.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElse::ceWPTTIMAIfElse(ceWindowMain &windowMain,
ceConversation &conversation, ceCAIfElse *action) :
ceWPTTIMAction(windowMain, etActionIfElse, conversation, action)
{
	SetIcon(windowMain.GetIconActionIfElse());
	SetText("If-Else");
	
	pElse = ceWPTTIMAIfElseElse::Ref::New(windowMain, conversation, action->GetElseActions());
	AddChild(pElse);
}

ceWPTTIMAIfElse::~ceWPTTIMAIfElse(){
}



// Management
///////////////

ceWPTTIMAIfElseIfCase *ceWPTTIMAIfElse::GetIfCaseChild(ceCAIfElseCase *ifCase) const{
	for(const auto &c : GetChildren()){
		if(c->GetType() != etActionIfElseCase){
			continue;
		}
		
		const ceWPTTIMAIfElseIfCase::Ref childIfCase(c.DynamicCast<ceWPTTIMAIfElseIfCase>());
		if(childIfCase->GetIfCase() == ifCase){
			return childIfCase;
		}
	}
	
	return nullptr;
}

void ceWPTTIMAIfElse::Update(){
	const ceCAIfElse &ifElse = *GetActionIfElse();
	SetExpanded(ifElse.GetTIMExpanded());
	
	pUpdateIfCases();
	
	const int countIfCases = ifElse.GetCases().GetCount();
	if(GetChildren().GetAt(countIfCases) != pElse){
		MoveChild(pElse.Pointer(), countIfCases);
	}
	pElse->Update();
	
	const int count = countIfCases + 1;
	while(GetChildren().GetCount() > count){
		RemoveChild(GetChildren().Last());
	}
}

void ceWPTTIMAIfElse::UpdateActionLists(){
	GetChildren().Visit([](const ceWPTTreeItemModel::Ref &c){
		switch(c->GetType()){
		case etActionIfElseCase:
			c.DynamicCast<ceWPTTIMAIfElseIfCase>()->UpdateActionLists();
			break;
			
		case etActionIfElseElse:
			c.DynamicCast<ceWPTTIMAIfElseElse>()->UpdateActions();
			break;
			
		default:
			break;
		}
	});
}

void ceWPTTIMAIfElse::OnExpandedChanged(){
	GetActionIfElse()->SetTIMExpanded(GetExpanded());
}

void ceWPTTIMAIfElse::OnContextMenu(igdeMenuCascade &contextMenu){
	if(!GetTreeItem()){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceCAIfElse &ifElse = *GetActionIfElse();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, ceWPTMAIfElseCaseAdd::Ref::New(
		windowMain, conversation, *topic, ifElse));
	helper.MenuSeparator(contextMenu);
	
	ceWPTTIMAction::OnContextMenu(contextMenu);
}



// Private Functions
//////////////////////

void ceWPTTIMAIfElse::pUpdateIfCases(){
	ceCAIfElse &ifElse = *GetActionIfElse();
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	
	// update if-cases
	const ceCAIfElseCase::List &ifCases = ifElse.GetCases();
	const int ifCaseCount = ifCases.GetCount();
	int i, j;
	
	for(i=0; i<ifCaseCount; i++){
		ceCAIfElseCase * const ifCase = ifCases.GetAt(i);
		
		// find item matching ifCase if present
		ceWPTTIMAIfElseIfCase::Ref model;
		const int childCount = GetChildren().GetCount();
		for(j=i; j<childCount; j++){
			ceWPTTreeItemModel * const child = GetChildren().GetAt(j);
			if(child->GetType() != etActionIfElseCase){
				continue;
			}
			
			ceWPTTIMAIfElseIfCase * const childIfCase = static_cast<ceWPTTIMAIfElseIfCase*>(child);
			if(childIfCase->GetIfCase() == ifCase){
				model = childIfCase;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if(model){
			if(j != i){
				MoveChild(j, i);
			}
			
			model->SetIndex(i);
			model->Update();
			
		}else{
			model = ceWPTTIMAIfElseIfCase::Ref::New(windowMain, conversation, ifElse, ifCase, i);
			
			try{
				InsertChild(model, i);
				model->Update();
				
			}catch(const deException &){
				throw;
			}
		}
	}
}
