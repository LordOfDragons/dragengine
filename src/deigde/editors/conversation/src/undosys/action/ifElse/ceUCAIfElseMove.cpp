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

#include "ceUCAIfElseMove.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseMove
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseMove::ceUCAIfElseMove( ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, ceConversationAction *action, int newIndex ){
	if( ! topic || ! ifElse || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	int count = 0;
	
	pTopic = NULL;
	pIfElse = NULL;
	pAction = NULL;
	pCase = NULL;
	pNewIndex = newIndex;
	pOldIndex = -1;
	
	if( ifcase ){
		pOldIndex = ifcase->GetActions().IndexOf( action );
		count = ifcase->GetActions().GetCount();
		
	}else{
		pOldIndex = ifElse->GetElseActions().IndexOf( action );
		count = ifElse->GetElseActions().GetCount();
	}
	
	if( pOldIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex < 0 || pNewIndex >= count ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex == pOldIndex ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "IfElse Move Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	if( ifcase ){
		pCase = ifcase;
		ifcase->AddReference();
	}
	
	pAction = action;
	action->AddReference();
}

ceUCAIfElseMove::~ceUCAIfElseMove(){
	if( pAction ){
		pAction->FreeReference();
	}
	if( pCase ){
		pCase->FreeReference();
	}
	if( pIfElse ){
		pIfElse->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAIfElseMove::Undo(){
	if( pCase ){
		pCase->GetActions().MoveTo( pAction, pOldIndex );
		
	}else{
		pIfElse->GetElseActions().MoveTo( pAction, pOldIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
}

void ceUCAIfElseMove::Redo(){
	if( pCase ){
		pCase->GetActions().MoveTo( pAction, pNewIndex );
		
	}else{
		pIfElse->GetElseActions().MoveTo( pAction, pNewIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
}
