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

#include "ceWPTTIMCVariable.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceCConditionVariable.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMCVariable::ceWPTTIMCVariable( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionVariable *condition ) :
ceWPTTIMCondition( windowMain, etConditionVariable, conversation, action, condition )
{
	SetIcon( windowMain.GetIconConditionVariable() );
	Update();
}

ceWPTTIMCVariable::~ceWPTTIMCVariable(){
}



// Management
///////////////

const char *ceWPTTIMCVariable::GetOperatorText( ceCConditionVariable::eOperators anOperator ){
	switch( anOperator ){
	case ceCConditionVariable::eopEqual:
		return "=";
		
	case ceCConditionVariable::eopNotEqual:
		return "!=";
		
	case ceCConditionVariable::eopLess:
		return "<";
		
	case ceCConditionVariable::eopLessEqual:
		return "<=";
		
	case ceCConditionVariable::eopGreater:
		return ">";
		
	case ceCConditionVariable::eopGreaterEqual:
		return ">=";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void ceWPTTIMCVariable::Update(){
	const ceCConditionVariable &condition = *GetConditionVariable();
	decString text;
	
	if( ! condition.GetTestVariable().IsEmpty() ){
		text.Format( "Variable '%s' %s '%s'", condition.GetVariable().GetString(),
			GetOperatorText( condition.GetOperator() ), condition.GetTestVariable().GetString() );
		
	}else{
		text.Format( "Variable '%s' %s %d", condition.GetVariable().GetString(),
			GetOperatorText( condition.GetOperator() ), condition.GetTestValue() );
	}
	
	SetText( text );
}
