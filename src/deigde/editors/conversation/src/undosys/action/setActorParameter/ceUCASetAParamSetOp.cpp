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

#include "ceUCASetAParamSetOp.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCASetAParamSetOp
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCASetAParamSetOp::ceUCASetAParamSetOp(ceConversationTopic *topic,
ceCASetActorParameter *action, ceCASetActorParameter::eOperators newOperator){
	if(!topic || !action) DETHROW(deeInvalidParam);
	
	pTopic = NULL;
	pAction = NULL;
	pOldOperator = action->GetOperator();
	pNewOperator = newOperator;
	
	SetShortInfo("Action SetActorParameter Operator");
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
}

ceUCASetAParamSetOp::~ceUCASetAParamSetOp(){
	if(pAction){
		pAction->FreeReference();
	}
	if(pTopic){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCASetAParamSetOp::Undo(){
	pAction->SetOperator(pOldOperator);
	pTopic->NotifyActionChanged(pAction);
}

void ceUCASetAParamSetOp::Redo(){
	pAction->SetOperator(pNewOperator);
	pTopic->NotifyActionChanged(pAction);
}
