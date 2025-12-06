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

#include "ceWPCGameCommand.h"
#include "../ceWindowProperties.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/condition/ceCConditionGameCommand.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/condition/gameCommand/ceUCCGameCmdSetCommand.h"
#include "../../../undosys/condition/gameCommand/ceUCCGameCmdToggleNegate.h"

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

class cTextCommand : public igdeTextFieldListener {
	ceWPCGameCommand &pPanel;
	
public:
	cTextCommand( ceWPCGameCommand &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionGameCommand * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || textField->GetText() == condition->GetCommand() ){
			return;
		}
		
		ceUCCGameCmdSetCommand::Ref undo(ceUCCGameCmdSetCommand::Ref::NewWith(
			topic, action, condition, textField->GetText()));
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionEditCommand : public igdeAction {
	ceWPCGameCommand &pPanel;
	
public:
	cActionEditCommand( ceWPCGameCommand &panel ) : igdeAction( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Edit command in larger dialog" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionGameCommand * const condition = pPanel.GetCondition();
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
		
		ceUCCGameCmdSetCommand::Ref undo(ceUCCGameCmdSetCommand::Ref::NewWith(
			topic, action, condition, text));
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionNegate : public igdeAction {
	ceWPCGameCommand &pPanel;
	
public:
	cActionNegate( ceWPCGameCommand &panel ) : igdeAction( "Negate", NULL,
		"True if the result of the command is negated" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionGameCommand * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition ){
			return;
		}
		
		ceUCCGameCmdToggleNegate::Ref undo(ceUCCGameCmdToggleNegate::Ref::NewWith(
			topic, action, condition));
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}


// Class ceWPCGameCommand
///////////////////////////

// Constructor, destructor
////////////////////////////

ceWPCGameCommand::ceWPCGameCommand( ceWPTopic &parentPanel ) : ceWPCondition( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	helper.FormLineStretchFirst( *this, "Command:", "Command to send", formLine );
	helper.EditString( formLine, "Command to send", pEditCommand, new cTextCommand( *this ) );
	helper.Button( formLine, pBtnCommand, new cActionEditCommand( *this ), true );
	
	helper.CheckBox( formLine, pChkNegate, new cActionNegate( *this ), true );
}

ceWPCGameCommand::~ceWPCGameCommand(){
}



// Management
///////////////

ceCConditionGameCommand *ceWPCGameCommand::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if( condition && condition->GetType() == ceConversationCondition::ectGameCommand ){
		return ( ceCConditionGameCommand* )condition;
		
	}else{
		return NULL;
	}
}

void ceWPCGameCommand::UpdateCondition(){
	const ceCConditionGameCommand * const condition = GetCondition();
	
	if( condition ){
		pEditCommand->SetText( condition->GetCommand() );
		pChkNegate->SetChecked( condition->GetNegate() );
		
	}else{
		pEditCommand->ClearText();
		pChkNegate->SetChecked( false );
	}
}
