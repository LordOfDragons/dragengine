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
#include <stdint.h>

#include "aeWPAPanelRuleGroup.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/rule/aeRuleGroup.h"
#include "../../../undosys/rule/group/aeURuleGroupToggleEnablePosition.h"
#include "../../../undosys/rule/group/aeURuleGroupToggleEnableRotation.h"
#include "../../../undosys/rule/group/aeURuleGroupToggleEnableSize.h"
#include "../../../undosys/rule/group/aeURuleGroupToggleEnableVertexPositionSet.h"
#include "../../../undosys/rule/group/aeURuleGroupToggleUseCurrentState.h"
#include "../../../undosys/rule/group/aeURuleGroupSetApplicationType.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleGroup &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPAPanelRuleGroup &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleGroup * const rule = (aeRuleGroup*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleGroup *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleGroup * const rule = (aeRuleGroup*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleGroup &){
		SetEnabled(true);
		SetSelected(false);
	}
};


class cComboApplicationType : public igdeComboBoxListener{
	aeWPAPanelRuleGroup &pPanel;
	
public:
	using Ref = deTObjectReference<cComboApplicationType>;
	cComboApplicationType(aeWPAPanelRuleGroup &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleGroup * const rule = (aeRuleGroup*)pPanel.GetRule();
		if(!animator || !rule || !comboBox->GetSelectedItem()){
			return;
		}
		
		const deAnimatorRuleGroup::eApplicationTypes value = (deAnimatorRuleGroup::eApplicationTypes)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(value == rule->GetApplicationType()){
			return;
		}
		
		animator->GetUndoSystem()->Add(
			aeURuleGroupSetApplicationType::Ref::New(rule, value));
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnablePosition>;
	
public:
	cActionEnablePosition(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleGroup.EnablePosition.Label", nullptr,
		"@Animator.WPAPanelRuleGroup.EnablePosition.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleGroup *rule) override{
		return aeURuleGroupToggleEnablePosition::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleGroup &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableRotation>;
	
public:
	cActionEnableRotation(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleGroup.EnableRotation.Label", nullptr,
		"@Animator.WPAPanelRuleGroup.EnableRotation.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleGroup *rule) override{
		return aeURuleGroupToggleEnableRotation::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleGroup &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableSize>;
	
public:
	cActionEnableSize(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleGroup.EnableSize.Label", nullptr,
		"@Animator.WPAPanelRuleGroup.EnableSize.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleGroup *rule) override{
		return aeURuleGroupToggleEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleGroup &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableVertexPositionSet>;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleGroup.EnableVertexPositionSet.Label", nullptr,
		"@Animator.WPAPanelRuleGroup.EnableVertexPositionSet.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleGroup *rule) override{
		return aeURuleGroupToggleEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleGroup &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

class cActionUseCurrentState : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionUseCurrentState>;
	
public:
	cActionUseCurrentState(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleGroup.UseCurrentState.Label", nullptr,
		"@Animator.WPAPanelRuleGroup.UseCurrentState.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleGroup *rule) override{
		return aeURuleGroupToggleUseCurrentState::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleGroup &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetUseCurrentState());
	}
};

}



// Class aeWPAPanelRuleGroup
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleGroup::aeWPAPanelRuleGroup(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertGroup)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "@Animator.WPAPanelRuleGroup.Group.Label");
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleGroup.ApplicationType.Label",
		"@Animator.WPAPanelRuleGroup.ApplicationType.ToolTip",
		pCBApplicationType, cComboApplicationType::Ref::New(*this));
	pCBApplicationType->SetAutoTranslateItems(true);
	pCBApplicationType->AddItem("@Animator.WPAPanelRuleGroup.ApplicationType.All",
		nullptr, (void*)(intptr_t)deAnimatorRuleGroup::eatAll);
	pCBApplicationType->AddItem("@Animator.WPAPanelRuleGroup.ApplicationType.Select",
		nullptr, (void*)(intptr_t)deAnimatorRuleGroup::eatSelect);
	
	helper.CheckBox(groupBox, pChkUseCurrentState, cActionUseCurrentState::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
}

aeWPAPanelRuleGroup::~aeWPAPanelRuleGroup(){
}



// Management
///////////////

void aeWPAPanelRuleGroup::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleGroup * const rule = (aeRuleGroup*)GetRule();
	
	if(rule){
		pCBApplicationType->SetSelectionWithData((void*)(intptr_t)rule->GetApplicationType());
		
	}else{
		pCBApplicationType->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleGroup::eatAll);
	}
	
	const bool enabled = rule;
	pCBApplicationType->SetEnabled(enabled);
	
	pChkUseCurrentState->GetAction()->Update();
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}

void aeWPAPanelRuleGroup::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleGroup * const rule = (aeRuleGroup*)GetRule();
	if(rule){
		AddTarget("Select", rule->GetTargetSelect());
	}
}
