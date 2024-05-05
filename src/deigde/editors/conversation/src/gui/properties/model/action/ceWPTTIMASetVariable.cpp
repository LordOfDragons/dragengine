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
