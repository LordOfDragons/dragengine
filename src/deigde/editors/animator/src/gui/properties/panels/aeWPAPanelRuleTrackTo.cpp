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

#include "aeWPAPanelRuleTrackTo.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleTrackTo.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetLockedAxis.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetTrackAxis.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetTrackBone.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetUpAxis.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetUpTarget.h"
#include "../../../animatoreditor.h"

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

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleTrackTo &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseComboBoxListener>;
	cBaseComboBoxListener(aeWPAPanelRuleTrackTo &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleTrackTo * const rule = (aeRuleTrackTo*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleTrackTo *rule) = 0;
};


class cComboTrackBone : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboTrackBone>;
	cComboTrackBone(aeWPAPanelRuleTrackTo &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule) override{
		return rule->GetTrackBone() != comboBox->GetText()
			? aeURuleTrackToSetTrackBone::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cComboTrackAxis : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboTrackAxis>;
	cComboTrackAxis(aeWPAPanelRuleTrackTo &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule) override{
		if(!comboBox->GetSelectedItem()){
			return {};
		}
		
		deAnimatorRuleTrackTo::eTrackAxis value = (deAnimatorRuleTrackTo::eTrackAxis)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetTrackAxis() != value ? aeURuleTrackToSetTrackAxis::Ref::New(rule, value) : aeURuleTrackToSetTrackAxis::Ref();
	}
};

class cComboUpAxis : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboUpAxis>;
	cComboUpAxis(aeWPAPanelRuleTrackTo &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule) override{
		if(!comboBox->GetSelectedItem()){
			return {};
		}
		
		deAnimatorRuleTrackTo::eTrackAxis value = (deAnimatorRuleTrackTo::eTrackAxis)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetUpAxis() != value ? aeURuleTrackToSetUpAxis::Ref::New(rule, value) : aeURuleTrackToSetUpAxis::Ref();
	}
};

class cComboUpTarget : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboUpTarget>;
	cComboUpTarget(aeWPAPanelRuleTrackTo &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule) override{
		if(!comboBox->GetSelectedItem()){
			return {};
		}
		
		deAnimatorRuleTrackTo::eUpTarget value = (deAnimatorRuleTrackTo::eUpTarget)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetUpTarget() != value ? aeURuleTrackToSetUpTarget::Ref::New(rule, value) : aeURuleTrackToSetUpTarget::Ref();
	}
};

class cComboLockedAxis : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboLockedAxis>;
	cComboLockedAxis(aeWPAPanelRuleTrackTo &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule) override{
		if(!comboBox->GetSelectedItem()){
			return {};
		}
		
		deAnimatorRuleTrackTo::eLockedAxis value = (deAnimatorRuleTrackTo::eLockedAxis)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetLockedAxis() != value ? aeURuleTrackToSetLockedAxis::Ref::New(rule, value) : aeURuleTrackToSetLockedAxis::Ref();
	}
};

}



