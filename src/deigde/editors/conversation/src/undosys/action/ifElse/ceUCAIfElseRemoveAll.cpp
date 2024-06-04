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

#include "ceUCAIfElseRemoveAll.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseRemoveAll
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseRemoveAll::ceUCAIfElseRemoveAll( ceConversationTopic *topic, ceCAIfElse *ifElse, ceCAIfElseCase *ifcase ){
	if( ! topic || ! ifElse ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pCase = NULL;
	
	if( ifcase ){
		pActionList = ifcase->GetActions();
		
	}else{
		pActionList = ifElse->GetElseActions();
	}
	
	SetShortInfo( "IfElse Remove All Actions" );
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	if( ifcase ){
		pCase = ifcase;
		ifcase->AddReference();
	}
}

ceUCAIfElseRemoveAll::~ceUCAIfElseRemoveAll(){
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

void ceUCAIfElseRemoveAll::Undo(){
	if( pCase ){
		pCase->GetActions() = pActionList;
		
	}else{
		pIfElse->GetElseActions() = pActionList;
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
	
	if( pActionList.GetCount() > 0 ){
		pTopic->SetActive( pActionList.GetAt( 0 ), NULL );
	}
}

void ceUCAIfElseRemoveAll::Redo(){
	if( pCase ){
		pCase->GetActions().RemoveAll();
		
	}else{
		pIfElse->GetElseActions().RemoveAll();
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
	
	pTopic->SetActive( pIfElse, NULL );
}
