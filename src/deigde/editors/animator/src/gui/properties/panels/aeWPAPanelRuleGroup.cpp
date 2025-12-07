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
	cBaseAction(aeWPAPanelRuleGroup &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleGroup * const rule = (aeRuleGroup*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeRuleGroup *rule) = 0;
	
	virtual void Update(){
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
	cComboApplicationType(aeWPAPanelRuleGroup &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
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
			aeURuleGroupSetApplicationType::Ref::NewWith(rule, value));
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleGroup *rule){
		return new aeURuleGroupToggleEnablePosition(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleGroup &rule){
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleGroup *rule){
		return new aeURuleGroupToggleEnableRotation(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleGroup &rule){
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleGroup *rule){
		return new aeURuleGroupToggleEnableSize(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleGroup &rule){
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if the vertex position set is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleGroup *rule){
		return new aeURuleGroupToggleEnableVertexPositionSet(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleGroup &rule){
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

class cActionUseCurrentState : public cBaseAction{
public:
	cActionUseCurrentState(aeWPAPanelRuleGroup &panel) : cBaseAction(panel,
		"Use current state", NULL, "Use current animation state instead of empty state"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleGroup *rule){
		return new aeURuleGroupToggleUseCurrentState(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleGroup &rule){
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
	
	
	helper.GroupBox(*this, groupBox, "Group:");
	
	helper.ComboBox(groupBox, "Application Type:", "Sets how child rules are process in the the group.\n"
		"Apply all rules (all) or blend between two selected rules using the select target (select).",
		pCBApplicationType, new cComboApplicationType(*this));
	pCBApplicationType->AddItem("All", NULL, (void*)(intptr_t)deAnimatorRuleGroup::eatAll);
	pCBApplicationType->AddItem("Select", NULL, (void*)(intptr_t)deAnimatorRuleGroup::eatSelect);
	
	helper.CheckBox(groupBox, pChkUseCurrentState, new cActionUseCurrentState(*this), true);
	
	helper.CheckBox(groupBox, pChkEnablePosition, new cActionEnablePosition(*this), true);
	helper.CheckBox(groupBox, pChkEnableRotation, new cActionEnableRotation(*this), true);
	helper.CheckBox(groupBox, pChkEnableSize, new cActionEnableSize(*this), true);
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, new cActionEnableVertexPositionSet(*this), true);
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
		AddTarget("Select", &rule->GetTargetSelect());
	}
}
