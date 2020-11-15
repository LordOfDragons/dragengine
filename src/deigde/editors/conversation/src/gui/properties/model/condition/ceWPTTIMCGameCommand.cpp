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

#include "ceWPTTIMCGameCommand.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceCConditionGameCommand.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMCGameCommand::ceWPTTIMCGameCommand( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionGameCommand *condition ) :
ceWPTTIMCondition( windowMain, etConditionGameCommand, conversation, action, condition )
{
	SetIcon( windowMain.GetIconConditionCommand() );
	Update();
}

ceWPTTIMCGameCommand::~ceWPTTIMCGameCommand(){
}



// Management
///////////////

void ceWPTTIMCGameCommand::Update(){
	const ceCConditionGameCommand &condition = *GetConditionGameCommand();
	decString text;
	
	if( ! condition.GetCommand().IsEmpty() ){
		const decString lineCommand( condition.GetCommand().Split( '\n' ).GetAt( 0 ) );
		
		if( condition.GetNegate() ){
			text += "[not] ";
		}
		
		if( lineCommand.GetLength() > 40 ){
			text += lineCommand.GetLeft( 40 ) + "...";
			
		}else{
			text += lineCommand;
		}
	}
	
	SetText( text );
}
