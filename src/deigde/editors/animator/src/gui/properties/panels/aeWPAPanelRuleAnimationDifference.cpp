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

#include "aeWPAPanelRuleAnimationDifference.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleAnimationDifference.h"
#include "../../../undosys/rule/animdiff/aeUSetRuleAniDLeadMoveName.h"
#include "../../../undosys/rule/animdiff/aeUSetRuleAniDLeadMoveTime.h"
#include "../../../undosys/rule/animdiff/aeUSetRuleAniDRefMoveName.h"
#include "../../../undosys/rule/animdiff/aeUSetRuleAniDRefMoveTime.h"
#include "../../../undosys/rule/animdiff/aeURuleAnimDiffToggleEnablePosition.h"
#include "../../../undosys/rule/animdiff/aeURuleAnimDiffToggleEnableRotation.h"
#include "../../../undosys/rule/animdiff/aeURuleAnimDiffToggleEnableSize.h"
#include "../../../undosys/rule/animdiff/aeURuleAnimDiffToggleEnableVertexPositionSet.h"
#include "../../../animatoreditor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
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
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleAnimationDifference &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPAPanelRuleAnimationDifference &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleAnimationDifference *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleAnimationDifference &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleAnimationDifference &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(aeWPAPanelRuleAnimationDifference &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleAnimationDifference *rule) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleAnimationDifference &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(aeWPAPanelRuleAnimationDifference &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleAnimationDifference *rule) = 0;
};


class cComboLeadingMoveName : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboLeadingMoveName> Ref;
	cComboLeadingMoveName(aeWPAPanelRuleAnimationDifference &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleAnimationDifference *rule) override{
		return rule->GetLeadingMoveName() != comboBox->GetText()
			? aeUSetRuleAniDLeadMoveName::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cTextLeadingMoveTime : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLeadingMoveTime> Ref;
	cTextLeadingMoveTime(aeWPAPanelRuleAnimationDifference &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleAnimationDifference *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetLeadingMoveTime() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleAniDLeadMoveTime::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cComboReferenceMoveName : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboReferenceMoveName> Ref;
	cComboReferenceMoveName(aeWPAPanelRuleAnimationDifference &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleAnimationDifference *rule) override{
		return rule->GetReferenceMoveName() != comboBox->GetText()
			? aeUSetRuleAniDRefMoveName::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cTextReferenceMoveTime : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextReferenceMoveTime> Ref;
	cTextReferenceMoveTime(aeWPAPanelRuleAnimationDifference &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleAnimationDifference *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetReferenceMoveTime() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleAniDRefMoveTime::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePosition> Ref;
	
public:
	cActionEnablePosition(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable position manipulation", nullptr, "Determines if the position is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationDifference *rule) override{
		return aeURuleAnimDiffToggleEnablePosition::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationDifference &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotation> Ref;
	
public:
	cActionEnableRotation(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable rotation manipulation", nullptr, "Determines if the rotation is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationDifference *rule) override{
		return aeURuleAnimDiffToggleEnableRotation::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationDifference &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableSize> Ref;
	
public:
	cActionEnableSize(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable size manipulation", nullptr, "Determines if the size is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationDifference *rule) override{
		return aeURuleAnimDiffToggleEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationDifference &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableVertexPositionSet> Ref;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if vertex position set is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationDifference *rule) override{
		return aeURuleAnimDiffToggleEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationDifference &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

}



// Class aeWPAPanelRuleAnimationDifference
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleAnimationDifference::aeWPAPanelRuleAnimationDifference(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertAnimationDifference)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "Animation:");
	
	helper.ComboBoxFilter(groupBox, "Leading Move Name:", true, "Name of leading animation move to use",
		pCBLMoveName, cComboLeadingMoveName::Ref::New(*this));
	pCBLMoveName->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "Leading Move Time:", "Time relative to leading move playtime to retrieve",
		pEditLMoveTime, cTextLeadingMoveTime::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "Reference Move Name:", true, "Name of reference animation move to use",
		pCBRMoveName, cComboReferenceMoveName::Ref::New(*this));
	pCBRMoveName->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "Reference Move Time:", "Time relative to reference move playtime to retrieve",
		pEditRMoveTime, cTextReferenceMoveTime::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
}

aeWPAPanelRuleAnimationDifference::~aeWPAPanelRuleAnimationDifference(){
}



// Management
///////////////

void aeWPAPanelRuleAnimationDifference::UpdateAnimMoveList(){
	aeWPAPanelRule::UpdateAnimMoveList();
	
	const decString leadingMoveName(pCBLMoveName->GetText());
	const decString referenceMoveName(pCBRMoveName->GetText());
	
	pCBLMoveName->RemoveAllItems();
	pCBRMoveName->RemoveAllItems();
	
	if(GetAnimator()){
		const deAnimation * const engAnimation = GetAnimator()->GetEngineAnimator()
			? GetAnimator()->GetEngineAnimator()->GetAnimation() : nullptr;
		if(engAnimation){
			const int count = engAnimation->GetMoveCount();
			int i;
			for(i=0; i<count; i++){
				pCBLMoveName->AddItem(engAnimation->GetMove(i)->GetName());
				pCBRMoveName->AddItem(engAnimation->GetMove(i)->GetName());
			}
		}
	}
	
	pCBLMoveName->SortItems();
	pCBLMoveName->StoreFilterItems();
	pCBLMoveName->SetText(leadingMoveName);
	
	pCBRMoveName->SortItems();
	pCBRMoveName->StoreFilterItems();
	pCBRMoveName->SetText(referenceMoveName);
}

void aeWPAPanelRuleAnimationDifference::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)GetRule();
	
	if(rule){
		pCBLMoveName->SetText(rule->GetLeadingMoveName());
		pEditLMoveTime->SetFloat(rule->GetLeadingMoveTime());
		pCBRMoveName->SetText(rule->GetReferenceMoveName());
		pEditRMoveTime->SetFloat(rule->GetReferenceMoveTime());
		
	}else{
		pCBLMoveName->ClearText();
		pEditLMoveTime->ClearText();
		pCBRMoveName->ClearText();
		pEditRMoveTime->ClearText();
	}
	
	const bool enabled = rule;
	pCBLMoveName->SetEnabled(enabled);
	pEditLMoveTime->SetEnabled(enabled);
	pCBRMoveName->SetEnabled(enabled);
	pEditRMoveTime->SetEnabled(enabled);
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}

void aeWPAPanelRuleAnimationDifference::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)GetRule();
	if(rule){
		AddTarget("Leading Move Time", rule->GetTargetLeadingMoveTime());
		AddTarget("Reference Move Time", rule->GetTargetReferenceMoveTime());
	}
}
