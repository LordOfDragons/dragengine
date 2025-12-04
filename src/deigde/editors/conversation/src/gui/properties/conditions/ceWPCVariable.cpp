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

#include "ceWPCVariable.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionVariable.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/condition/variable/ceUCCVarSetOperator.h"
#include "../../../undosys/condition/variable/ceUCCVarSetTestValue.h"
#include "../../../undosys/condition/variable/ceUCCVarSetTestVariable.h"
#include "../../../undosys/condition/variable/ceUCCVarSetVariable.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
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

class cComboOperator : public igdeComboBoxListener {
	ceWPCVariable &pPanel;
	
public:
	cComboOperator( ceWPCVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCConditionVariable::eOperators newOperator =
			( ceCConditionVariable::eOperators )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( newOperator == condition->GetOperator() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCCVarSetOperator( topic, action, condition, newOperator ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextVariable : public igdeTextFieldListener {
	ceWPCVariable &pPanel;
	
public:
	cTextVariable( ceWPCVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || condition->GetVariable() == textField->GetText() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCCVarSetVariable( topic, action, condition, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTestValue : public igdeTextFieldListener {
	ceWPCVariable &pPanel;
	
public:
	cTextTestValue( ceWPCVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition ){
			return;
		}
		
		const int value = textField->GetInteger();
		if( value == condition->GetTestValue() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCCVarSetTestValue( topic, action, condition, value ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTestVariable : public igdeTextFieldListener {
	ceWPCVariable &pPanel;
	
public:
	cTextTestVariable( ceWPCVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || condition->GetTestVariable() == textField->GetText() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCCVarSetTestVariable( topic, action, condition, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPCVariable
////////////////////////

// Constructor, destructor
////////////////////////////

ceWPCVariable::ceWPCVariable( ceWPTopic &parentPanel ) : ceWPCondition( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	helper.ComboBox( *this, "Operator:", "Operator to compare variable value with test value",
		pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "Equal", NULL, ( void* )( intptr_t )ceCConditionVariable::eopEqual );
	pCBOperator->AddItem( "Not Equal", NULL, ( void* )( intptr_t )ceCConditionVariable::eopNotEqual );
	pCBOperator->AddItem( "Less", NULL, ( void* )( intptr_t )ceCConditionVariable::eopLess );
	pCBOperator->AddItem( "Less or Equal", NULL, ( void* )( intptr_t )ceCConditionVariable::eopLessEqual );
	pCBOperator->AddItem( "Greater", NULL, ( void* )( intptr_t )ceCConditionVariable::eopGreater );
	pCBOperator->AddItem( "Greater or Equal", NULL, ( void* )( intptr_t )ceCConditionVariable::eopGreaterEqual );
	
	helper.EditString( *this, "Variable:", "Variable to compare",
		pEditVariable, new cTextVariable( *this ) );
	helper.EditInteger( *this, "Test Value:", "Value to compare against",
		pEditTestValue, new cTextTestValue( *this ) );
	helper.EditString( *this, "Test Variable:",
		"Variable to compare against or empty string to use 'Test Value'",
		pEditTestVariable, new cTextTestVariable( *this ) );
}

ceWPCVariable::~ceWPCVariable(){
}



// Management
///////////////

ceCConditionVariable *ceWPCVariable::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if( condition && condition->GetType() == ceConversationCondition::ectVariable ){
		return ( ceCConditionVariable* )condition;
		
	}else{
		return NULL;
	}
}

void ceWPCVariable::UpdateCondition(){
	const ceCConditionVariable * const condition = GetCondition();
	
	if( condition ){
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )condition->GetOperator() );
		pEditVariable->SetText( condition->GetVariable() );
		pEditTestValue->SetInteger( condition->GetTestValue() );
		pEditTestVariable->SetText( condition->GetTestVariable() );
		
	}else{
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )ceCConditionVariable::eopEqual );
		pEditVariable->ClearText();
		pEditTestValue->ClearText();
		pEditTestVariable->ClearText();
	}
}
