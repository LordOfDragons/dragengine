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

#include "ceWPTMAIfElseCaseAdd.h"
#include "../../../ceWPTTreeModel.h"
#include "../../../action/ceWPTTIMAction.h"
#include "../../../action/ifelse/ceWPTTIMAIfElse.h"
#include "../../../action/ifelse/ceWPTTIMAIfElseIfCase.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../../undosys/action/ifElse/ceUCAIfElseCaseAdd.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAIfElseCaseAdd::ceWPTMAIfElseCaseAdd(ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic, ceCAIfElse &ifElse) :
ceWPTMenuAction(windowMain, "Add If-Case", windowMain.GetIconActionIfElseCaseIf()),
pConversation(&conversation),
pTopic(&topic),
pIfElse(&ifElse){
}



// Management
///////////////

void ceWPTMAIfElseCaseAdd::OnAction(){
	ceCAIfElseCase *selectIfCase = NULL;
	ceCAIfElseCase *ifCase = NULL;
	igdeUndo::Ref undo;
	
	try{
		ifCase = new ceCAIfElseCase;
		undo.TakeOver(new ceUCAIfElseCaseAdd(pTopic, pIfElse, ifCase, pIfElse->GetCases().GetCount()));
		selectIfCase = ifCase;
		ifCase->FreeReference();
		ifCase = NULL;
		
		pConversation->GetUndoSystem()->Add(undo);
		
	}catch(const deException &){
		throw;
	}
	
	ceWPTopic &wptopic = GetWindowMain().GetWindowProperties().GetPanelTopic();
	if(!wptopic.GetActionTreeModel()){
		return;
	}
	
	ceWPTTreeModel &model = *wptopic.GetActionTreeModel();
	ceWPTTIMAIfElse * const modelIfElse = (ceWPTTIMAIfElse*)model.DeepFindAction(pIfElse);
	if(!modelIfElse){
		return;
	}
	
	ceWPTTIMAIfElseIfCase * const modelIfCase = modelIfElse->GetIfCaseChild(selectIfCase);
	if(!modelIfCase){
		return;
	}
	
	modelIfCase->SetAsCurrentItem();
	wptopic.SelectActivePanel();
}
