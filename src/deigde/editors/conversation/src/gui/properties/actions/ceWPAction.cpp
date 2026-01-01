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

#include "ceWPAction.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/coordsystem/ceCoordSystem.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/ceUCActionToggleWaitForActor.h"
#include "../../../undosys/action/ceUCActionToggleWaitSpeakOnly.h"
#include "../../../undosys/action/ceUCActionSetWaitForActorID.h"
#include "../../../undosys/action/ceUCActionSetDelay.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
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

class cTextDelay : public igdeTextFieldListener {
	ceWPAction &pPanel;
	
public:
	typedef deTObjectReference<cTextDelay> Ref;
	cTextDelay(ceWPAction &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		if(!topic || !action){
			return;
		}
		
		const float delay = textField->GetFloat();
		if(fabsf(delay - action->GetDelay()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCActionSetDelay::Ref::New(topic, action, delay));
	}
};

class cActionWaitForActor : public igdeAction{
public:
	typedef deTObjectReference<cActionWaitForActor> Ref;
	
private:
	ceWPAction &pPanel;
	
public:
	cActionWaitForActor(ceWPAction &panel) : igdeAction("Wait for actor",
		"Wait for actor(s) to finish speaking and return false if asked to wait"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		if(!topic || !action){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCActionToggleWaitForActor::Ref::New(topic, action));
	}
	
	void Update() override{
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		SetEnabled(action);
		SetSelected(action && action->GetWaitForActor());
	}
};

class cActionWaitSpeakOnly : public igdeAction{
public:
	typedef deTObjectReference<cActionWaitSpeakOnly> Ref;
	
private:
	ceWPAction &pPanel;
	
public:
	cActionWaitSpeakOnly(ceWPAction &panel) : igdeAction("Wait speak only",
		"Waiting considers only actor speaking not actor waiting"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		if(!topic || !action){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCActionToggleWaitSpeakOnly::Ref::New(topic, action));
	}
	
	void Update() override{
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		SetEnabled(action);
		SetSelected(action && action->GetWaitSpeakOnly());
	}
};

class cComboWaitForActorID : public igdeComboBoxListener {
	ceWPAction &pPanel;
	
public:
	typedef deTObjectReference<cComboWaitForActorID> Ref;
	cComboWaitForActorID(ceWPAction &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		if(!topic || !action || comboBox->GetText() == action->GetWaitForActorID()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCActionSetWaitForActorID::Ref::New(topic, action, comboBox->GetText()));
	}
};

}



// Class ceWPAction
/////////////////////

// Constructor, destructor
////////////////////////////

ceWPAction::ceWPAction(ceWPTopic &parentPanel) :
igdeContainerForm(parentPanel.GetEnvironment()),
pParentPanel(parentPanel){
}

ceWPAction::~ceWPAction(){
}



// Management
///////////////

void ceWPAction::CreateGUICommon(igdeContainerForm &container){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	helper.EditFloat(container, "Delay:",
		"Delay the processing of the action after waiting conditions are fulfilled",
		pEditDelay, cTextDelay::Ref::New(*this));
	
	helper.CheckBoxOnly(container, pChkWaitForActor, cActionWaitForActor::Ref::New(*this));
	helper.ComboBox(container, true, "ID of actor to wait for or empty string to wait for all actors",
		pCBWaitForActorID, cComboWaitForActorID::Ref::New(*this));
	
	helper.CheckBox(container, pChkWaitSpeakOnly, cActionWaitSpeakOnly::Ref::New(*this));
}

void ceWPAction::UpdateCommonParams(){
	if(!pEditDelay){
		return;
	}
	
	const ceConversationAction * const action = pParentPanel.GetTreeAction();
	
	if(action){
		pEditDelay->SetFloat(action->GetDelay());
		pCBWaitForActorID->SetText(action->GetWaitForActorID());
		
	}else{
		pEditDelay->ClearText();
		pCBWaitForActorID->ClearText();
	}
	
	pChkWaitForActor->GetAction()->Update();
	pChkWaitSpeakOnly->GetAction()->Update();
}



void ceWPAction::UpdateComboBoxWithActorIDList(igdeComboBox &combobox){
	const ceConversation * const conversation = pParentPanel.GetConversation();
	const decString selection(combobox.GetText());
	
	combobox.RemoveAllItems();
	
	if(conversation){
		const ceConversationActorList &list = conversation->GetActorList();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			if(!list.GetAt(i)->GetID().IsEmpty()){
				const decString &id = list.GetAt(i)->GetID();
				const decString &aliasID = list.GetAt(i)->GetAliasID();
				
				if(!combobox.HasItem(id)){
					combobox.AddItem(id);
				}
				if(!aliasID.IsEmpty() && !combobox.HasItem(aliasID)){
					combobox.AddItem(aliasID);
				}
			}
		}
		
		combobox.SortItems();
	}
	
	combobox.SetText(selection);
}

void ceWPAction::UpdateComboBoxWithConvoCoordSysIDList(igdeComboBox &combobox){
	const ceConversation * const conversation = pParentPanel.GetConversation();
	const decString selection(combobox.GetText());
	
	combobox.RemoveAllItems();
	
	if(conversation){
		const ceCoordSystemList &list = conversation->GetCoordSystemList();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			if(!list.GetAt(i)->GetID().IsEmpty()){
				const decString &id = list.GetAt(i)->GetID();
				const decString &aliasID = list.GetAt(i)->GetAliasID();
				
				if(!combobox.HasItem(id)){
					combobox.AddItem(id);
				}
				if(!aliasID.IsEmpty() && !combobox.HasItem(aliasID)){
					combobox.AddItem(aliasID);
				}
			}
		}
		
		combobox.SortItems();
	}
	
	combobox.SetText(selection);
}

void ceWPAction::UpdateActorIDLists(){
	// check for nullptr since the widgets exists only if CreateGUICommon() has been called
	if(pCBWaitForActorID){
		UpdateComboBoxWithActorIDList(pCBWaitForActorID);
	}
}

void ceWPAction::OnConversationPathChanged(){
}
