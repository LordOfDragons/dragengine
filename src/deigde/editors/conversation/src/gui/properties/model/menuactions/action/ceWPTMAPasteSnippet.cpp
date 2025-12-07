/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAPasteSnippet::ceWPTMAPasteSnippet(ceWindowMain &windowMain,
ceConversation &conversation) :
ceWPTMenuAction(windowMain, "Paste Conversation Snippet...",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste)),
pConversation(&conversation){
}



// Management
///////////////

void ceWPTMAPasteSnippet::OnAction(){
	ceWPTopic &wptopic = GetWindowMain().GetWindowProperties().GetPanelTopic();
	if(!wptopic.GetActionTreeModel()){
		return;
	}
	
	ceDialogPasteSnippet::Ref dialog(ceDialogPasteSnippet::Ref::NewWith(
		GetWindowMain().GetEnvironment(), pConversation));
	
	if(!dialog->Run(&GetWindowMain()) || dialog->GetActions().GetCount() == 0){
		return;
	}
	
	igdeUndo::Ref undo(igdeUndo::Ref::New(CreateUndo(dialog->GetActions())));
	//undo->SetShortInfo( "Paste Conversation Snippet" );
	pConversation->GetUndoSystem()->Add(undo);
}
