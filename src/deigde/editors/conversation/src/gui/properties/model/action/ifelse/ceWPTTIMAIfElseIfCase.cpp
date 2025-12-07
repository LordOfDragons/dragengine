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
#include "ceWPTTIMAIfElseIfCaseCondition.h"
#include "ceWPTTIMAIfElseIfCaseActions.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseRemove.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseMove.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElseIfCase::ceWPTTIMAIfElseIfCase(ceWindowMain &windowMain,
ceConversation &conversation, ceCAIfElse &ifElse, ceCAIfElseCase *ifCase, int index) :
ceWPTTreeItemModel(windowMain, conversation, etActionIfElseCase),
pIfCase(NULL),
pIndex(index),
pCondition(NULL),
pActions(NULL)
{
	if(!ifCase){
		DETHROW(deeInvalidParam);
	}
	
	SetIcon(windowMain.GetIconActionIfElseCaseIf());
	
	try{
		pCondition = new ceWPTTIMAIfElseIfCaseCondition(
			windowMain, conversation, ifElse, ifCase);
		AddChild(pCondition);
		
		pActions = new ceWPTTIMAIfElseIfCaseActions(
			windowMain, conversation, ifCase->GetActions());
		AddChild(pActions);
		
	}catch(const deException &){
		if(pCondition){
			pCondition->FreeReference();
		}
		if(pActions){
			pActions->FreeReference();
		}
		throw;
	}
	
	pCondition->FreeReference(); // held by super-class child list
	pActions->FreeReference(); // held by super-class child list
	
	pIfCase = ifCase;
	ifCase->AddReference();
	
	pUpdateText();
}

ceWPTTIMAIfElseIfCase::~ceWPTTIMAIfElseIfCase(){
	if(pIfCase){
		pIfCase->FreeReference();
	}
}



// Management
///////////////

ceWPTTIMAIfElse *ceWPTTIMAIfElseIfCase::GetModelIfElse() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if(!parent){
		return NULL;
	}
	
	if(parent->GetType() == etActionIfElse){
		return (ceWPTTIMAIfElse*)parent;
		
	}else{
		return NULL;
	}
}

void ceWPTTIMAIfElseIfCase::SetIndex(int index){
	if(index == pIndex){
		return;
	}
	
	pIndex = index;
	pUpdateText();
}

void ceWPTTIMAIfElseIfCase::Update(){
	SetExpanded(pIfCase->GetTIMExpanded());
	
	pCondition->Update();
	pActions->Update();
}

void ceWPTTIMAIfElseIfCase::UpdateActionLists(){
	pActions->UpdateActions();
}

void ceWPTTIMAIfElseIfCase::OnExpandedChanged(){
	pIfCase->SetTIMExpanded(GetTreeItem()->GetExpanded());
}

void ceWPTTIMAIfElseIfCase::OnContextMenu(igdeMenuCascade &contextMenu){
	if(!GetTreeItem()){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if(!modelIfElse){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if(!topic){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, new ceWPTMAIfElseCaseMove(windowMain, conversation,
		*topic, ifElse, pIfCase, pIndex - 1, "Move If-Case Up",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp)), true);
	helper.MenuCommand(contextMenu, new ceWPTMAIfElseCaseMove(windowMain, conversation,
		*topic, ifElse, pIfCase, pIndex + 1, "Move If-Case Down",
		windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown)), true);
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, new ceWPTMAIfElseCaseRemove(
		windowMain, conversation, *topic, ifElse, pIfCase), true);
}



// Private Functions
//////////////////////

void ceWPTTIMAIfElseIfCase::pUpdateText(){
	decString text;
	text.Format("If-Case %d", pIndex + 1);
	SetText(text);
}
