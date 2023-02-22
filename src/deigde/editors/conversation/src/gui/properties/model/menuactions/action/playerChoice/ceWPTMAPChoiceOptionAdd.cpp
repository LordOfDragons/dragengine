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

#include "ceWPTMAPChoiceOptionAdd.h"
#include "../../../ceWPTTreeModel.h"
#include "../../../action/ceWPTTIMAction.h"
#include "../../../action/playerChoice/ceWPTTIMAPlayerChoice.h"
#include "../../../action/playerChoice/ceWPTTIMAPlayerChoiceOption.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../../../../undosys/action/playerChoice/ceUCAPChoiceOptionAdd.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAPChoiceOptionAdd::ceWPTMAPChoiceOptionAdd( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic, ceCAPlayerChoice &playerChoice ) :
ceWPTMenuAction( windowMain, "Add Option", windowMain.GetIconActionOption() ),
pConversation( &conversation ),
pTopic( &topic ),
pPlayerChoice( &playerChoice ){
}



// Management
///////////////

void ceWPTMAPChoiceOptionAdd::OnAction(){
	ceCAPlayerChoiceOption *selectOption = NULL;
	ceCAPlayerChoiceOption *option = NULL;
	igdeUndoReference undo;
	
	try{
		option = new ceCAPlayerChoiceOption;
		undo.TakeOver( new ceUCAPChoiceOptionAdd( pTopic, pPlayerChoice,
			option, pPlayerChoice->GetOptions().GetCount() ) );
		selectOption = option;
		option->FreeReference();
		option = NULL;
		
		pConversation->GetUndoSystem()->Add( undo );
		
	}catch( const deException & ){
		if( option ){
			option->FreeReference();
		}
		throw;
	}
	
	ceWPTopic &wptopic = GetWindowMain().GetWindowProperties().GetPanelTopic();
	if( ! wptopic.GetActionTreeModel() ){
		return;
	}
	
	ceWPTTreeModel &model = *wptopic.GetActionTreeModel();
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = ( ceWPTTIMAPlayerChoice* )model.DeepFindAction( pPlayerChoice );
	if( ! modelPlayerChoice ){
		return;
	}
	
	ceWPTTIMAPlayerChoiceOption * const modelOption = modelPlayerChoice->GetOptionChild( selectOption );
	if( ! modelOption ){
		return;
	}
	
	modelOption->SetAsCurrentItem();
	wptopic.SelectActivePanel();
}
