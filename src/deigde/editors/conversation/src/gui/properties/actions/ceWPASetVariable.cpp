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

#include "ceWPASetVariable.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCASetVariable.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/setVariable/ceUCASetVarSetName.h"
#include "../../../undosys/action/setVariable/ceUCASetVarSetOp.h"
#include "../../../undosys/action/setVariable/ceUCASetVarSetValue.h"
#include "../../../undosys/action/setVariable/ceUCASetVarSetValueVariable.h"

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

class cTextName : public igdeTextFieldListener {
	ceWPASetVariable &pPanel;
	
public:
	cTextName( ceWPASetVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetVariable * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetName() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetVarSetName( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cComboOperator : public igdeComboBoxListener {
	ceWPASetVariable &pPanel;
	
public:
	cComboOperator( ceWPASetVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetVariable * const action = pPanel.GetAction();
		if( ! topic || ! action || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCASetVariable::eOperators newOperator =
			( ceCASetVariable::eOperators )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( newOperator == action->GetOperator() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetVarSetOp( topic, action, newOperator ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextValue : public igdeTextFieldListener {
	ceWPASetVariable &pPanel;
	
public:
	cTextValue( ceWPASetVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetVariable * const action = pPanel.GetAction();
		const int value = textField->GetInteger();
		if( ! topic || ! action || value == action->GetValue() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetVarSetValue( topic, action, value ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextValueVariable : public igdeTextFieldListener {
	ceWPASetVariable &pPanel;
	
public:
	cTextValueVariable( ceWPASetVariable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetVariable * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetValueVariable() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetVarSetValueVariable( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPASetVariable
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPASetVariable::ceWPASetVariable( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon( *this );
	
	helper.EditString( *this, "Name:", "Name of the variable to set or empty to have no effect",
		pEditName, new cTextName( *this ) );
	
	helper.ComboBox( *this, "Operator:", "How to apply the value to the variable",
		pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "Set", NULL, ( void* )( intptr_t )ceCASetVariable::eopSet );
	pCBOperator->AddItem( "Increment", NULL, ( void* )( intptr_t )ceCASetVariable::eopIncrement );
	pCBOperator->AddItem( "Decrement", NULL, ( void* )( intptr_t )ceCASetVariable::eopDecrement );
	pCBOperator->AddItem( "Random", NULL, ( void* )( intptr_t )ceCASetVariable::eopRandom );
	
	helper.EditInteger( *this, "Value:", "Value to use for operation (integer value)",
		pEditValue, new cTextValue( *this ) );
	helper.EditString( *this, "Variable:", "Variable value to use for operation or empty string to use 'Value'",
		pEditValueVariable, new cTextValueVariable( *this ) );
}

ceWPASetVariable::~ceWPASetVariable(){
}



// Management
///////////////

ceCASetVariable *ceWPASetVariable::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if( action && action->GetType() == ceConversationAction::eatSetVariable ){
		return ( ceCASetVariable* )action;
		
	}else{
		return NULL;
	}
}

void ceWPASetVariable::UpdateAction(){
	const ceCASetVariable * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pEditName->SetText( action->GetName() );
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )action->GetOperator() );
		pEditValue->SetInteger( action->GetValue() );
		pEditValueVariable->SetText( action->GetValueVariable() );
		
	}else{
		pEditName->ClearText();
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )ceCASetVariable::eopSet );
		pEditValue->ClearText();
		pEditValueVariable->ClearText();
	}
}
