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

#include "ceWPTMAPasteActions.h"
#include "../../ceWPTTreeModel.h"
#include "../../action/ceWPTTIMAction.h"
#include "../../../ceWPTopic.h"
#include "../../../ceWindowProperties.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../clipboard/ceClipboardDataAction.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../undosys/action/ceUCActionPaste.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAPasteActions::ceWPTMAPasteActions(ceWindowMain &windowMain,
ceConversation &conversation) :
ceWPTMenuAction(windowMain, "Paste Actions",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste)),
pConversation(&conversation)
{
	SetEnabled(windowMain.GetClipboard().HasWithTypeName(ceClipboardDataAction::TYPE_NAME));
}

ceWPTMAPasteActions::ceWPTMAPasteActions(ceWindowMain &windowMain,
ceConversation &conversation, const char *text) :
ceWPTMenuAction(windowMain, text,
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste)),
pConversation(&conversation)
{
	SetEnabled(windowMain.GetClipboard().HasWithTypeName(ceClipboardDataAction::TYPE_NAME));
}



// Management
///////////////

void ceWPTMAPasteActions::OnAction(){
	ceClipboardDataAction * const cdata = (ceClipboardDataAction*)GetWindowMain().
		GetClipboard().GetWithTypeName(ceClipboardDataAction::TYPE_NAME);
	if(!cdata){
		return;
	}
	
	pConversation->GetUndoSystem()->Add(CreateUndo(cdata->GetActions()));
}

ceUCActionPaste::Ref ceWPTMAPasteActions::CreateUndo(const ceConversationAction::List &actions){
	// only not pure-virtual because FOX toolkit requires final classes. if the system
	// moves over to the IGDE ToolKit this will become a pure virtual again
	DETHROW(deeInvalidParam);
}
