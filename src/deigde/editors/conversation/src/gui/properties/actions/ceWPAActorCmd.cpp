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

#include "ceWPAActorCmd.h"
#include "../ceWPTopic.h"
#include "../ceWindowProperties.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorCommand.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/actorCommand/ceUCAACmdSetActor.h"
#include "../../../undosys/action/actorCommand/ceUCAACmdSetCommand.h"

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

class cComboActor : public igdeComboBoxListener {
	ceWPAActorCmd &pPanel;
	
public:
	using Ref = deTObjectReference<cComboActor>;
	cComboActor(ceWPAActorCmd &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorCommand * const action = pPanel.GetAction();
		if(!topic || !action  || comboBox->GetText() == action->GetActor()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAACmdSetActor::Ref::New(topic, action, comboBox->GetText()));
	}
};

class cTextCommand : public igdeTextFieldListener {
	ceWPAActorCmd &pPanel;
	
public:
	using Ref = deTObjectReference<cTextCommand>;
	cTextCommand(ceWPAActorCmd &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorCommand * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetCommand()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAACmdSetCommand::Ref::New(topic, action, textField->GetText()));
	}
};

class cActionEditCommand : public igdeAction {
	ceWPAActorCmd &pPanel;
	
public:
	using Ref = deTObjectReference<cActionEditCommand>;
	cActionEditCommand(ceWPAActorCmd &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"@Conversation.Action.EditInDialog.ToolTip"), pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorCommand * const action = pPanel.GetAction();
		if(!topic || !action){
			return;
		}
		
		decString text(action->GetCommand());
		if(!igdeCommonDialogs::GetMultilineString(
			pPanel.GetParentPanel().GetWindowProperties().GetWindowMain(),
			"@Conversation.Dialog.EditCommand", "@Conversation.Dialog.Command", text)
		|| text == action->GetCommand()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAACmdSetCommand::Ref::New(topic, action, text));
	}
};

}


// Class ceWPAActorCmd
////////////////////////

// Constructor, destructor
////////////////////////////

ceWPAActorCmd::ceWPAActorCmd(ceWPTopic &parentPanel) : ceWPAction(parentPanel){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	CreateGUICommon(*this);
	
	helper.ComboBox(*this, "@Conversation.WPActionActorCmd.ActorID.Label", true, "@Conversation.ToolTip.ActorToSendCommand", pCBActorID, cComboActor::Ref::New(*this));
	pCBActorID->SetDefaultSorter();
	
	helper.FormLineStretchFirst(*this, "@Conversation.FormLine.Command", "@Conversation.WPActionActorCmd.Commandtosend.Label", formLine);
	helper.EditString(formLine, "@Conversation.WPActionActorCmd.Commandtosend.Label", pEditCommand, cTextCommand::Ref::New(*this));
	helper.Button(formLine, pBtnCommand, cActionEditCommand::Ref::New(*this));
}

ceWPAActorCmd::~ceWPAActorCmd(){
}



// Management
///////////////

ceCAActorCommand *ceWPAActorCmd::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if(action && action->GetType() == ceConversationAction::eatActorCommand){
		return (ceCAActorCommand*)action;
		
	}else{
		return nullptr;
	}
}

void ceWPAActorCmd::UpdateAction(){
	const ceCAActorCommand * const action = GetAction();
	
	UpdateCommonParams();
	
	if(action){
		pCBActorID->SetText(action->GetActor());
		pEditCommand->SetText(action->GetCommand());
		
	}else{
		pCBActorID->ClearText();
		pEditCommand->ClearText();
	}
}



void ceWPAActorCmd::UpdateActorIDLists(){
	ceWPAction::UpdateActorIDLists();
	UpdateComboBoxWithActorIDList(pCBActorID);
}
