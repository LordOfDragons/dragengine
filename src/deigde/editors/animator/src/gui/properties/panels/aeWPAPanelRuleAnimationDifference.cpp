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
	cBaseAction(aeWPAPanelRuleAnimationDifference &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)pPanel.GetRule();
		if(! animator || ! rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeRuleAnimationDifference *rule) = 0;
	
	virtual void Update(){
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
	cBaseTextFieldListener(aeWPAPanelRuleAnimationDifference &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)pPanel.GetRule();
		if(! animator || ! rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleAnimationDifference *rule) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleAnimationDifference &pPanel;
	
public:
	cBaseComboBoxListener(aeWPAPanelRuleAnimationDifference &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationDifference * const rule = (aeRuleAnimationDifference*)pPanel.GetRule();
		if(! animator || ! rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(comboBox, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleAnimationDifference *rule) = 0;
};


class cComboLeadingMoveName : public cBaseComboBoxListener{
public:
	cComboLeadingMoveName(aeWPAPanelRuleAnimationDifference &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleAnimationDifference *rule){
		return rule->GetLeadingMoveName() != comboBox->GetText()
			? new aeUSetRuleAniDLeadMoveName(rule, comboBox->GetText()) : NULL;
	}
};

class cTextLeadingMoveTime : public cBaseTextFieldListener{
public:
	cTextLeadingMoveTime(aeWPAPanelRuleAnimationDifference &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeRuleAnimationDifference *rule){
		const float value = textField->GetFloat();
		return fabsf(rule->GetLeadingMoveTime() - value) > FLOAT_SAFE_EPSILON
			? new aeUSetRuleAniDLeadMoveTime(rule, value) : NULL;
	}
};

class cComboReferenceMoveName : public cBaseComboBoxListener{
public:
	cComboReferenceMoveName(aeWPAPanelRuleAnimationDifference &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleAnimationDifference *rule){
		return rule->GetReferenceMoveName() != comboBox->GetText()
			? new aeUSetRuleAniDRefMoveName(rule, comboBox->GetText()) : NULL;
	}
};

class cTextReferenceMoveTime : public cBaseTextFieldListener{
public:
	cTextReferenceMoveTime(aeWPAPanelRuleAnimationDifference &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeRuleAnimationDifference *rule){
		const float value = textField->GetFloat();
		return fabsf(rule->GetReferenceMoveTime() - value) > FLOAT_SAFE_EPSILON
			? new aeUSetRuleAniDRefMoveTime(rule, value) : NULL;
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleAnimationDifference *rule){
		return new aeURuleAnimDiffToggleEnablePosition(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleAnimationDifference &rule){
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleAnimationDifference *rule){
		return new aeURuleAnimDiffToggleEnableRotation(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleAnimationDifference &rule){
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleAnimationDifference *rule){
		return new aeURuleAnimDiffToggleEnableSize(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleAnimationDifference &rule){
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleAnimationDifference &panel) : cBaseAction(panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if vertex position set is modified or kept as it is"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleAnimationDifference *rule){
		return new aeURuleAnimDiffToggleEnableVertexPositionSet(rule);
	}
	
	virtual void Update(const aeAnimator & , const aeRuleAnimationDifference &rule){
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
		pCBLMoveName, new cComboLeadingMoveName(*this));
	pCBLMoveName->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "Leading Move Time:", "Time relative to leading move playtime to retrieve",
		pEditLMoveTime, new cTextLeadingMoveTime(*this));
	
	helper.ComboBoxFilter(groupBox, "Reference Move Name:", true, "Name of reference animation move to use",
		pCBRMoveName, new cComboReferenceMoveName(*this));
	pCBRMoveName->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "Reference Move Time:", "Time relative to reference move playtime to retrieve",
		pEditRMoveTime, new cTextReferenceMoveTime(*this));
	helper.CheckBox(groupBox, pChkEnablePosition, new cActionEnablePosition(*this), true);
	helper.CheckBox(groupBox, pChkEnableRotation, new cActionEnableRotation(*this), true);
	helper.CheckBox(groupBox, pChkEnableSize, new cActionEnableSize(*this), true);
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, new cActionEnableVertexPositionSet(*this), true);
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
			? GetAnimator()->GetEngineAnimator()->GetAnimation() : NULL;
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
		AddTarget("Leading Move Time", &rule->GetTargetLeadingMoveTime());
		AddTarget("Reference Move Time", &rule->GetTargetReferenceMoveTime());
	}
}
