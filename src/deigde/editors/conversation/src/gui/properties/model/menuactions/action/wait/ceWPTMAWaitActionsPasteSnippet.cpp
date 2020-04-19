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

#include "ceWPTMAWaitActionsPasteSnippet.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../../conversation/action/ceCAWait.h"
#include "../../../../../../undosys/action/wait/ceUCAWaitPaste.h"

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAWaitActionsPasteSnippet::ceWPTMAWaitActionsPasteSnippet( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceCAWait &wait, int index ) :
ceWPTMAPasteSnippet( windowMain, conversation ),
pTopic( &topic ),
pWait( &wait ),
pIndex( index )
{
	if( index < 0 || index > wait.GetActions().GetCount() ){
		DETHROW( deeInvalidAction );
	}
}



// Management
///////////////

ceUCActionPaste *ceWPTMAWaitActionsPasteSnippet::CreateUndo( const ceConversationActionList &actions ){
	ceUCActionPaste * const undo = new ceUCAWaitPaste( pTopic, pWait, actions, pIndex );
	undo->SetShortInfo( "Paste Conversation Snippet" );
	return undo;
}
