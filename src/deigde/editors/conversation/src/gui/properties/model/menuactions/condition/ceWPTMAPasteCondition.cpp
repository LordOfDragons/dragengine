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

#include "ceWPTMAPasteCondition.h"
#include "../../ceWPTTreeModel.h"
#include "../../condition/ceWPTTIMCondition.h"
#include "../../../ceWPTopic.h"
#include "../../../ceWindowProperties.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../clipboard/ceClipboardDataCondition.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/condition/ceConversationConditionReference.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAPasteCondition::ceWPTMAPasteCondition( ceWindowMain &windowMain,
ceConversation &conversation ) :
ceWPTMenuAction( windowMain, "Paste Condition",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ) ),
pConversation( &conversation )
{
	SetEnabled( windowMain.GetClipboard().HasWithTypeName( ceClipboardDataCondition::TYPE_NAME ) );
}

ceWPTMAPasteCondition::ceWPTMAPasteCondition( ceWindowMain &windowMain,
ceConversation &conversation, const char *text ) :
ceWPTMenuAction( windowMain, text,
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ) ),
pConversation( &conversation )
{
	SetEnabled( windowMain.GetClipboard().HasWithTypeName( ceClipboardDataCondition::TYPE_NAME ) );
}



// Management
///////////////

void ceWPTMAPasteCondition::OnAction(){
	ceClipboardDataCondition * const cdata = ( ceClipboardDataCondition* )GetWindowMain().
		GetClipboard().GetWithTypeName( ceClipboardDataCondition::TYPE_NAME );
	if( ! cdata ){
		return;
	}
	
	ceConversationConditionReference condition;
	condition.TakeOver( cdata->GetConditions().GetAt( 0 )->CreateCopy() );
	
	ceConversationConditionList conditions;
	conditions.Add( condition );
	
	igdeUndoReference undo;
	undo.TakeOver( CreateUndo( conditions ) );
	pConversation->GetUndoSystem()->Add( undo );
}

igdeUndo *ceWPTMAPasteCondition::CreateUndo( const ceConversationConditionList &conditions ){
	// only not pure-virtual because FOX toolkit requires final classes. if the system
	// moves over to the IGDE ToolKit this will become a pure virtual again
	DETHROW( deeInvalidParam );
}
