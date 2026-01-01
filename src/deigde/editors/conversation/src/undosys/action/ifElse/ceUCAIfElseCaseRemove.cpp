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

#include "ceUCAIfElseCaseRemove.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseCaseRemove
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseCaseRemove::ceUCAIfElseCaseRemove(ceConversationTopic *topic, ceCAIfElse *ifelse, ceCAIfElseCase *ifcase){
	if(!topic || !ifelse || !ifcase){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = nullptr;
	pIfElse = nullptr;
	pCase = nullptr;
	pIndex = ifelse->GetCases().IndexOf(ifcase);
	
	if(pIndex == -1){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("If-else remove case");
	
	pTopic = topic;
	pIfElse = ifelse;
	pCase = ifcase;
}

ceUCAIfElseCaseRemove::~ceUCAIfElseCaseRemove(){
}



// Management
///////////////

void ceUCAIfElseCaseRemove::Undo(){
	pIfElse->GetCases().Insert(pCase, pIndex);
	pTopic->NotifyActionStructureChanged(pIfElse);
}

void ceUCAIfElseCaseRemove::Redo(){
	pIfElse->GetCases().Remove(pCase);
	pTopic->NotifyActionStructureChanged(pIfElse);
}
