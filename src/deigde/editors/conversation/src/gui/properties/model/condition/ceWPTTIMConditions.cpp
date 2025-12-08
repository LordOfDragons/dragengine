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

#include "ceWPTTIMCondition.h"
#include "ceWPTTIMConditions.h"
#include "ceWPTTIMCLogic.h"
#include "ceWPTTIMCHasActor.h"
#include "ceWPTTIMCActorInConversation.h"
#include "ceWPTTIMCVariable.h"
#include "ceWPTTIMCActorParameter.h"
#include "ceWPTTIMCActorCommand.h"
#include "ceWPTTIMCGameCommand.h"
#include "ceWPTTIMCTrigger.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceConversationCondition.h"
#include "../../../../conversation/condition/ceConversationConditionList.h"
#include "../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMConditions::ceWPTTIMConditions(ceWindowMain &windowMain, eTypes type,
ceConversation &conversation, ceConversationAction &action, ceConversationCondition *condition,
const ceConversationConditionList &conditions) :
ceWPTTIMCondition(windowMain, type, conversation, action, condition),
pConditions(conditions){
}

ceWPTTIMConditions::~ceWPTTIMConditions(){
}



// Management
///////////////

ceWPTTIMCondition *ceWPTTIMConditions::GetChildWith(ceConversationCondition *condition) const{
	const int count = GetChildCount();
	int i;
	
	for(i=0; i<count; i++){
		ceWPTTIMCondition * const child = (ceWPTTIMCondition*)GetChildAt(i);
		if(child->GetCondition() == condition){
			return child;
		}
	}
	
	return NULL;
}

void ceWPTTIMConditions::StructureChanged(){
	Update();
}



void ceWPTTIMConditions::Update(){
	UpdateConditions();
}

void ceWPTTIMConditions::UpdateConditions(){
	const int count = pConditions.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		ceConversationCondition * const condition = pConditions.GetAt(i);
		
		// find item matching condition if present
		ceWPTTIMCondition::Ref model = NULL;
		const int childCount = GetChildCount();
		for(j=i; j<childCount; j++){
			ceWPTTIMCondition * const child = (ceWPTTIMCondition*)GetChildAt(j);
			if(child->GetCondition() == condition){
				model = child;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if(model){
			if(j != i){
				MoveChild(j, i);
			}
			model->Update();
			
		}else{
			model = CreateConditionModel(GetWindowMain(), GetConversation(),
				GetAction(), condition);
			
			try{
				InsertChild(model, i);
				model->Update();
				
			}catch(const deException &){
				throw;
			}
		}
	}
	
	// remove non-matching nodes
	while(GetChildCount() > count){
		RemoveChild(GetChildAt(GetChildCount() - 1));
	}
}

void ceWPTTIMConditions::OnContextMenu(igdeMenuCascade &contextMenu){
	ceWPTTIMCondition::OnContextMenu(contextMenu);
}



ceWPTTIMCondition *ceWPTTIMConditions::CreateConditionModel(ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceConversationCondition *condition){
	if(!condition){
		DETHROW(deeInvalidParam);
	}
	
	switch(condition->GetType()){
	case ceConversationCondition::ectLogic:
		return new ceWPTTIMCLogic(windowMain, conversation, action,
			(ceCConditionLogic*)condition);
		
	case ceConversationCondition::ectHasActor:
		return new ceWPTTIMCHasActor(windowMain, conversation, action,
			(ceCConditionHasActor*)condition);
		
	case ceConversationCondition::ectActorInConversation:
		return new ceWPTTIMCActorInConversation(windowMain, conversation, action,
			(ceCConditionActorInConversation*)condition);
		
	case ceConversationCondition::ectVariable:
		return new ceWPTTIMCVariable(windowMain, conversation, action,
			(ceCConditionVariable*)condition);
		
	case ceConversationCondition::ectActorParameter:
		return new ceWPTTIMCActorParameter(windowMain, conversation, action,
			(ceCConditionActorParameter*)condition);
		
	case ceConversationCondition::ectActorCommand:
		return new ceWPTTIMCActorCommand(windowMain, conversation, action,
			(ceCConditionActorCommand*)condition);
		
	case ceConversationCondition::ectGameCommand:
		return new ceWPTTIMCGameCommand(windowMain, conversation, action,
			(ceCConditionGameCommand*)condition);
		
	case ceConversationCondition::ectTrigger:
		return new ceWPTTIMCTrigger(windowMain, conversation, action,
			(ceCConditionTrigger*)condition);
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	DETHROW(deeInvalidParam);
}
