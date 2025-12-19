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

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPATrigger.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCATrigger.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/trigger/ceUCATriggerSetName.h"
#include "../../../undosys/action/trigger/ceUCATriggerSetAction.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextName : public igdeTextFieldListener {
	ceWPATrigger &pPanel;
	
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(ceWPATrigger &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCATrigger * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetName()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCATriggerSetName::Ref::New(topic, action, textField->GetText()));
	}
};

class cComboAction : public igdeComboBoxListener {
	ceWPATrigger &pPanel;
	
public:
	typedef deTObjectReference<cComboAction> Ref;
	cComboAction(ceWPATrigger &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCATrigger * const action = pPanel.GetAction();
		if(!topic || !action || !comboBox->GetSelectedItem()){
			return;
		}
		
		const ceCATrigger::eActions triggerAction = (ceCATrigger::eActions)(intptr_t)
			comboBox->GetSelectedItem()->GetData();
		if(triggerAction == action->GetAction()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCATriggerSetAction::Ref::New(topic, action, triggerAction));
	}
};

}



// Class ceWPATrigger
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPATrigger::ceWPATrigger(ceWPTopic &parentPanel) : ceWPAction(parentPanel){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon(*this);
	
	helper.EditString(*this, "Name:", "Name of the trigger to operate or empty to have no effect",
		pEditName, cTextName::Ref::New(*this));
	
	helper.ComboBox(*this, "Action:", "The action to apply on the trigger",
		pCBAction, cComboAction::Ref::New(*this));
	pCBAction->AddItem("Fire", nullptr, (void*)(intptr_t)ceCATrigger::eaFire);
	pCBAction->AddItem("Reset", nullptr, (void*)(intptr_t)ceCATrigger::eaReset);
	pCBAction->AddItem("Pulse", nullptr, (void*)(intptr_t)ceCATrigger::eaPulse);
	pCBAction->AddItem("Full Reset", nullptr, (void*)(intptr_t)ceCATrigger::eaFullReset);
}

ceWPATrigger::~ceWPATrigger(){
}



// Management
///////////////

ceCATrigger *ceWPATrigger::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if(action && action->GetType() == ceConversationAction::eatTrigger){
		return (ceCATrigger*)action;
		
	}else{
		return nullptr;
	}
}

void ceWPATrigger::UpdateAction(){
	const ceCATrigger * const action = GetAction();
	
	UpdateCommonParams();
	
	if(action){
		pEditName->SetText(action->GetName());
		pCBAction->SetSelectionWithData((void*)(intptr_t)action->GetAction());
		
	}else{
		pEditName->ClearText();
		pCBAction->SetSelectionWithData((void*)(intptr_t)ceCATrigger::eaFire);
	}
}
