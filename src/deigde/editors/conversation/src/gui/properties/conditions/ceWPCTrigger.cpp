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

class cComboTestMode : public igdeComboBoxListener {
	ceWPCTrigger &pPanel;
	
public:
	cComboTestMode( ceWPCTrigger &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionTrigger * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCConditionTrigger::eTestModes testMode =
			( ceCConditionTrigger::eTestModes )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( testMode == condition->GetTestMode() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCTriggerSetTestMode( topic, action, condition, testMode ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTrigger : public igdeTextFieldListener {
	ceWPCTrigger &pPanel;
	
public:
	cTextTrigger( ceWPCTrigger &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionTrigger * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || condition->GetTrigger() == textField->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCTriggerSetTrigger( topic, action, condition, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}


// Class ceWPCTrigger
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPCTrigger::ceWPCTrigger( ceWPTopic &parentPanel ) : ceWPCondition( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	helper.ComboBox( *this, "Test Mode:", "How to test the trigger",
		pCBTestMode, new cComboTestMode( *this ) );
	pCBTestMode->AddItem( "Fired", NULL, ( void* )( intptr_t )ceCConditionTrigger::etmFired );
	pCBTestMode->AddItem( "Not Fired", NULL, ( void* )( intptr_t )ceCConditionTrigger::etmNotFired );
	pCBTestMode->AddItem( "Has Ever Fired", NULL, ( void* )( intptr_t )ceCConditionTrigger::etmEverFired );
	pCBTestMode->AddItem( "Has Never Fired", NULL, ( void* )( intptr_t )ceCConditionTrigger::etmNeverFired );
	
	helper.EditString( *this, "Trigger:", "Name of trigger to test", pEditTrigger, new cTextTrigger( *this ) );
}

ceWPCTrigger::~ceWPCTrigger(){
}



// Management
///////////////

ceCConditionTrigger *ceWPCTrigger::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if( condition && condition->GetType() == ceConversationCondition::ectTrigger ){
		return ( ceCConditionTrigger* )condition;
		
	}else{
		return NULL;
	}
}

void ceWPCTrigger::UpdateCondition(){
	const ceCConditionTrigger * const condition = GetCondition();
	
	if( condition ){
		pCBTestMode->SetSelectionWithData( ( void* )( intptr_t )condition->GetTestMode() );
		pEditTrigger->SetText( condition->GetTrigger() );
		
	}else{
		pCBTestMode->SetSelectionWithData( ( void* )( intptr_t )ceCConditionTrigger::etmFired );
		pEditTrigger->ClearText();
	}
}
