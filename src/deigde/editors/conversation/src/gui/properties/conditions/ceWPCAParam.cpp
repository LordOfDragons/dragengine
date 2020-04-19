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

#include "ceWPCAParam.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/condition/ceCConditionActorParameter.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/condition/actorParameter/ceUCCAParamSetActor.h"
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

class cComboActor : public igdeComboBoxListener {
	ceWPCAParam &pPanel;
	
public:
	cComboActor( ceWPCAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || comboBox->GetText() == condition->GetActor() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCAParamSetActor( topic, action, condition, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cComboOperator : public igdeComboBoxListener {
	ceWPCAParam &pPanel;
	
public:
	cComboOperator( ceWPCAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCConditionActorParameter::eOperators newOperator =
			( ceCConditionActorParameter::eOperators )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( newOperator == condition->GetOperator() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCAParamSetOperator( topic, action, condition, newOperator ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextParameter : public igdeTextFieldListener {
	ceWPCAParam &pPanel;
	
public:
	cTextParameter( ceWPCAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || condition->GetParameter() == textField->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCAParamSetParameter( topic, action, condition, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTestValue : public igdeTextFieldListener {
	ceWPCAParam &pPanel;
	
public:
	cTextTestValue( ceWPCAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition ){
			return;
		}
		
		const int value = textField->GetInteger();
		if( value == condition->GetTestValue() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCAParamSetTestValue( topic, action, condition, value ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTestVariable : public igdeTextFieldListener {
	ceWPCAParam &pPanel;
	
public:
	cTextTestVariable( ceWPCAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionActorParameter * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || condition->GetTestVariable() == textField->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCAParamSetTestVariable( topic, action, condition, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPCAParam
//////////////////////

// Constructor, destructor
////////////////////////////

ceWPCAParam::ceWPCAParam( ceWPTopic &parentPanel ) : ceWPCondition( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	helper.ComboBox( *this, "Actor ID:", true, "Actor ID to test", pCBActorID, new cComboActor( *this ) );
	pCBActorID->SetDefaultSorter();
	
	helper.ComboBox( *this, "Operator:", "Operator to compare parameter value with test value",
		pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "Equal", NULL, ( void* )( intptr_t )ceCConditionActorParameter::eopEqual );
	pCBOperator->AddItem( "Not Equal", NULL, ( void* )( intptr_t )ceCConditionActorParameter::eopNotEqual );
	pCBOperator->AddItem( "Less", NULL, ( void* )( intptr_t )ceCConditionActorParameter::eopLess );
	pCBOperator->AddItem( "Less or Equal", NULL, ( void* )( intptr_t )ceCConditionActorParameter::eopLessEqual );
	pCBOperator->AddItem( "Greater", NULL, ( void* )( intptr_t )ceCConditionActorParameter::eopGreater );
	pCBOperator->AddItem( "Greater or Equal", NULL, ( void* )( intptr_t )ceCConditionActorParameter::eopGreaterEqual );
	
	helper.EditString( *this, "Parameter:", "Parameter to compare",
		pEditParameter, new cTextParameter( *this ) );
	helper.EditInteger( *this, "Test Value:", "Value to compare against",
		pEditTestValue, new cTextTestValue( *this ) );
	helper.EditString( *this, "Test Variable:",
		"Variable to compare against or empty string to use 'Test Value'",
		pEditTestVariable, new cTextTestVariable( *this ) );
}

ceWPCAParam::~ceWPCAParam(){
}



// Management
///////////////

ceCConditionActorParameter *ceWPCAParam::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetCondition();
	
	if( condition && condition->GetType() == ceConversationCondition::ectActorParameter ){
		return ( ceCConditionActorParameter* )condition;
		
	}else{
		return NULL;
	}
}

void ceWPCAParam::UpdateCondition(){
	const ceCConditionActorParameter * const condition = GetCondition();
	
	if( condition ){
		pCBActorID->SetText( condition->GetActor() );
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )condition->GetOperator() );
		pEditParameter->SetText( condition->GetParameter() );
		pEditTestValue->SetInteger( condition->GetTestValue() );
		pEditTestVariable->SetText( condition->GetTestVariable() );
		
	}else{
		pCBActorID->ClearText();
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )ceCConditionActorParameter::eopEqual );
		pEditParameter->ClearText();
		pEditTestValue->ClearText();
		pEditTestVariable->ClearText();
	}
}



void ceWPCAParam::UpdateActorIDList(){
	UpdateComboBoxWithActorIDList( pCBActorID );
}
