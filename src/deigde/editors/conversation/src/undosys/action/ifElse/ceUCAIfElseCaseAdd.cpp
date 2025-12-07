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
#include <string.h>
#include <stdlib.h>

#include "ceUCAIfElseCaseAdd.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseCaseAdd
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseCaseAdd::ceUCAIfElseCaseAdd(ceConversationTopic *topic, ceCAIfElse *ifelse, ceCAIfElseCase *ifcase, int index){
	if(!topic || !ifelse || !ifcase){
		DETHROW(deeInvalidParam);
	}
	if(index < 0 || index > ifelse->GetCases().GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pCase = NULL;
	pIndex = index;
	
	SetShortInfo("If-else add case");
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifelse;
	ifelse->AddReference();
	
	pCase = ifcase;
	ifcase->AddReference();
}

ceUCAIfElseCaseAdd::~ceUCAIfElseCaseAdd(){
}



// Management
///////////////

void ceUCAIfElseCaseAdd::Undo(){
	pIfElse->GetCases().Remove(pCase);
	pTopic->NotifyActionStructureChanged(pIfElse);
}

void ceUCAIfElseCaseAdd::Redo(){
	pIfElse->GetCases().InsertAt(pCase, pIndex);
	pTopic->NotifyActionStructureChanged(pIfElse);
}
