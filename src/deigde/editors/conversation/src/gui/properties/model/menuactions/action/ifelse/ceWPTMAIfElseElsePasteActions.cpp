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

#include "ceWPTMAIfElseElsePasteActions.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../../undosys/action/ifElse/ceUCAIfElsePaste.h"

#include <dragengine/common/exceptions.h>

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>



// Constructor, destructor
////////////////////////////

ceWPTMAIfElseElsePasteActions::ceWPTMAIfElseElsePasteActions( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceCAIfElse &ifElse, int index ) :
ceWPTMAPasteActions( windowMain, conversation ),
pTopic( &topic ),
pIfElse( &ifElse ),
pIndex( index )
{
	if( index < 0 || index > ifElse.GetElseActions().GetCount() ){
		DETHROW( deeInvalidAction );
	}
}

ceWPTMAIfElseElsePasteActions::ceWPTMAIfElseElsePasteActions( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceCAIfElse &ifElse, int index, const char *text ) :
ceWPTMAPasteActions( windowMain, conversation, text ),
pTopic( &topic ),
pIfElse( &ifElse ),
pIndex( index )
{
	if( index < 0 || index > ifElse.GetElseActions().GetCount() ){
		DETHROW( deeInvalidAction );
	}
}



// Management
///////////////

ceUCActionPaste *ceWPTMAIfElseElsePasteActions::CreateUndo( const ceConversationActionList &actions ){
	return new ceUCAIfElsePaste( pTopic, pIfElse, NULL, actions, pIndex );
}
