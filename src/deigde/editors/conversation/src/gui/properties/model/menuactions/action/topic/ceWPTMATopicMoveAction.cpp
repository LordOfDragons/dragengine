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

#include "ceWPTMATopicMoveAction.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../../undosys/action/ceUCActionMove.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMATopicMoveAction::ceWPTMATopicMoveAction( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceConversationAction *action, int index, const char *text, igdeIcon *icon ) :
ceWPTMenuAction( windowMain, text, icon ),
pConversation( &conversation ),
pTopic( &topic ),
pAction( action ),
pIndex( index )
{
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	
	SetEnabled( index >= 0 && index < topic.GetActionList().GetCount() );
}



// Management
///////////////

void ceWPTMATopicMoveAction::OnAction(){
	if( pIndex < 0 || pIndex > pTopic->GetActionList().GetCount() ){
		DETHROW( deeInvalidAction );
	}
	
	igdeUndoReference undo;
	undo.TakeOver( new ceUCActionMove( pTopic, pAction, pIndex ) );
	pConversation->GetUndoSystem()->Add( undo );
}
