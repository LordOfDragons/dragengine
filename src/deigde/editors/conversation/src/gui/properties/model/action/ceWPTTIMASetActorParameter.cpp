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

#include "ceWPTTIMASetActorParameter.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCASetActorParameter.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMASetActorParameter::ceWPTTIMASetActorParameter( ceWindowMain &windowMain,
ceConversation &conversation, ceCASetActorParameter *action ) :
ceWPTTIMAction( windowMain, etActionSetActorParameter, conversation, action )
{
	SetIcon( windowMain.GetIconActionVariable() );
	Update();
}

ceWPTTIMASetActorParameter::~ceWPTTIMASetActorParameter(){
}



// Management
///////////////

const char *ceWPTTIMASetActorParameter::GetOperatorText(
ceCASetActorParameter::eOperators anOperator ){
	switch( anOperator ){
	case ceCASetActorParameter::eopSet:
		return "=";
		
	case ceCASetActorParameter::eopIncrement:
		return "+=";
		
	case ceCASetActorParameter::eopDecrement:
		return "-=";
		
	case ceCASetActorParameter::eopRandom:
		return "random";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void ceWPTTIMASetActorParameter::Update(){
	const ceCASetActorParameter &action = *GetActionSetActorParameter();
	decString text;
	
	const char * const textOperator = GetOperatorText( action.GetOperator() );
	
	text.Format( "Set Actor Parameter (%s): '%s' %s", action.GetActor().GetString(),
		action.GetName().GetString(), textOperator );
	
	if( ! action.GetValueVariable().IsEmpty() ){
		text.AppendFormat( "'%s'", action.GetValueVariable().GetString() );
		
	}else{
		text.AppendFormat( "%d", action.GetValue() );
	}
	
	SetText( text );
}
