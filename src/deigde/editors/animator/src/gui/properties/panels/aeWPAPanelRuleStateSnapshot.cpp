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
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPAPanelRuleStateSnapshot &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = (aeRuleStateSnapshot*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleStateSnapshot *rule) = 0;
	
	void Update() override{
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
	using Ref = deTObjectReference<cBaseTextFieldListener>;
	cBaseTextFieldListener(aeWPAPanelRuleStateSnapshot &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = (aeRuleStateSnapshot*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleStateSnapshot *rule) = 0;
};


class cActionUseLastState : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionUseLastState>;
	
public:
	cActionUseLastState(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleStateSnapshot.UseLastState", nullptr,
		"@Animator.WPAPanelRuleStateSnapshot.UseLastState.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateSnapshot *rule) override{
		return aeUSetRuleSnapUseLast::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetUseLastState());
	}
};

class cTextID : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextID>;
	cTextID(aeWPAPanelRuleStateSnapshot &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleStateSnapshot *rule) override{
		const int value = textField->GetInteger();
		return rule->GetID() != value ? aeURuleSnapSetID::Ref::New(rule, value) : aeURuleSnapSetID::Ref();
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnablePosition>;
	
public:
	cActionEnablePosition(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleStateSnapshot.EnablePosition", nullptr,
		"@Animator.WPAPanelRuleStateSnapshot.EnablePosition.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateSnapshot *rule) override{
		return aeURuleSnapToggleEnablePosition::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableRotation>;
	
public:
	cActionEnableRotation(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleStateSnapshot.EnableRotation", nullptr,
		"@Animator.WPAPanelRuleStateSnapshot.EnableRotation.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateSnapshot *rule) override{
		return aeURuleSnapToggleEnableRotation::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableSize>;
	
public:
	cActionEnableSize(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleStateSnapshot.EnableSize", nullptr,
		"@Animator.WPAPanelRuleStateSnapshot.EnableSize.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateSnapshot *rule) override{
		return aeURuleSnapToggleEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableVertexPositionSet>;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleStateSnapshot.EnableVertexPositionSet", nullptr,
		"@Animator.WPAPanelRuleStateSnapshot.EnableVertexPositionSet.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateSnapshot *rule) override{
		return aeURuleSnapToggleEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateSnapshot &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

class cActionSnapshot : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionSnapshot>;
	
public:
	cActionSnapshot(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleStateSnapshot.ActionSnapshot", nullptr,
		"@Animator.WPAPanelRuleStateSnapshot.ActionSnapshot.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleStateSnapshot *rule) override{
		deAnimatorInstance * const engAnimatorInstance = animator->GetEngineAnimatorInstance();
		if(engAnimatorInstance){
			engAnimatorInstance->CaptureStateInto(rule->GetID());
		}
		return {};
	}
};

class cActionGetFrame : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionGetFrame>;
	
public:
	cActionGetFrame(aeWPAPanelRuleStateSnapshot &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleStateSnapshot.ActionGetFrame", nullptr,
		"@Animator.WPAPanelRuleStateSnapshot.ActionGetFrame.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleStateSnapshot *rule) override{
		deAnimatorInstance * const engAnimatorInstance = animator->GetEngineAnimatorInstance();
		const decString moveName(pPanel.GetCBMoveNameText());
		if(engAnimatorInstance){
			engAnimatorInstance->StoreFrameInto(rule->GetID(), moveName, pPanel.GetTextMoveTime());
		}
		return {};
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
	
	
	helper.GroupBox(*this, groupBox, "@Animator.WPAPanelRuleStateSnapshot.StateSnapshot");
	
	helper.CheckBox(groupBox, pChkUseLastState, cActionUseLastState::Ref::New(*this));
	helper.EditInteger(groupBox, "@Animator.WPAPanelRuleStateSnapshot.ID",
		"@Animator.WPAPanelRuleStateSnapshot.ID.ToolTip",
		pEditID, cTextID::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "@Animator.WPAPanelRuleStateSnapshot.MoveName", true,
		"@Animator.WPAPanelRuleStateSnapshot.MoveName.ToolTip",
		pCBMoveName, {});
	pCBMoveName->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "@Animator.WPAPanelRuleStateSnapshot.MoveTime",
		"@Animator.WPAPanelRuleStateSnapshot.MoveTime.ToolTip",
		pEditMoveTime, {});
	
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
	
	helper.FormLine(groupBox, "@Animator.WPAPanelRuleStateSnapshot.Snapshot",
		"@Animator.WPAPanelRuleStateSnapshot.Snapshot.ToolTip", formLine);
	helper.Button(formLine, pBtnSnapshot, cActionSnapshot::Ref::New(*this));
	helper.Button(formLine, pBtnGetFrame, cActionGetFrame::Ref::New(*this));
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
			? GetAnimator()->GetEngineAnimator()->GetAnimation().Pointer() : nullptr;
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
