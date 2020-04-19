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

#include "ceWPTTIMASetVariable.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCASetVariable.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMASetVariable::ceWPTTIMASetVariable( ceWindowMain &windowMain,
ceConversation &conversation, ceCASetVariable *action ) :
ceWPTTIMAction( windowMain, etActionSetVariable, conversation, action )
{
	SetIcon( windowMain.GetIconActionVariable() );
	Update();
}

ceWPTTIMASetVariable::~ceWPTTIMASetVariable(){
}



// Management
///////////////

const char *ceWPTTIMASetVariable::GetOperatorText( ceCASetVariable::eOperators anOperator ){
	switch( anOperator ){
	case ceCASetVariable::eopSet:
		return "=";
		
	case ceCASetVariable::eopIncrement:
		return "+=";
		
	case ceCASetVariable::eopDecrement:
		return "-=";
		
	case ceCASetVariable::eopRandom:
		return "random";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void ceWPTTIMASetVariable::Update(){
	const ceCASetVariable &action = *GetActionSetVariable();
	decString text;
	
	const char * const textOperator = GetOperatorText( action.GetOperator() );
	
	if( ! action.GetValueVariable().IsEmpty() ){
		text.Format( "Set Variable: '%s' %s '%s'", action.GetName().GetString(),
			textOperator, action.GetValueVariable().GetString() );
		
	}else{
		text.Format( "Set Variable: '%s' %s %d", action.GetName().GetString(),
			textOperator, action.GetValue() );
	}
	
	SetText( text );
}
