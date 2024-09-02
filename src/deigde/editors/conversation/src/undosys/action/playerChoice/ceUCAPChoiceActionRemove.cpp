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

#include "ceUCAPChoiceActionRemove.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionRemove
///////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionRemove::ceUCAPChoiceActionRemove( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, ceConversationAction *action ){
	if( ! topic || ! playerChoice || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pAction = NULL;
	
	if( option ){
		pIndex = option->GetActions().IndexOf( action );
		
	}else{
		pIndex = playerChoice->GetActions().IndexOf( action );
	}
	
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Player Choice Remove Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	if( option ){
		pOption = option;
		option->AddReference();
	}
	
	pAction = action;
	action->AddReference();
}

ceUCAPChoiceActionRemove::~ceUCAPChoiceActionRemove(){
	if( pAction ){
		pAction->FreeReference();
	}
	if( pOption ){
		pOption->FreeReference();
	}
	if( pPlayerChoice ){
		pPlayerChoice->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAPChoiceActionRemove::Undo(){
	if( pOption ){
		pOption->GetActions().InsertAt( pAction, pIndex );
		
	}else{
		pPlayerChoice->GetActions().InsertAt( pAction, pIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	pTopic->SetActive( pAction, NULL );
}

void ceUCAPChoiceActionRemove::Redo(){
	ceConversationAction *activateAction = NULL;
	
	if( pOption ){
		activateAction = ceUActionHelpers::ActivateActionAfterRemove( pOption->GetActions(), pAction );
		pOption->GetActions().Remove( pAction );
		
	}else{
		activateAction = ceUActionHelpers::ActivateActionAfterRemove( pPlayerChoice->GetActions(), pAction );
		pPlayerChoice->GetActions().Remove( pAction );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	
	pTopic->SetActive( activateAction ? activateAction : pPlayerChoice, NULL );
}
