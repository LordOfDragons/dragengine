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

#include "ceWPAActorCmd.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorCommand.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/actorCommand/ceUCAACmdSetActor.h"
#include "../../../undosys/action/actorCommand/ceUCAACmdSetActor.h"
#include "../../../undosys/action/actorCommand/ceUCAACmdSetCommand.h"

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
	ceWPAActorCmd &pPanel;
	
public:
	cComboActor( ceWPAActorCmd &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorCommand * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetActor() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAACmdSetActor( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextCommand : public igdeTextFieldListener {
	ceWPAActorCmd &pPanel;
	
public:
	cTextCommand( ceWPAActorCmd &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorCommand * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetCommand() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAACmdSetCommand( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionEditCommand : public igdeAction {
	ceWPAActorCmd &pPanel;
	
public:
	cActionEditCommand( ceWPAActorCmd &panel ) : igdeAction( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Edit command in larger dialog" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorCommand * const action = pPanel.GetAction();
		if( ! topic || ! action ){
			return;
		}
		
		decString text( action->GetCommand() );
		if( ! igdeCommonDialogs::GetMultilineString( &pPanel, "Edit Command", "Command:", text )
		|| text == action->GetCommand() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAACmdSetCommand( topic, action, text ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}


// Class ceWPAActorCmd
////////////////////////

// Constructor, destructor
////////////////////////////

ceWPAActorCmd::ceWPAActorCmd( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	CreateGUICommon( *this );
	
	helper.ComboBox( *this, "Actor ID:", true, "Actor to send command to", pCBActorID, new cComboActor( *this ) );
	pCBActorID->SetDefaultSorter();
	
	helper.FormLineStretchFirst( *this, "Command:", "Command to send", formLine );
	helper.EditString( formLine, "Command to send", pEditCommand, new cTextCommand( *this ) );
	helper.Button( formLine, pBtnCommand, new cActionEditCommand( *this ), true );
}

ceWPAActorCmd::~ceWPAActorCmd(){
}



// Management
///////////////

ceCAActorCommand *ceWPAActorCmd::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetAction();
	
	if( action && action->GetType() == ceConversationAction::eatActorCommand ){
		return ( ceCAActorCommand* )action;
		
	}else{
		return NULL;
	}
}

void ceWPAActorCmd::UpdateAction(){
	const ceCAActorCommand * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBActorID->SetText( action->GetActor() );
		pEditCommand->SetText( action->GetCommand() );
		
	}else{
		pCBActorID->ClearText();
		pEditCommand->ClearText();
	}
}



void ceWPAActorCmd::UpdateActorIDLists(){
	ceWPAction::UpdateActorIDLists();
	UpdateComboBoxWithActorIDList( pCBActorID );
}
