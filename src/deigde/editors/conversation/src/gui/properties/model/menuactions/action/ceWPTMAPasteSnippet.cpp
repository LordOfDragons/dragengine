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

#include "ceWPTMAPasteSnippet.h"
#include "../../ceWPTTreeModel.h"
#include "../../action/ceWPTTIMAction.h"
#include "../../../ceWPTopic.h"
#include "../../../ceWindowProperties.h"
#include "../../../../ceWindowMain.h"
#include "../../../../dialog/ceDialogPasteSnippet.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../undosys/action/ceUCActionPaste.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAPasteSnippet::ceWPTMAPasteSnippet( ceWindowMain &windowMain,
ceConversation &conversation ) :
ceWPTMenuAction( windowMain, "Paste Conversation Snippet...",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ) ),
pConversation( &conversation ){
}



// Management
///////////////

void ceWPTMAPasteSnippet::OnAction(){
	ceWPTopic &wptopic = GetWindowMain().GetWindowProperties().GetPanelTopic();
	if( ! wptopic.GetActionTreeModel() ){
		return;
	}
	
	igdeDialogReference refDialog;
	refDialog.TakeOver( new ceDialogPasteSnippet( GetWindowMain().GetEnvironment(), pConversation ) );
	ceDialogPasteSnippet &dialog = ( ceDialogPasteSnippet& )( igdeDialog& )refDialog;
	
	if( ! dialog.Run( &GetWindowMain() ) || dialog.GetActions().GetCount() == 0 ){
		return;
	}
	
	ceConversationAction *selectAction = NULL;
	igdeUndoReference undo;
	undo.TakeOver( CreateUndo( dialog.GetActions() ) );
	//undo->SetShortInfo( "Paste Conversation Snippet" );
	selectAction = ( ( ceUCActionPaste& )( igdeUndo& )undo ).GetActions().GetAt( 0 );
	pConversation->GetUndoSystem()->Add( undo );
	
	ceWPTTreeModel &model = *wptopic.GetActionTreeModel();
	ceWPTTIMAction * const modelAction = model.DeepFindAction( selectAction );
	if( modelAction ){
		modelAction->SetAsCurrentItem();
	}
	
	wptopic.SelectActiveAction();
}
