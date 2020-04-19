/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include <deigde/undo/igdeUndoReference.h>
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
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCConditionVariable::eOperators newOperator =
			( ceCConditionVariable::eOperators )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( newOperator == condition->GetOperator() ){
			return;
		}
		
		igdeUndoReference undo;
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
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || condition->GetVariable() == textField->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
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
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition ){
			return;
		}
		
		const int value = textField->GetInteger();
		if( value == condition->GetTestValue() ){
			return;
		}
		
		igdeUndoReference undo;
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
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionVariable * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || condition->GetTestVariable() == textField->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
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
	ceConversationCondition * const condition = pParentPanel.GetCondition();
	
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
