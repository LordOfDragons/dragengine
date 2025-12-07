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

#include "ceWPASetAParam.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCASetActorParameter.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/setActorParameter/ceUCASetAParamSetActor.h"
#include "../../../undosys/action/setActorParameter/ceUCASetAParamSetName.h"
#include "../../../undosys/action/setActorParameter/ceUCASetAParamSetOp.h"
#include "../../../undosys/action/setActorParameter/ceUCASetAParamSetValue.h"
#include "../../../undosys/action/setActorParameter/ceUCASetAParamSetValueVariable.h"

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

class cComboActorID : public igdeComboBoxListener {
	ceWPASetAParam &pPanel;
	
public:
	cComboActorID(ceWPASetAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if(!topic || !action  || comboBox->GetText() == action->GetActor()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetActor::Ref::NewWith(topic, action, comboBox->GetText()));
	}
};

class cTextName : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	cTextName(ceWPASetAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetName()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetName::Ref::NewWith(topic, action, textField->GetText()));
	}
};

class cComboOperator : public igdeComboBoxListener {
	ceWPASetAParam &pPanel;
	
public:
	cComboOperator(ceWPASetAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if(!topic || !action || !comboBox->GetSelectedItem()){
			return;
		}
		
		const ceCASetActorParameter::eOperators newOperator =
			(ceCASetActorParameter::eOperators)(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(newOperator == action->GetOperator()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetOp::Ref::NewWith(topic, action, newOperator));
	}
};

class cTextValue : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	cTextValue(ceWPASetAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		const int value = textField->GetInteger();
		if(!topic || !action || value == action->GetValue()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetValue::Ref::NewWith(topic, action, value));
	}
};

class cTextValueVariable : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	cTextValueVariable(ceWPASetAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetValueVariable()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetValueVariable::Ref::NewWith(topic, action, textField->GetText()));
	}
};

}



// Class ceWPASetAParam
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPASetAParam::ceWPASetAParam(ceWPTopic &parentPanel) : ceWPAction(parentPanel){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon(*this);
	
	helper.ComboBox(*this, "Actor ID:", true, "Actor to modify", pCBActorID, new cComboActorID(*this));
	pCBActorID->SetDefaultSorter();
	
	helper.EditString(*this, "Name:", "Name of the parameter to set or empty to have no effect",
		pEditName, new cTextName(*this));
	
	helper.ComboBox(*this, "Operator:", "How to apply the value to the parameter",
		pCBOperator, new cComboOperator(*this));
	pCBOperator->AddItem("Set", NULL, (void*)(intptr_t)ceCASetActorParameter::eopSet);
	pCBOperator->AddItem("Increment", NULL, (void*)(intptr_t)ceCASetActorParameter::eopIncrement);
	pCBOperator->AddItem("Decrement", NULL, (void*)(intptr_t)ceCASetActorParameter::eopDecrement);
	pCBOperator->AddItem("Random", NULL, (void*)(intptr_t)ceCASetActorParameter::eopRandom);
	
	helper.EditInteger(*this, "Value:", "Value to use for operation (integer value)",
		pEditValue, new cTextValue(*this));
	helper.EditString(*this, "Variable:", "Variable value to use for operation or empty string to use 'Value'",
		pEditValueVariable, new cTextValueVariable(*this));
}

ceWPASetAParam::~ceWPASetAParam(){
}



// Management
///////////////

ceCASetActorParameter *ceWPASetAParam::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if(action && action->GetType() == ceConversationAction::eatSetActorParameter){
		return (ceCASetActorParameter*)action;
		
	}else{
		return NULL;
	}
}

void ceWPASetAParam::UpdateAction(){
	const ceCASetActorParameter * const action = GetAction();
	
	UpdateCommonParams();
	
	if(action){
		pCBActorID->SetText(action->GetActor());
		pEditName->SetText(action->GetName());
		pCBOperator->SetSelectionWithData((void*)(intptr_t)action->GetOperator());
		pEditValue->SetInteger(action->GetValue());
		pEditValueVariable->SetText(action->GetValueVariable());
		
	}else{
		pCBActorID->ClearText();
		pEditName->ClearText();
		pCBOperator->SetSelectionWithData((void*)(intptr_t)ceCASetActorParameter::eopSet);
		pEditValue->ClearText();
		pEditValueVariable->ClearText();
	}
}



void ceWPASetAParam::UpdateActorIDLists(){
	ceWPAction::UpdateActorIDLists();
	UpdateComboBoxWithActorIDList(pCBActorID);
}
