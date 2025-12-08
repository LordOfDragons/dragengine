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
#include "ceCAIfElseCase.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class ceCAIfElseCase
/////////////////////////

// Constructor, destructor
////////////////////////////

ceCAIfElseCase::ceCAIfElseCase() :
pCondition(NULL),
pTIMExpanded(true),
pTIMConditionExpanded(true),
pTIMActionsExpanded(true){
}

ceCAIfElseCase::ceCAIfElseCase(const ceCAIfElseCase &ifcase) :
pCondition(NULL),
pTIMExpanded(ifcase.pTIMExpanded),
pTIMConditionExpanded(ifcase.pTIMConditionExpanded),
pTIMActionsExpanded(ifcase.pTIMActionsExpanded)
{
	const ceConversationActionList &actions = ifcase.GetActions();
	ceConversationAction *newAction = NULL;
	int i, count;
	
	try{
		if(ifcase.GetCondition()){
			SetCondition(ifcase.GetCondition()->CreateCopy());
		}
		
		count = actions.GetCount();
		for(i=0; i<count; i++){
			newAction = actions.GetAt(i)->CreateCopy();
			pActions.Add(newAction);
			newAction->FreeReference();
			newAction = NULL;
		}
		
	}catch(const deException &){
		if(newAction){
			newAction->FreeReference();
		}
		pActions.RemoveAll();
		throw;
	}
}

ceCAIfElseCase::~ceCAIfElseCase(){
	pActions.RemoveAll();
	SetCondition(NULL);
}



// Management
///////////////

void ceCAIfElseCase::SetCondition(ceConversationCondition *condition){
	if(condition == pCondition){
		return;
	}
	
	
	pCondition = condition;
}



// UI
///////

void ceCAIfElseCase::SetTIMExpanded(bool expanded){
	pTIMExpanded = expanded;
}

void ceCAIfElseCase::SetTIMConditionExpanded(bool expanded){
	pTIMConditionExpanded = expanded;
}

void ceCAIfElseCase::SetTIMActionsExpanded(bool expanded){
	pTIMActionsExpanded = expanded;
}
