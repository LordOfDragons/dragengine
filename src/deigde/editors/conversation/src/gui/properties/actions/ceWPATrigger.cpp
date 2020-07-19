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

#include "ceWPATrigger.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCATrigger.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/trigger/ceUCATriggerSetName.h"
#include "../../../undosys/action/trigger/ceUCATriggerSetAction.h"

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
	ceWPATrigger &pPanel;
	
public:
	cTextName( ceWPATrigger &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCATrigger * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetName() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCATriggerSetName( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cComboAction : public igdeComboBoxListener {
	ceWPATrigger &pPanel;
	
public:
	cComboAction( ceWPATrigger &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCATrigger * const action = pPanel.GetAction();
		if( ! topic || ! action || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCATrigger::eActions triggerAction = ( ceCATrigger::eActions )( intptr_t )
			comboBox->GetSelectedItem()->GetData();
		if( triggerAction == action->GetAction() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCATriggerSetAction( topic, action, triggerAction ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPATrigger
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPATrigger::ceWPATrigger( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon( *this );
	
	helper.EditString( *this, "Name:", "Name of the trigger to operate or empty to have no effect",
		pEditName, new cTextName( *this ) );
	
	helper.ComboBox( *this, "Action:", "The action to apply on the trigger",
		pCBAction, new cComboAction( *this ) );
	pCBAction->AddItem( "Fire", NULL, ( void* )( intptr_t )ceCATrigger::eaFire );
	pCBAction->AddItem( "Reset", NULL, ( void* )( intptr_t )ceCATrigger::eaReset );
	pCBAction->AddItem( "Pulse", NULL, ( void* )( intptr_t )ceCATrigger::eaPulse );
	pCBAction->AddItem( "Full Reset", NULL, ( void* )( intptr_t )ceCATrigger::eaFullReset );
}

ceWPATrigger::~ceWPATrigger(){
}



// Management
///////////////

ceCATrigger *ceWPATrigger::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if( action && action->GetType() == ceConversationAction::eatTrigger ){
		return ( ceCATrigger* )action;
		
	}else{
		return NULL;
	}
}

void ceWPATrigger::UpdateAction(){
	const ceCATrigger * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pEditName->SetText( action->GetName() );
		pCBAction->SetSelectionWithData( ( void* )( intptr_t )action->GetAction() );
		
	}else{
		pEditName->ClearText();
		pCBAction->SetSelectionWithData( ( void* )( intptr_t )ceCATrigger::eaFire );
	}
}
