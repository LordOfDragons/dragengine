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

#include "ceWPTMAIfElseCaseAdd.h"
#include "../../../ceWPTTreeModel.h"
#include "../../../action/ceWPTTIMAction.h"
#include "../../../action/ifelse/ceWPTTIMAIfElse.h"
#include "../../../action/ifelse/ceWPTTIMAIfElseIfCase.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../../undosys/action/ifElse/ceUCAIfElseCaseAdd.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAIfElseCaseAdd::ceWPTMAIfElseCaseAdd( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic, ceCAIfElse &ifElse ) :
ceWPTMenuAction( windowMain, "Add If-Case", windowMain.GetIconActionIfElseCaseIf() ),
pConversation( &conversation ),
pTopic( &topic ),
pIfElse( &ifElse ){
}



// Management
///////////////

void ceWPTMAIfElseCaseAdd::OnAction(){
	ceCAIfElseCase *selectIfCase = NULL;
	ceCAIfElseCase *ifCase = NULL;
	igdeUndoReference undo;
	
	try{
		ifCase = new ceCAIfElseCase;
		undo.TakeOver( new ceUCAIfElseCaseAdd( pTopic, pIfElse, ifCase, pIfElse->GetCases().GetCount() ) );
		selectIfCase = ifCase;
		ifCase->FreeReference();
		ifCase = NULL;
		
		pConversation->GetUndoSystem()->Add( undo );
		
	}catch( const deException & ){
		if( ifCase ){
			ifCase->FreeReference();
		}
		throw;
	}
	
	ceWPTopic &wptopic = GetWindowMain().GetWindowProperties().GetPanelTopic();
	if( ! wptopic.GetActionTreeModel() ){
		return;
	}
	
	ceWPTTreeModel &model = *wptopic.GetActionTreeModel();
	ceWPTTIMAIfElse * const modelIfElse = ( ceWPTTIMAIfElse* )model.DeepFindAction( pIfElse );
	if( ! modelIfElse ){
		return;
	}
	
	ceWPTTIMAIfElseIfCase * const modelIfCase = modelIfElse->GetIfCaseChild( selectIfCase );
	if( ! modelIfCase ){
		return;
	}
	
	modelIfCase->SetAsCurrentItem();
	wptopic.SelectActivePanel();
}
