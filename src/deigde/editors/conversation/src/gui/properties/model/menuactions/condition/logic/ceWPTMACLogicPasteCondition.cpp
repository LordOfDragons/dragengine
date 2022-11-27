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

#include "ceWPTMACLogicPasteCondition.h"
#include "../../../../../../conversation/condition/ceCConditionLogic.h"
#include "../../../../../../undosys/condition/logic/ceUCCLogicPaste.h"

#include <dragengine/common/exceptions.h>

#include <deigde/undo/igdeUndoSystem.h>



// Constructor, destructor
////////////////////////////

ceWPTMACLogicPasteCondition::ceWPTMACLogicPasteCondition( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceConversationAction &action, ceCConditionLogic &logic ) :
ceWPTMAPasteCondition( windowMain, conversation, "Logic: Paste Condition" ),
pTopic( &topic ),
pAction( &action ),
pLogic( &logic ){
}



// Management
///////////////

igdeUndo *ceWPTMACLogicPasteCondition::CreateUndo( const ceConversationConditionList &conditions ){
	return new ceUCCLogicPaste( pTopic, pAction, pLogic, conditions );
}
