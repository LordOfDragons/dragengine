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
#include <deigde/undo/igdeUndo.h>

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
	
	igdeUndo::Ref undo;
	undo.TakeOver( CreateUndo( conditions ) );
	pConversation->GetUndoSystem()->Add( undo );
}

igdeUndo *ceWPTMAPasteCondition::CreateUndo( const ceConversationConditionList &conditions ){
	// only not pure-virtual because FOX toolkit requires final classes. if the system
	// moves over to the IGDE ToolKit this will become a pure virtual again
	DETHROW( deeInvalidParam );
}
