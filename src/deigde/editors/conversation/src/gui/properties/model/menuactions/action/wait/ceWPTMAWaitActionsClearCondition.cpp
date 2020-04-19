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

#include "ceWPTMAWaitActionsClearCondition.h"
#include "../../../ceWPTTreeModel.h"
#include "../../../action/ceWPTTIMAction.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/action/ceCAWait.h"
#include "../../../../../../undosys/action/wait/ceUCAWaitSetCondition.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAWaitActionsClearCondition::ceWPTMAWaitActionsClearCondition( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic, ceCAWait &wait ) :
ceWPTMenuAction( windowMain, "Clear Condition",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus) ),
pConversation( &conversation ),
pTopic( &topic ),
pWait( &wait )
{
	SetEnabled( wait.GetCondition() != NULL );
}



// Management
///////////////

void ceWPTMAWaitActionsClearCondition::OnAction(){
	igdeUndoReference undo;
	undo.TakeOver( new ceUCAWaitSetCondition( pTopic, pWait, NULL ) );
	GetConversation().GetUndoSystem()->Add( undo );
	
	ceWPTopic &wptopic = GetWindowMain().GetWindowProperties().GetPanelTopic();
	if( ! wptopic.GetActionTreeModel() ){
		return;
	}
	
	ceWPTTreeModel &model = *wptopic.GetActionTreeModel();
	ceWPTTIMAction * const modelAction = model.DeepFindAction( pWait );
	if( modelAction ){
		modelAction->SetAsCurrentItem();
	}
	
	wptopic.SelectActiveAction();
}
