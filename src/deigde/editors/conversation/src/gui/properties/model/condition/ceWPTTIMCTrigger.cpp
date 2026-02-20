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

ceWPTTIMCTrigger::ceWPTTIMCTrigger(ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionTrigger *condition) :
ceWPTTIMCondition(windowMain, etConditionTrigger, conversation, action, condition)
{
	SetIcon(windowMain.GetIconConditionTrigger());
	Update();
}

ceWPTTIMCTrigger::~ceWPTTIMCTrigger(){
}



// Management
///////////////

decString ceWPTTIMCTrigger::GetTestModeText(ceCConditionTrigger::eTestModes testMode) const{
	switch(testMode){
	case ceCConditionTrigger::etmFired:
		return GetWindowMain().Translate("Conversation.TriggerTestMode.Fired").ToUTF8();
		
	case ceCConditionTrigger::etmNotFired:
		return GetWindowMain().Translate("Conversation.TriggerTestMode.NotFired").ToUTF8();
		
	case ceCConditionTrigger::etmEverFired:
		return GetWindowMain().Translate("Conversation.TriggerTestMode.EverFired").ToUTF8();
		
	case ceCConditionTrigger::etmNeverFired:
		return GetWindowMain().Translate("Conversation.TriggerTestMode.NeverFired").ToUTF8();
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void ceWPTTIMCTrigger::Update(){
	const ceCConditionTrigger &condition = *GetConditionTrigger();
	decString text;
	
	text.FormatSafe( GetWindowMain().Translate( "Conversation.Format.TriggerTest" ).ToUTF8(),
		condition.GetTrigger(), GetTestModeText(condition.GetTestMode()) );
	
	SetText(text);
}
