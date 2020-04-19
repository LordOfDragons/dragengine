/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
