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

#include "ceWPTMAPChoiceOptionAdd.h"
#include "../../../ceWPTTreeModel.h"
#include "../../../action/ceWPTTIMAction.h"
#include "../../../action/playerChoice/ceWPTTIMAPlayerChoice.h"
#include "../../../action/playerChoice/ceWPTTIMAPlayerChoiceOption.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../../../../undosys/action/playerChoice/ceUCAPChoiceOptionAdd.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAPChoiceOptionAdd::ceWPTMAPChoiceOptionAdd(ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic, ceCAPlayerChoice &playerChoice) :
ceWPTMenuAction(windowMain, "Add Option", windowMain.GetIconActionOption()),
pConversation(&conversation),
pTopic(&topic),
pPlayerChoice(&playerChoice){
}



// Management
///////////////

void ceWPTMAPChoiceOptionAdd::OnAction(){
	ceCAPlayerChoiceOption *selectOption = NULL;
	ceCAPlayerChoiceOption *option = NULL;
	igdeUndo::Ref undo;
	
	try{
		option = new ceCAPlayerChoiceOption;
		undo.TakeOver(new ceUCAPChoiceOptionAdd(pTopic, pPlayerChoice,
			option, pPlayerChoice->GetOptions().GetCount()));
		selectOption = option;
		option->FreeReference();
		option = NULL;
		
		pConversation->GetUndoSystem()->Add(undo);
		
	}catch(const deException &){
		if(option){
			option->FreeReference();
		}
		throw;
	}
	
	ceWPTopic &wptopic = GetWindowMain().GetWindowProperties().GetPanelTopic();
	if(!wptopic.GetActionTreeModel()){
		return;
	}
	
	ceWPTTreeModel &model = *wptopic.GetActionTreeModel();
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = (ceWPTTIMAPlayerChoice*)model.DeepFindAction(pPlayerChoice);
	if(!modelPlayerChoice){
		return;
	}
	
	ceWPTTIMAPlayerChoiceOption * const modelOption = modelPlayerChoice->GetOptionChild(selectOption);
	if(!modelOption){
		return;
	}
	
	modelOption->SetAsCurrentItem();
	wptopic.SelectActivePanel();
}
