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

#include "aeWPAPanelRuleStateSnapshot.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleStateSnapshot.h"
#include "../../../undosys/rule/snapshot/aeUSetRuleSnapUseLast.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapSetID.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapToggleEnablePosition.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapToggleEnableRotation.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapToggleEnableSize.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapToggleEnableVertexPositionSet.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleStateSnapshot &pPanel;
	
public:
	cBaseAction(aeWPAPanelRuleStateSnapshot &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = (aeRuleStateSnapshot*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeRuleStateSnapshot *rule) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = (aeRuleStateSnapshot*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleStateSnapshot &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
	aeWPAPanelRuleStateSnapshot &pPanel;
	
public:
	cBaseTextFieldListener(aeWPAPanelRuleStateSnapshot &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = (aeRuleStateSnapshot*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleStateSnapshot *rule) = 0;
};


class cActionUseLastState : public cBaseAction{
public:
	cActionUseLastState(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"Use last animation state", NULL,
		"Determines if the animation state from the last frame update is used or the stored snapshot"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleStateSnapshot *rule){
		return new aeUSetRuleSnapUseLast(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetUseLastState());
	}
};

class cTextID : public cBaseTextFieldListener{
public:
	cTextID(aeWPAPanelRuleStateSnapshot &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeRuleStateSnapshot *rule){
		const int value = textField->GetInteger();
		return rule->GetID() != value ? new aeURuleSnapSetID(rule, value) : NULL;
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleStateSnapshot *rule){
		return new aeURuleSnapToggleEnablePosition(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleStateSnapshot *rule){
		return new aeURuleSnapToggleEnableRotation(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleStateSnapshot *rule){
		return new aeURuleSnapToggleEnableSize(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if vertex position set is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleStateSnapshot *rule){
		return new aeURuleSnapToggleEnableVertexPositionSet(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

class cActionSnapshot : public cBaseAction{
public:
	cActionSnapshot(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel, "State", NULL,
		"Takes a snapshot of the current animation state"){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeRuleStateSnapshot *rule){
		deAnimatorInstance * const engAnimatorInstance = animator->GetEngineAnimatorInstance();
		if(engAnimatorInstance){
			engAnimatorInstance->CaptureStateInto(rule->GetID());
		}
		return NULL;
	}
};

class cActionGetFrame : public cBaseAction{
public:
	cActionGetFrame(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel, "Animation", NULL,
		"Takes a snapshot of the selected animation frame"){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeRuleStateSnapshot *rule){
		deAnimatorInstance * const engAnimatorInstance = animator->GetEngineAnimatorInstance();
		const decString moveName(pPanel.GetCBMoveNameText());
		if(engAnimatorInstance){
			engAnimatorInstance->StoreFrameInto(rule->GetID(), moveName, pPanel.GetTextMoveTime());
		}
		return NULL;
	}
	
	void Update(const aeAnimator &, const aeRuleStateSnapshot &) override{
		SetEnabled(!pPanel.GetCBMoveNameText().IsEmpty());
	}
};

}



// Class aeWPAPanelRuleStateSnapshot
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleStateSnapshot::aeWPAPanelRuleStateSnapshot(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertStateSnapshot)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, formLine;
	
	
	helper.GroupBox(*this, groupBox, "State Snapshot:");
	
	helper.CheckBox(groupBox, pChkUseLastState, new cActionUseLastState(*this), true);
	helper.EditInteger(groupBox, "ID:", "Identifier to snap state during runtime",
		pEditID, new cTextID(*this));
	
	helper.ComboBoxFilter(groupBox, "Move Name:", true, "Name of animation move to use",
		pCBMoveName, NULL);
	pCBMoveName->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "Move Time:", "Time in seconds of animation frame to retrieve",
		pEditMoveTime, NULL);
	
	helper.CheckBox(groupBox, pChkEnablePosition, new cActionEnablePosition(*this), true);
	helper.CheckBox(groupBox, pChkEnableRotation, new cActionEnableRotation(*this), true);
	helper.CheckBox(groupBox, pChkEnableSize, new cActionEnableSize(*this), true);
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, new cActionEnableVertexPositionSet(*this), true);
	
	helper.FormLine(groupBox, "Snapshot:", "Store snapshot (runtime simulation)", formLine);
	helper.Button(formLine, pBtnSnapshot, new cActionSnapshot(*this), true);
	helper.Button(formLine, pBtnGetFrame, new cActionGetFrame(*this), true);
}

aeWPAPanelRuleStateSnapshot::~aeWPAPanelRuleStateSnapshot(){
}



// Management
///////////////

void aeWPAPanelRuleStateSnapshot::UpdateAnimMoveList(){
	aeWPAPanelRule::UpdateAnimMoveList();
	
	const decString moveName(pCBMoveName->GetText());
	
	pCBMoveName->RemoveAllItems();
	
	if(GetAnimator()){
		const deAnimation * const engAnimation = GetAnimator()->GetEngineAnimator()
			? GetAnimator()->GetEngineAnimator()->GetAnimation() : NULL;
		if(engAnimation){
			const int count = engAnimation->GetMoveCount();
			int i;
			for(i=0; i<count; i++){
				pCBMoveName->AddItem(engAnimation->GetMove(i)->GetName());
			}
		}
	}
	
	pCBMoveName->SortItems();
	pCBMoveName->StoreFilterItems();
	
	pCBMoveName->SetText(moveName);
}

void aeWPAPanelRuleStateSnapshot::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleStateSnapshot * const rule = (aeRuleStateSnapshot*)GetRule();
	
	if(rule){
		pEditID->SetInteger(rule->GetID());
		
	}else{
		pEditID->ClearText();
	}
	
	const bool enabled = rule;
	pEditID->SetEnabled(enabled);
	pCBMoveName->SetEnabled(enabled);
	pEditMoveTime->SetEnabled(enabled);
	
	pChkUseLastState->GetAction()->Update();
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
	pBtnSnapshot->GetAction()->Update();
	pBtnGetFrame->GetAction()->Update();
}

const decString &aeWPAPanelRuleStateSnapshot::GetCBMoveNameText() const{
	return pCBMoveName->GetText();
}

float aeWPAPanelRuleStateSnapshot::GetTextMoveTime() const{
	return pEditMoveTime->GetFloat();
}
