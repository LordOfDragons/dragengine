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

#include "ceWPCActorCommand.h"
#include "../ceWindowProperties.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/condition/ceCConditionActorCommand.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/condition/actorCommand/ceUCCACommandSetActor.h"
#include "../../../undosys/condition/actorCommand/ceUCCACommandSetCommand.h"
#include "../../../undosys/condition/actorCommand/ceUCCACommandToggleNegate.h"

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
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboActor : public igdeComboBoxListener {
	ceWPCActorCommand &pPanel;
	
public:
	cComboActor( ceWPCActorCommand &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorCommand * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || comboBox->GetText() == condition->GetActor() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCACommandSetActor( topic, action, condition, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};


class cTextCommand : public igdeTextFieldListener {
	ceWPCActorCommand &pPanel;
	
public:
	cTextCommand( ceWPCActorCommand &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorCommand * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || textField->GetText() == condition->GetCommand() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCACommandSetCommand( topic, action, condition, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionEditCommand : public igdeAction {
	ceWPCActorCommand &pPanel;
	
public:
	cActionEditCommand( ceWPCActorCommand &panel ) : igdeAction( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Edit command in larger dialog" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorCommand * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition ){
			return;
		}
		
		decString text( condition->GetCommand() );
		if( ! igdeCommonDialogs::GetMultilineString(
			&pPanel.GetParentPanel().GetWindowProperties().GetWindowMain(),
			"Edit Command", "Command:", text )
		|| text == condition->GetCommand() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCACommandSetCommand( topic, action, condition, text ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionNegate : public igdeAction {
	ceWPCActorCommand &pPanel;
	
public:
	cActionNegate( ceWPCActorCommand &panel ) : igdeAction( "Negate", NULL,
		"True if the result of the command is negated" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorCommand * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCACommandToggleNegate( topic, action, condition ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}


// Class ceWPCActorCommand
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPCActorCommand::ceWPCActorCommand( ceWPTopic &parentPanel ) : ceWPCondition( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	helper.ComboBox( *this, "Actor ID:", true, "Actor ID to test", pCBActorID, new cComboActor( *this ) );
	pCBActorID->SetDefaultSorter();
	
	helper.FormLineStretchFirst( *this, "Command:", "Command to send", formLine );
	helper.EditString( formLine, "Command to send", pEditCommand, new cTextCommand( *this ) );
	helper.Button( formLine, pBtnCommand, new cActionEditCommand( *this ), true );
	
	helper.CheckBox( formLine, pChkNegate, new cActionNegate( *this ), true );
}

ceWPCActorCommand::~ceWPCActorCommand(){
}



// Management
///////////////

ceCConditionActorCommand *ceWPCActorCommand::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if( condition && condition->GetType() == ceConversationCondition::ectActorCommand ){
		return ( ceCConditionActorCommand* )condition;
		
	}else{
		return NULL;
	}
}

void ceWPCActorCommand::UpdateCondition(){
	const ceCConditionActorCommand * const condition = GetCondition();
	
	if( condition ){
		pCBActorID->SetText( condition->GetActor() );
		pEditCommand->SetText( condition->GetCommand() );
		pChkNegate->SetChecked( condition->GetNegate() );
		
	}else{
		pCBActorID->ClearText();
		pEditCommand->ClearText();
		pChkNegate->SetChecked( false );
	}
}



void ceWPCActorCommand::UpdateActorIDList(){
	UpdateComboBoxWithActorIDList( pCBActorID );
}
