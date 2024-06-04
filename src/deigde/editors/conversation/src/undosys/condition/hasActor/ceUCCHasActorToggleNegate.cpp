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

#include "ceUCCHasActorToggleNegate.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionHasActor.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCHasActorToggleNegate
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCHasActorToggleNegate::ceUCCHasActorToggleNegate( ceConversationTopic *topic, ceConversationAction *action, ceCConditionHasActor *hasActor ){
	if( ! topic || ! action || ! hasActor ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pAction = NULL;
	pHasActor = NULL;
	
	SetShortInfo( "Has Actor Negate" );
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
	
	pHasActor = hasActor;
	hasActor->AddReference();
}

ceUCCHasActorToggleNegate::~ceUCCHasActorToggleNegate(){
	if( pHasActor ){
		pHasActor->FreeReference();
	}
	if( pAction ){
		pAction->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCCHasActorToggleNegate::Undo(){
	pHasActor->SetNegate( ! pHasActor->GetNegate() );
	pTopic->NotifyConditionChanged( pAction, pHasActor );
}

void ceUCCHasActorToggleNegate::Redo(){
	pHasActor->SetNegate( ! pHasActor->GetNegate() );
	pTopic->NotifyConditionChanged( pAction, pHasActor );
}
