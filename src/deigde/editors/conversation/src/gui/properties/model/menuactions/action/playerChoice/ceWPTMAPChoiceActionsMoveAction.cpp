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

#include "ceWPTMAPChoiceActionsMoveAction.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../../undosys/action/playerChoice/ceUCAPChoiceActionMove.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAPChoiceActionsMoveAction::ceWPTMAPChoiceActionsMoveAction( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceCAPlayerChoice &playerChoice, ceConversationAction *action,
int index, const char *text, igdeIcon *icon ) :
ceWPTMenuAction( windowMain, text, icon ),
pConversation( &conversation ),
pTopic( &topic ),
pPlayerChoice( &playerChoice ),
pAction( action ),
pIndex( index )
{
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	
	SetEnabled( index >= 0 && index < playerChoice.GetActions().GetCount() );
}



// Management
///////////////

void ceWPTMAPChoiceActionsMoveAction::OnAction(){
	if( pIndex < 0 || pIndex > pPlayerChoice->GetActions().GetCount() ){
		DETHROW( deeInvalidAction );
	}
	
	igdeUndoReference undo;
	undo.TakeOver( new ceUCAPChoiceActionMove( pTopic, pPlayerChoice, NULL, pAction, pIndex ) );
	pConversation->GetUndoSystem()->Add( undo );
}
