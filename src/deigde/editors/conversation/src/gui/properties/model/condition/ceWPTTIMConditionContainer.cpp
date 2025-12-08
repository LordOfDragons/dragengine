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
#include "ceWPTTIMConditionContainer.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceConversationCondition.h"
#include "../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMConditionContainer::ceWPTTIMConditionContainer(
ceWindowMain &windowMain, ceConversation &conversation, ceConversationAction &action, eTypes type) :
ceWPTTreeItemModel(windowMain, conversation, type),
pAction(&action),
pChildCondition(NULL){
}

ceWPTTIMConditionContainer::~ceWPTTIMConditionContainer(){
}



// Management
///////////////

void ceWPTTIMConditionContainer::SetCondition(ceConversationCondition *condition){
	if(pCondition == condition){
		return;
	}
	
	if(pChildCondition){
		RemoveChild(pChildCondition);
		pChildCondition = NULL;
	}
	
	pCondition = condition;
	
	if(!condition){
		return;
	}
	
	ceWPTTIMCondition::Ref child = NULL;
	try{
		child = ceWPTTIMConditions::CreateConditionModel(GetWindowMain(),
			GetConversation(), GetAction(), condition);
		AddChild(child);
		pChildCondition = child;
		child->FreeReference(); // held by superclass child list
		
	}catch(const deException &){
		throw;
	}
}

void ceWPTTIMConditionContainer::SetOrUpdateCondition(ceConversationCondition *condition){
	if(pCondition == condition){
		if(pChildCondition){
			pChildCondition->Update();
		}
		
	}else{
		SetCondition(condition);
	}
}



ceWPTTIMCondition *ceWPTTIMConditionContainer::DeepFindCondition(ceConversationCondition *condition){
	if(pChildCondition){
		return pChildCondition->DeepFindCondition(condition);
		
	}else{
		return NULL;
	}
}
