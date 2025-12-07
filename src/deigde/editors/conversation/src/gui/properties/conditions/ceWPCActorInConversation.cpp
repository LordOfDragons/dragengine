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

#include "ceWPCActorInConversation.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/condition/ceCConditionActorInConversation.h"
#include "../../../undosys/condition/actorInConversation/ceUCCAInConvoSetActor.h"
#include "../../../undosys/condition/actorInConversation/ceUCCAInConvoToggleNegate.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboActor : public igdeComboBoxListener {
	ceWPCActorInConversation &pPanel;
	
public:
	cComboActor(ceWPCActorInConversation &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorInConversation * const condition = pPanel.GetCondition();
		if(! topic || ! action || ! condition || comboBox->GetText() == condition->GetActor()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCAInConvoSetActor::Ref::NewWith(topic, action, condition, comboBox->GetText()));
	}
};

class cActionNegate : public igdeAction {
	ceWPCActorInConversation &pPanel;
	
public:
	cActionNegate(ceWPCActorInConversation &panel) : igdeAction("Negate", NULL,
		"True if the information is missing instead of existing"), pPanel(panel){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionActorInConversation * const condition = pPanel.GetCondition();
		if(! topic || ! action || ! condition){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCCAInConvoToggleNegate::Ref::NewWith(topic, action, condition));
	}
};

}



// Class ceWPCActorInConversation
///////////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPCActorInConversation::ceWPCActorInConversation(ceWPTopic &parentPanel) :
ceWPCondition(parentPanel){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	helper.ComboBox(*this, "Actor ID:", true, "Actor ID to test", pCBActor, new cComboActor(*this));
	pCBActor->SetDefaultSorter();
		
	helper.CheckBox(*this, pChkNegate, new cActionNegate(*this), true);
}

ceWPCActorInConversation::~ceWPCActorInConversation(){
}



// Management
///////////////

ceCConditionActorInConversation *ceWPCActorInConversation::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if(condition && condition->GetType() == ceConversationCondition::ectActorInConversation){
		return (ceCConditionActorInConversation*)condition;
		
	}else{
		return NULL;
	}
}

void ceWPCActorInConversation::UpdateCondition(){
	const ceCConditionActorInConversation * const condition = GetCondition();
	
	if(condition){
		pCBActor->SetText(condition->GetActor());
		pChkNegate->SetChecked(condition->GetNegate());
		
	}else{
		pCBActor->ClearText();
		pChkNegate->SetChecked(false);
	}
}

void ceWPCActorInConversation::UpdateActorList(){
	UpdateComboBoxWithActorIDList(pCBActor);
}
