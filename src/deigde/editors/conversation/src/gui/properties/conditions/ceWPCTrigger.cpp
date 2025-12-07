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

#include "ceWPCTrigger.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionTrigger.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/condition/trigger/ceUCCTriggerSetTestMode.h"
#include "../../../undosys/condition/trigger/ceUCCTriggerSetTrigger.h"

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

class cComboTestMode : public igdeComboBoxListener {
	ceWPCTrigger &pPanel;
	
public:
	cComboTestMode(ceWPCTrigger &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionTrigger * const condition = pPanel.GetCondition();
		if(! topic || ! action || ! condition || ! comboBox->GetSelectedItem()){
			return;
		}
		
		const ceCConditionTrigger::eTestModes testMode =
			(ceCConditionTrigger::eTestModes)(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(testMode == condition->GetTestMode()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCTriggerSetTestMode::Ref::NewWith(topic, action, condition, testMode));
	}
};

class cTextTrigger : public igdeTextFieldListener {
	ceWPCTrigger &pPanel;
	
public:
	cTextTrigger(ceWPCTrigger &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionTrigger * const condition = pPanel.GetCondition();
		if(! topic || ! action || ! condition || condition->GetTrigger() == textField->GetText()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCTriggerSetTrigger::Ref::NewWith(topic, action, condition, textField->GetText()));
	}
};

}


// Class ceWPCTrigger
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPCTrigger::ceWPCTrigger(ceWPTopic &parentPanel) : ceWPCondition(parentPanel){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	helper.ComboBox(*this, "Test Mode:", "How to test the trigger",
		pCBTestMode, new cComboTestMode(*this));
	pCBTestMode->AddItem("Fired", NULL, (void*)(intptr_t)ceCConditionTrigger::etmFired);
	pCBTestMode->AddItem("Not Fired", NULL, (void*)(intptr_t)ceCConditionTrigger::etmNotFired);
	pCBTestMode->AddItem("Has Ever Fired", NULL, (void*)(intptr_t)ceCConditionTrigger::etmEverFired);
	pCBTestMode->AddItem("Has Never Fired", NULL, (void*)(intptr_t)ceCConditionTrigger::etmNeverFired);
	
	helper.EditString(*this, "Trigger:", "Name of trigger to test", pEditTrigger, new cTextTrigger(*this));
}

ceWPCTrigger::~ceWPCTrigger(){
}



// Management
///////////////

ceCConditionTrigger *ceWPCTrigger::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if(condition && condition->GetType() == ceConversationCondition::ectTrigger){
		return (ceCConditionTrigger*)condition;
		
	}else{
		return NULL;
	}
}

void ceWPCTrigger::UpdateCondition(){
	const ceCConditionTrigger * const condition = GetCondition();
	
	if(condition){
		pCBTestMode->SetSelectionWithData((void*)(intptr_t)condition->GetTestMode());
		pEditTrigger->SetText(condition->GetTrigger());
		
	}else{
		pCBTestMode->SetSelectionWithData((void*)(intptr_t)ceCConditionTrigger::etmFired);
		pEditTrigger->ClearText();
	}
}
