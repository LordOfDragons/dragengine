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

#include "ceWPTTIMATrigger.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCATrigger.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMATrigger::ceWPTTIMATrigger(ceWindowMain &windowMain,
ceConversation &conversation, ceCATrigger *action) :
ceWPTTIMAction(windowMain, etActionTrigger, conversation, action)
{
	SetIcon(windowMain.GetIconActionTrigger());
	Update();
}

ceWPTTIMATrigger::~ceWPTTIMATrigger(){
}



// Management
///////////////

decString ceWPTTIMATrigger::GetActionText(ceCATrigger::eActions action) const{
	switch(action){
	case ceCATrigger::eaFire:
		return GetWindowMain().Translate("Conversation.TriggerAction.Fire").ToUTF8();
		
	case ceCATrigger::eaReset:
		return GetWindowMain().Translate("Conversation.TriggerAction.Reset").ToUTF8();
		
	case ceCATrigger::eaPulse:
		return GetWindowMain().Translate("Conversation.TriggerAction.Pulse").ToUTF8();
		
	case ceCATrigger::eaFullReset:
		return GetWindowMain().Translate("Conversation.TriggerAction.FullReset").ToUTF8();
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void ceWPTTIMATrigger::Update(){
	const ceCATrigger &action = *GetActionTrigger();
	decString text;
	
	const decString textAction = GetActionText(action.GetAction());
	
	text.FormatSafe( GetWindowMain().Translate( "Conversation.Format.Trigger" ).ToUTF8(),
		textAction, action.GetName() );
	
	SetText(text);
}
