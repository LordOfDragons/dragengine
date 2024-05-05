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
