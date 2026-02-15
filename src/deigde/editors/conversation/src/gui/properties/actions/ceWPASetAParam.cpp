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
	using Ref = deTObjectReference<cComboActorID>;
	cComboActorID(ceWPASetAParam &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if(!topic || !action  || comboBox->GetText() == action->GetActor()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetActor::Ref::New(topic, action, comboBox->GetText()));
	}
};

class cTextName : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	using Ref = deTObjectReference<cTextName>;
	cTextName(ceWPASetAParam &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetName()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetName::Ref::New(topic, action, textField->GetText()));
	}
};

class cComboOperator : public igdeComboBoxListener {
	ceWPASetAParam &pPanel;
	
public:
	using Ref = deTObjectReference<cComboOperator>;
	cComboOperator(ceWPASetAParam &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
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
			ceUCASetAParamSetOp::Ref::New(topic, action, newOperator));
	}
};

class cTextValue : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	using Ref = deTObjectReference<cTextValue>;
	cTextValue(ceWPASetAParam &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		const int value = textField->GetInteger();
		if(!topic || !action || value == action->GetValue()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetValue::Ref::New(topic, action, value));
	}
};

class cTextValueVariable : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	using Ref = deTObjectReference<cTextValueVariable>;
	cTextValueVariable(ceWPASetAParam &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetValueVariable()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCASetAParamSetValueVariable::Ref::New(topic, action, textField->GetText()));
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
	
	helper.ComboBox(*this, "@Conversation.WPActionSetAParam.ActorID", true, "@Conversation.ToolTip.ActorToModify", pCBActorID, cComboActorID::Ref::New(*this));
	pCBActorID->SetDefaultSorter();
	
	helper.EditString(*this, "@Conversation.WPActionSetAParam.Name", "@Conversation.ToolTip.ParameterName",
		pEditName, cTextName::Ref::New(*this));
	
	helper.ComboBox(*this, "@Conversation.WPActionSetAParam.Operator", "@Conversation.ToolTip.OperatorForParameter",
		pCBOperator, cComboOperator::Ref::New(*this));
	pCBOperator->SetAutoTranslateItems(true);
	pCBOperator->AddItem("@Conversation.WPActionSetAParam.Set", nullptr, (void*)(intptr_t)ceCASetActorParameter::eopSet);
	pCBOperator->AddItem("@Conversation.WPActionSetAParam.Increment", nullptr, (void*)(intptr_t)ceCASetActorParameter::eopIncrement);
	pCBOperator->AddItem("@Conversation.WPActionSetAParam.Decrement", nullptr, (void*)(intptr_t)ceCASetActorParameter::eopDecrement);
	pCBOperator->AddItem("@Conversation.WPActionSetAParam.Random", nullptr, (void*)(intptr_t)ceCASetActorParameter::eopRandom);
	
	helper.EditInteger(*this, "@Conversation.WPActionSetAParam.Value", "@Conversation.WPActionSetAParam.Value.ToolTip",
		pEditValue, cTextValue::Ref::New(*this));
	helper.EditString(*this, "@Conversation.WPActionSetAParam.Variable", "@Conversation.ToolTip.VariableForParameter",
		pEditValueVariable, cTextValueVariable::Ref::New(*this));
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
		return nullptr;
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
