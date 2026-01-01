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

#include "ceWPCAParam.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/condition/ceCConditionActorParameter.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/condition/actorParameter/ceUCCAParamSetActor.h"
#include "../../../undosys/condition/actorParameter/ceUCCAParamSetOperator.h"
#include "../../../undosys/condition/actorParameter/ceUCCAParamSetParameter.h"
#include "../../../undosys/condition/actorParameter/ceUCCAParamSetTestValue.h"
#include "../../../undosys/condition/actorParameter/ceUCCAParamSetTestVariable.h"

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

class cComboActor : public igdeComboBoxListener {
	ceWPCAParam &pPanel;
	
public:
	typedef deTObjectReference<cComboActor> Ref;
	cComboActor(ceWPCAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if(!topic || !action || !condition || comboBox->GetText() == condition->GetActor()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCAParamSetActor::Ref::New(topic, action, condition, comboBox->GetText()));
	}
};

class cComboOperator : public igdeComboBoxListener {
	ceWPCAParam &pPanel;
	
public:
	typedef deTObjectReference<cComboOperator> Ref;
	cComboOperator(ceWPCAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if(!topic || !action || !condition || !comboBox->GetSelectedItem()){
			return;
		}
		
		const ceCConditionActorParameter::eOperators newOperator =
			(ceCConditionActorParameter::eOperators)(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(newOperator == condition->GetOperator()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCAParamSetOperator::Ref::New(topic, action, condition, newOperator));
	}
};

class cTextParameter : public igdeTextFieldListener {
	ceWPCAParam &pPanel;
	
public:
	typedef deTObjectReference<cTextParameter> Ref;
	cTextParameter(ceWPCAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if(!topic || !action || !condition || condition->GetParameter() == textField->GetText()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCAParamSetParameter::Ref::New(topic, action, condition, textField->GetText()));
	}
};

class cTextTestValue : public igdeTextFieldListener {
	ceWPCAParam &pPanel;
	
public:
	typedef deTObjectReference<cTextTestValue> Ref;
	cTextTestValue(ceWPCAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if(!topic || !action || !condition){
			return;
		}
		
		const int value = textField->GetInteger();
		if(value == condition->GetTestValue()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCAParamSetTestValue::Ref::New(topic, action, condition, value));
	}
};

class cTextTestVariable : public igdeTextFieldListener {
	ceWPCAParam &pPanel;
	
public:
	typedef deTObjectReference<cTextTestVariable> Ref;
	cTextTestVariable(ceWPCAParam &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if(!topic || !action || !condition || condition->GetTestVariable() == textField->GetText()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCAParamSetTestVariable::Ref::New(topic, action, condition, textField->GetText()));
	}
};

}



// Class ceWPCAParam
//////////////////////

// Constructor, destructor
////////////////////////////

ceWPCAParam::ceWPCAParam(ceWPTopic &parentPanel) : ceWPCondition(parentPanel){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	helper.ComboBox(*this, "Actor ID:", true, "Actor ID to test", pCBActorID, cComboActor::Ref::New(*this));
	pCBActorID->SetDefaultSorter();
	
	helper.ComboBox(*this, "Operator:", "Operator to compare parameter value with test value",
		pCBOperator, cComboOperator::Ref::New(*this));
	pCBOperator->AddItem("Equal", nullptr, (void*)(intptr_t)ceCConditionActorParameter::eopEqual);
	pCBOperator->AddItem("Not Equal", nullptr, (void*)(intptr_t)ceCConditionActorParameter::eopNotEqual);
	pCBOperator->AddItem("Less", nullptr, (void*)(intptr_t)ceCConditionActorParameter::eopLess);
	pCBOperator->AddItem("Less or Equal", nullptr, (void*)(intptr_t)ceCConditionActorParameter::eopLessEqual);
	pCBOperator->AddItem("Greater", nullptr, (void*)(intptr_t)ceCConditionActorParameter::eopGreater);
	pCBOperator->AddItem("Greater or Equal", nullptr, (void*)(intptr_t)ceCConditionActorParameter::eopGreaterEqual);
	
	helper.EditString(*this, "Parameter:", "Parameter to compare",
		pEditParameter, cTextParameter::Ref::New(*this));
	helper.EditInteger(*this, "Test Value:", "Value to compare against",
		pEditTestValue, cTextTestValue::Ref::New(*this));
	helper.EditString(*this, "Test Variable:",
		"Variable to compare against or empty string to use 'Test Value'",
		pEditTestVariable, cTextTestVariable::Ref::New(*this));
}

ceWPCAParam::~ceWPCAParam(){
}



// Management
///////////////

ceCConditionActorParameter *ceWPCAParam::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if(condition && condition->GetType() == ceConversationCondition::ectActorParameter){
		return (ceCConditionActorParameter*)condition;
		
	}else{
		return nullptr;
	}
}

void ceWPCAParam::UpdateCondition(){
	const ceCConditionActorParameter * const condition = GetCondition();
	
	if(condition){
		pCBActorID->SetText(condition->GetActor());
		pCBOperator->SetSelectionWithData((void*)(intptr_t)condition->GetOperator());
		pEditParameter->SetText(condition->GetParameter());
		pEditTestValue->SetInteger(condition->GetTestValue());
		pEditTestVariable->SetText(condition->GetTestVariable());
		
	}else{
		pCBActorID->ClearText();
		pCBOperator->SetSelectionWithData((void*)(intptr_t)ceCConditionActorParameter::eopEqual);
		pEditParameter->ClearText();
		pEditTestValue->ClearText();
		pEditTestVariable->ClearText();
	}
}



void ceWPCAParam::UpdateActorIDList(){
	UpdateComboBoxWithActorIDList(pCBActorID);
}
