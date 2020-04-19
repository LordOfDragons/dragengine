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

#include "ceWPTTIMCActorInConversation.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceCConditionActorInConversation.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMCActorInConversation::ceWPTTIMCActorInConversation( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionActorInConversation *condition ) :
ceWPTTIMCondition( windowMain, etConditionActorInConversation, conversation, action, condition )
{
	SetIcon( windowMain.GetIconConditionActorInConversation() );
	Update();
}

ceWPTTIMCActorInConversation::~ceWPTTIMCActorInConversation(){
}



// Management
///////////////

void ceWPTTIMCActorInConversation::Update(){
	const ceCConditionActorInConversation &condition = *GetConditionActorInConversation();
	decString text;
	
	if( condition.GetNegate() ){
		text.Format( "Actor not in Conversation: '%s'", condition.GetActor().GetString() );
		
	}else{
		text.Format( "Actor in Conversation: '%s'", condition.GetActor().GetString() );
	}
	
	SetText( text );
}
