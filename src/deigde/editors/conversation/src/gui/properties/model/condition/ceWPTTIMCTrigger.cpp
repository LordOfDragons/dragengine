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

#include "ceWPTTIMCTrigger.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceCConditionTrigger.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMCTrigger::ceWPTTIMCTrigger( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionTrigger *condition ) :
ceWPTTIMCondition( windowMain, etConditionTrigger, conversation, action, condition )
{
	SetIcon( windowMain.GetIconConditionVariable() );
	Update();
}

ceWPTTIMCTrigger::~ceWPTTIMCTrigger(){
}



// Management
///////////////

const char *ceWPTTIMCTrigger::GetTestModeText( ceCConditionTrigger::eTestModes testMode ){
	switch( testMode ){
	case ceCConditionTrigger::etmFired:
		return "fired";
		
	case ceCConditionTrigger::etmNotFired:
		return "not fired";
		
	case ceCConditionTrigger::etmEverFired:
		return "ever fired";
		
	case ceCConditionTrigger::etmNeverFired:
		return "never fired";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void ceWPTTIMCTrigger::Update(){
	const ceCConditionTrigger &condition = *GetConditionTrigger();
	decString text;
	
	text.Format( "Trigger '%s' %s", condition.GetTrigger().GetString(),
		GetTestModeText( condition.GetTestMode() ) );
	
	SetText( text );
}
