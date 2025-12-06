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

#include "ceWPAGameCommand.h"
#include "../ceWindowProperties.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAGameCommand.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/gameCommand/ceUCAGameCmdSetCommand.h"

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
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextCommand : public igdeTextFieldListener {
	ceWPAGameCommand &pPanel;
	
public:
	cTextCommand( ceWPAGameCommand &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAGameCommand * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetCommand() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCAGameCmdSetCommand( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionEditCommand : public igdeAction {
	ceWPAGameCommand &pPanel;
	
public:
	cActionEditCommand( ceWPAGameCommand &panel ) : igdeAction( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Edit command in larger dialog" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAGameCommand * const action = pPanel.GetAction();
		if( ! topic || ! action ){
			return;
		}
		
		decString text( action->GetCommand() );
		if( ! igdeCommonDialogs::GetMultilineString(
			&pPanel.GetParentPanel().GetWindowProperties().GetWindowMain(),
			"Edit Command", "Command:", text )
		|| text == action->GetCommand() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCAGameCmdSetCommand( topic, action, text ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPAGameCommand
////////////////////////

// Constructor, destructor
////////////////////////////

ceWPAGameCommand::ceWPAGameCommand( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	CreateGUICommon( *this );
	
	helper.FormLineStretchFirst( *this, "Command:", "Command to send", formLine );
	helper.EditString( formLine, "Command to send", pEditCommand, new cTextCommand( *this ) );
	helper.Button( formLine, pBtnCommand, new cActionEditCommand( *this ), true );
}

ceWPAGameCommand::~ceWPAGameCommand(){
}



// Management
///////////////

ceCAGameCommand *ceWPAGameCommand::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if( action && action->GetType() == ceConversationAction::eatGameCommand ){
		return ( ceCAGameCommand* )action;
		
	}else{
		return NULL;
	}
}

void ceWPAGameCommand::UpdateAction(){
	const ceCAGameCommand * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pEditCommand->SetText( action->GetCommand() );
		
	}else{
		pEditCommand->ClearText();
	}
}
