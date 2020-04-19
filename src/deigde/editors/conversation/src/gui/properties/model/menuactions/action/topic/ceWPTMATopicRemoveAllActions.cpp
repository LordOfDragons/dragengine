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

#include "ceWPTMATopicRemoveAllActions.h"
#include "../../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../../undosys/action/ceUCActionRemoveAll.h"

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMATopicRemoveAllActions::ceWPTMATopicRemoveAllActions( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic ) :
ceWPTMARemoveAllActions( windowMain, conversation ),
pTopic( &topic )
{
	SetEnabled( topic.GetActionList().GetCount() > 0 );
}



// Management
///////////////

igdeUndo *ceWPTMATopicRemoveAllActions::CreateUndo(){
	return new ceUCActionRemoveAll( pTopic );
}
