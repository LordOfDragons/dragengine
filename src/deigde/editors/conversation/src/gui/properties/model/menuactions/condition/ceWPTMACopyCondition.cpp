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

#include "ceWPTMACopyCondition.h"
#include "../../ceWPTTreeModel.h"
#include "../../../ceWPTopic.h"
#include "../../../ceWindowProperties.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../clipboard/ceClipboardDataCondition.h"
#include "../../../../../conversation/condition/ceConversationConditionList.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/clipboard/igdeClipboardDataReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMACopyCondition::ceWPTMACopyCondition( ceWindowMain &windowMain,
ceConversationCondition *condition ) :
ceWPTMenuAction( windowMain, "Copy Condition",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ) ),
pCondition( condition )
{
	SetEnabled( condition != NULL );
}

ceWPTMACopyCondition::ceWPTMACopyCondition( ceWindowMain &windowMain,
ceConversationCondition *condition, const char *text, igdeIcon *icon ) :
ceWPTMenuAction( windowMain, text, icon ),
pCondition( condition )
{
	SetEnabled( condition != NULL );
}



// Management
///////////////

void ceWPTMACopyCondition::OnAction(){
	if( ! pCondition ){
		return;
	}
	
	ceConversationConditionList conditions;
	conditions.Add( pCondition );
	
	igdeClipboardDataReference cdata;
	cdata.TakeOver( new ceClipboardDataCondition( conditions ) );
	GetWindowMain().GetClipboard().Set( cdata );
}
