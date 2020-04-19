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

#include "ceWPTTIMATrigger.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCATrigger.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMATrigger::ceWPTTIMATrigger( ceWindowMain &windowMain,
ceConversation &conversation, ceCATrigger *action ) :
ceWPTTIMAction( windowMain, etActionTrigger, conversation, action )
{
	SetIcon( windowMain.GetIconActionTrigger() );
	Update();
}

ceWPTTIMATrigger::~ceWPTTIMATrigger(){
}



// Management
///////////////

const char *ceWPTTIMATrigger::GetActionText( ceCATrigger::eActions action ){
	switch( action ){
	case ceCATrigger::eaFire:
		return "fire";
		
	case ceCATrigger::eaReset:
		return "reset";
		
	case ceCATrigger::eaPulse:
		return "pulse";
		
	case ceCATrigger::eaFullReset:
		return "full reset";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void ceWPTTIMATrigger::Update(){
	const ceCATrigger &action = *GetActionTrigger();
	decString text;
	
	const char * const textAction = GetActionText( action.GetAction() );
	
	text.Format( "Trigger: %s '%s'", textAction, action.GetName().GetString() );
	
	SetText( text );
}