// Class aeWPAPanelRuleTrackTo
////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleTrackTo::aeWPAPanelRuleTrackTo(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertTrackTo)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "@Animator.WPAPanelRuleTrackTo.TrackTo");
	
	helper.ComboBoxFilter(groupBox, "@Animator.WPAPanelRuleTrackTo.TrackBone", true,
		"@Animator.WPAPanelRuleTrackTo.TrackBone.ToolTip",
		pCBTrackBone, cComboTrackBone::Ref::New(*this));
	pCBTrackBone->SetDefaultSorter();
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleTrackTo.TrackAxis",
		"@Animator.WPAPanelRuleTrackTo.TrackAxis.ToolTip",
		pCBTrackAxis, cComboTrackAxis::Ref::New(*this));
	pCBTrackAxis->SetAutoTranslateItems(true);
	pCBTrackAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosX", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaPosX);
	pCBTrackAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosY", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaPosY);
	pCBTrackAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosZ", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaPosZ);
	pCBTrackAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegX", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaNegX);
	pCBTrackAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegY", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaNegY);
	pCBTrackAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegZ", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaNegZ);
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleTrackTo.UpAxis",
		"@Animator.WPAPanelRuleTrackTo.UpAxis.ToolTip",
		pCBUpAxis, cComboUpAxis::Ref::New(*this));
	pCBUpAxis->SetAutoTranslateItems(true);
	pCBUpAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosX", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaPosX);
	pCBUpAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosY", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaPosY);
	pCBUpAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosZ", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaPosZ);
	pCBUpAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegX", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaNegX);
	pCBUpAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegY", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaNegY);
	pCBUpAxis->AddItem("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegZ", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::etaNegZ);
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleTrackTo.UpTarget",
		"@Animator.WPAPanelRuleTrackTo.UpTarget.ToolTip",
		pCBUpTarget, cComboUpTarget::Ref::New(*this));
	pCBUpTarget->SetAutoTranslateItems(true);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.WorldX", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutWorldX);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.WorldY", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutWorldY);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.WorldZ", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutWorldZ);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.ComponentX", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutComponentX);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.ComponentY", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutComponentY);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.ComponentZ", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutComponentZ);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.TrackBoneX", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutTrackBoneX);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.TrackBoneY", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutTrackBoneY);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.TrackBoneZ", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutTrackBoneZ);
	pCBUpTarget->AddItem("@Animator.WPAPanelRuleTrackTo.UpTarget.Controller", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::eutController);
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleTrackTo.LockedAxis",
		"@Animator.WPAPanelRuleTrackTo.LockedAxis.ToolTip",
		pCBLockedAxis, cComboLockedAxis::Ref::New(*this));
	pCBLockedAxis->SetAutoTranslateItems(true);
	pCBLockedAxis->AddItem("@Animator.WPAPanelRuleTrackTo.LockedAxis.None", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::elaNone);
	pCBLockedAxis->AddItem("@Animator.WPAPanelRuleTrackTo.LockedAxis.X", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::elaX);
	pCBLockedAxis->AddItem("@Animator.WPAPanelRuleTrackTo.LockedAxis.Y", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::elaY);
	pCBLockedAxis->AddItem("@Animator.WPAPanelRuleTrackTo.LockedAxis.Z", nullptr, (void*)(intptr_t)deAnimatorRuleTrackTo::elaZ);
}

aeWPAPanelRuleTrackTo::~aeWPAPanelRuleTrackTo(){
}



// Management
///////////////

void aeWPAPanelRuleTrackTo::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selection(pCBTrackBone->GetText());
	
	pCBTrackBone->RemoveAllItems();
	
	if(GetAnimator()){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			for(i=0; i<count; i++){
				pCBTrackBone->AddItem(rig->GetBoneAt(i)->GetName());
			}
		}
		pCBTrackBone->SortItems();
	}
	
	pCBTrackBone->StoreFilterItems();
	pCBTrackBone->SetText(selection);
}

void aeWPAPanelRuleTrackTo::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleTrackTo * const rule = (aeRuleTrackTo*)GetRule();
	
	if(rule){
		pCBTrackBone->SetText(rule->GetTrackBone());
		pCBTrackAxis->SetSelectionWithData((void*)(intptr_t)rule->GetTrackAxis());
		pCBUpAxis->SetSelectionWithData((void*)(intptr_t)rule->GetUpAxis());
		pCBUpTarget->SetSelectionWithData((void*)(intptr_t)rule->GetUpTarget());
		pCBLockedAxis->SetSelectionWithData((void*)(intptr_t)rule->GetLockedAxis());
		
	}else{
		pCBTrackBone->ClearText();
		pCBTrackAxis->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleTrackTo::etaPosZ);
		pCBUpAxis->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleTrackTo::etaPosY);
		pCBUpTarget->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleTrackTo::eutComponentY);
		pCBLockedAxis->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleTrackTo::elaNone);
	}
	
	const bool enabled = rule;
	pCBTrackBone->SetEnabled(enabled);
	pCBTrackAxis->SetEnabled(enabled);
	pCBUpAxis->SetEnabled(enabled);
	pCBUpTarget->SetEnabled(enabled);
	pCBLockedAxis->SetEnabled(enabled);
}

void aeWPAPanelRuleTrackTo::UpdateTargetList(){
	aeRuleTrackTo *rule = (aeRuleTrackTo*)GetRule();
	
	aeWPAPanelRule::UpdateTargetList();
	
	if(rule){
		AddTarget("@Animator.Target.Position", rule->GetTargetPosition());
		AddTarget("@Animator.Target.Up", rule->GetTargetUp());
	}
}
