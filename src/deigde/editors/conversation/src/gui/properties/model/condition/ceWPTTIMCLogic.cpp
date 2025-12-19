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

#include "ceWPTTIMCLogic.h"
#include "../ceWPTTreeItem.h"
#include "../condition/ceWPTTIMCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicAddCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicClearCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicCutCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicPasteCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicRemoveCondition.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAIfElse.h"
#include "../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../conversation/condition/ceCConditionLogic.h"
#include "../../../../conversation/file/ceConversationFile.h"
#include "../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMCLogic::ceWPTTIMCLogic(ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionLogic *condition) :
ceWPTTIMConditions(windowMain, etConditionLogic, conversation, action,
	condition, condition->GetConditions())
{
	SetIcon(windowMain.GetIconConditionLogic());
	Update();
}

ceWPTTIMCLogic::~ceWPTTIMCLogic(){
}



// Management
///////////////

const char *ceWPTTIMCLogic::GetOperatorText(ceCConditionLogic::eOperators anOperator){
	switch(anOperator){
	case ceCConditionLogic::eopNone:
		return "none";
		
	case ceCConditionLogic::eopAny:
		return "any";
		
	case ceCConditionLogic::eopAll:
		return "all";
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void ceWPTTIMCLogic::Update(){
	const ceCConditionLogic &condition = *GetConditionLogic();
	
	decString text;
	text.Format("Logic (%s)", GetOperatorText(condition.GetOperator()));
	SetText(text);
	
	ceWPTTIMConditions::Update();
	
	SetExpanded(condition.GetTIMExpanded());
}

void ceWPTTIMCLogic::OnContextMenu(igdeMenuCascade &contextMenu){
	ceWPTTIMConditions::OnContextMenu(contextMenu);
	
	if(!GetTreeItem()){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	ceCConditionLogic &logic = *GetConditionLogic();
	ceConversationAction &action = GetAction();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// child action specific
	helper.MenuSeparator(contextMenu);
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(
		environment, "Logic: Add Condition", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	
	for(i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMACLogicAddCondition::Ref::New(windowMain, conversation, *topic,
			action, logic, ceWPTTIMCondition::ListAddMenuConditions[i]));
	}
	
	helper.MenuCommand(contextMenu, ceWPTMACLogicClearCondition::Ref::New(
		windowMain, conversation, *topic, action, logic));
	helper.MenuCommand(contextMenu, ceWPTMACLogicPasteCondition::Ref::New(
		windowMain, conversation, *topic, action, logic));
}

void ceWPTTIMCLogic::ContextMenuCondition(igdeMenuCascade &contextMenu, ceConversationCondition *condition){
	if(!GetTreeItem()){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	ceCConditionLogic &logic = *GetConditionLogic();
	ceConversationAction &action = GetAction();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// child action specific
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(
		environment, "Add Condition", environment.GetStockIcon(igdeEnvironment::esiPlus)));
	contextMenu.AddChild(subMenu);
	
	for(i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMACLogicAddCondition::Ref::New(windowMain, conversation, *topic,
			action, logic, ceWPTTIMCondition::ListAddMenuConditions[i]));
	}
	
	helper.MenuCommand(contextMenu, ceWPTMACopyCondition::Ref::New(windowMain, condition));
	helper.MenuCommand(contextMenu, ceWPTMACLogicCutCondition::Ref::New(
		windowMain, conversation, *topic, action, logic, condition));
	helper.MenuCommand(contextMenu, ceWPTMACLogicPasteCondition::Ref::New(
		windowMain, conversation, *topic, action, logic));
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, ceWPTMACLogicRemoveCondition::Ref::New(
		windowMain, conversation, *topic, action, logic, condition));
}

void ceWPTTIMCLogic::OnExpandedChanged(){
	GetConditionLogic()->SetTIMExpanded(GetExpanded());
}
