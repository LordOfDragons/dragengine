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
