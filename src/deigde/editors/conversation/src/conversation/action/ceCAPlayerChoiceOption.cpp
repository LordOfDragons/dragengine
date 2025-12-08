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

#include "ceConversationAction.h"
#include "ceCAPlayerChoiceOption.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class ceCAPlayerChoiceOption
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceCAPlayerChoiceOption::ceCAPlayerChoiceOption() :
pCondition(NULL),
pTIMExpanded(true),
pTIMConditionExpanded(true),
pTIMActionsExpanded(true){
}

ceCAPlayerChoiceOption::ceCAPlayerChoiceOption(const ceCAPlayerChoiceOption &option) :
pText(option.pText),
pCondition(NULL),
pTIMExpanded(option.pTIMExpanded),
pTIMConditionExpanded(option.pTIMConditionExpanded),
pTIMActionsExpanded(option.pTIMActionsExpanded)
{
	const ceConversationActionList &actions = option.GetActions();
	int i;
	
	try{
		if(option.pCondition){
			SetCondition(option.pCondition->CreateCopy());
		}
		
		const int count = actions.GetCount();
		for(i=0; i<count; i++){
			pActions.Add(ceConversationAction::Ref::New(actions.GetAt(i)->CreateCopy()));
		}
		
	}catch(const deException &){
		pActions.RemoveAll();
		throw;
	}
}

ceCAPlayerChoiceOption::~ceCAPlayerChoiceOption(){
	pActions.RemoveAll();
	SetCondition(NULL);
}



// Management
///////////////

void ceCAPlayerChoiceOption::SetText(const decUnicodeString &text){
	pText = text;
}

void ceCAPlayerChoiceOption::SetCondition(ceConversationCondition *condition){
	pCondition = condition;
}



// UI
///////

void ceCAPlayerChoiceOption::SetTIMExpanded(bool expanded){
	pTIMExpanded = expanded;
}

void ceCAPlayerChoiceOption::SetTIMConditionExpanded(bool expanded){
	pTIMConditionExpanded = expanded;
}

void ceCAPlayerChoiceOption::SetTIMActionsExpanded(bool expanded){
	pTIMActionsExpanded = expanded;
}
