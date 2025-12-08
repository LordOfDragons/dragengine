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

#include "ceWPTTIMAWait.h"
#include "ceWPTTIMAWaitCondition.h"
#include "ceWPTTIMAWaitActions.h"
#include "../../ceWPTTreeItem.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAWait.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAWait::ceWPTTIMAWait(ceWindowMain &windowMain,
ceConversation &conversation, ceCAWait *action) :
ceWPTTIMAction(windowMain, etActionWait, conversation, action),
pCondition(NULL),
pActions(NULL)
{
	SetIcon(windowMain.GetIconActionWait());
	SetText("Wait");
	
	try{
		pCondition.TakeOverWith(windowMain, conversation, action);
		AddChild(pCondition);
		
		pActions.TakeOverWith(windowMain, conversation, action->GetActions());
		AddChild(pActions);
		
	}catch(const deException &){
		throw;
	}
	
	pCondition->FreeReference(); // held by super-class child list
	pActions->FreeReference(); // held by super-class child list
}

ceWPTTIMAWait::~ceWPTTIMAWait(){
}



// Management
///////////////

void ceWPTTIMAWait::Update(){
	const ceCAWait &wait = *GetActionWait();
	SetExpanded(wait.GetTIMExpanded());
	
	pCondition->Update();
	pActions->Update();
}

void ceWPTTIMAWait::UpdateActionLists(){
	pActions->UpdateActions();
}

void ceWPTTIMAWait::OnExpandedChanged(){
	GetActionWait()->SetTIMExpanded(GetExpanded());
}
