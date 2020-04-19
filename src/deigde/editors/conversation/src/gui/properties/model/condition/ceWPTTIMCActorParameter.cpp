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

#include "ceWPTTIMCActorParameter.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceCConditionActorParameter.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMCActorParameter::ceWPTTIMCActorParameter( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionActorParameter *condition ) :
ceWPTTIMCondition( windowMain, etConditionActorParameter, conversation, action, condition )
{
	SetIcon( windowMain.GetIconConditionVariable() );
	Update();
}

ceWPTTIMCActorParameter::~ceWPTTIMCActorParameter(){
}



// Management
///////////////

const char *ceWPTTIMCActorParameter::GetOperatorText(
ceCConditionActorParameter::eOperators anOperator ){
	switch( anOperator ){
	case ceCConditionActorParameter::eopEqual:
		return "=";
		
	case ceCConditionActorParameter::eopNotEqual:
		return "!=";
		
	case ceCConditionActorParameter::eopLess:
		return "<";
		
	case ceCConditionActorParameter::eopLessEqual:
		return "<=";
		
	case ceCConditionActorParameter::eopGreater:
		return ">";
		
	case ceCConditionActorParameter::eopGreaterEqual:
		return ">=";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void ceWPTTIMCActorParameter::Update(){
	const ceCConditionActorParameter &condition = *GetConditionActorParameter();
	decString text;
	
	text.Format( "Actor Parameter: '%s': '%s' %s ", condition.GetActor().GetString(),
		condition.GetParameter().GetString(), GetOperatorText( condition.GetOperator() ) );
	
	if( ! condition.GetTestVariable().IsEmpty() ){
		text.AppendFormat( "'%s'", condition.GetTestVariable().GetString() );
		
	}else{
		text.AppendFormat( "%d", condition.GetTestValue() );
	}
	
	SetText( text );
}
