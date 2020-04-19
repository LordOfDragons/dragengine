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

class cComboActorID : public igdeComboBoxListener {
	ceWPASetAParam &pPanel;
	
public:
	cComboActorID( ceWPASetAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetActor() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetAParamSetActor( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextName : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	cTextName( ceWPASetAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetName() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetAParamSetName( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cComboOperator : public igdeComboBoxListener {
	ceWPASetAParam &pPanel;
	
public:
	cComboOperator( ceWPASetAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if( ! topic || ! action || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCASetActorParameter::eOperators newOperator =
			( ceCASetActorParameter::eOperators )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( newOperator == action->GetOperator() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetAParamSetOp( topic, action, newOperator ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextValue : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	cTextValue( ceWPASetAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		const int value = textField->GetInteger();
		if( ! topic || ! action || value == action->GetValue() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetAParamSetValue( topic, action, value ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextValueVariable : public igdeTextFieldListener {
	ceWPASetAParam &pPanel;
	
public:
	cTextValueVariable( ceWPASetAParam &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASetActorParameter * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetValueVariable() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASetAParamSetValueVariable( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPASetAParam
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPASetAParam::ceWPASetAParam( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon( *this );
	
	helper.ComboBox( *this, "Actor ID:", true, "Actor to modify", pCBActorID, new cComboActorID( *this ) );
	pCBActorID->SetDefaultSorter();
	
	helper.EditString( *this, "Name:", "Name of the parameter to set or empty to have no effect",
		pEditName, new cTextName( *this ) );
	
	helper.ComboBox( *this, "Operator:", "How to apply the value to the parameter",
		pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "Set", NULL, ( void* )( intptr_t )ceCASetActorParameter::eopSet );
	pCBOperator->AddItem( "Increment", NULL, ( void* )( intptr_t )ceCASetActorParameter::eopIncrement );
	pCBOperator->AddItem( "Decrement", NULL, ( void* )( intptr_t )ceCASetActorParameter::eopDecrement );
	pCBOperator->AddItem( "Random", NULL, ( void* )( intptr_t )ceCASetActorParameter::eopRandom );
	
	helper.EditInteger( *this, "Value:", "Value to use for operation (integer value)",
		pEditValue, new cTextValue( *this ) );
	helper.EditString( *this, "Variable:", "Variable value to use for operation or empty string to use 'Value'",
		pEditValueVariable, new cTextValueVariable( *this ) );
}

ceWPASetAParam::~ceWPASetAParam(){
}



// Management
///////////////

ceCASetActorParameter *ceWPASetAParam::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetAction();
	
	if( action && action->GetType() == ceConversationAction::eatSetActorParameter ){
		return ( ceCASetActorParameter* )action;
		
	}else{
		return NULL;
	}
}

void ceWPASetAParam::UpdateAction(){
	const ceCASetActorParameter * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBActorID->SetText( action->GetActor() );
		pEditName->SetText( action->GetName() );
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )action->GetOperator() );
		pEditValue->SetInteger( action->GetValue() );
		pEditValueVariable->SetText( action->GetValueVariable() );
		
	}else{
		pCBActorID->ClearText();
		pEditName->ClearText();
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )ceCASetActorParameter::eopSet );
		pEditValue->ClearText();
		pEditValueVariable->ClearText();
	}
}



void ceWPASetAParam::UpdateActorIDLists(){
	ceWPAction::UpdateActorIDLists();
	UpdateComboBoxWithActorIDList( pCBActorID );
}
