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

#include "aeWPAPanelRuleForeignState.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleForeignState.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaBone.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaVertexPositionSet.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaPosition.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaRotation.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaSize.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaScaleVertexPositionSet.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaEnablePos.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaEnableRot.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaEnableSize.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaEnableVertexPositionSet.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaSrcCFrame.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaDestCFrame.h"

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
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleForeignState &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPAPanelRuleForeignState &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = (aeRuleForeignState*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleForeignState *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = (aeRuleForeignState*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleForeignState &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleForeignState &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseComboBoxListener>;
	cBaseComboBoxListener(aeWPAPanelRuleForeignState &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = (aeRuleForeignState*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleForeignState *rule) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleForeignState &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseTextFieldListener>;
	cBaseTextFieldListener(aeWPAPanelRuleForeignState &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = (aeRuleForeignState*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleForeignState *rule) = 0;
};


class cComboBone : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboBone>;
	cComboBone(aeWPAPanelRuleForeignState &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule) override{
		return rule->GetForeignBone() != comboBox->GetText()
			? aeUSetRuleFStaBone::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cComboVertexPositionSet : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboVertexPositionSet>;
	cComboVertexPositionSet(aeWPAPanelRuleForeignState &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule) override{
		return rule->GetForeignVertexPositionSet() != comboBox->GetText()
			? aeUSetRuleFStaVertexPositionSet::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cComboCoordFrameSource : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboCoordFrameSource>;
	cComboCoordFrameSource(aeWPAPanelRuleForeignState &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule) override{
		if(!comboBox->GetSelectedItem()){
			return igdeUndo::Ref();
		}
		
		deAnimatorRuleForeignState::eCoordinateFrames value =
			(deAnimatorRuleForeignState::eCoordinateFrames)(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetSourceCoordinateFrame() != value ? aeUSetRuleFStaSrcCFrame::Ref::New(rule, value) : aeUSetRuleFStaSrcCFrame::Ref();
	}
};

class cComboCoordFrameDestination : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboCoordFrameDestination>;
	cComboCoordFrameDestination(aeWPAPanelRuleForeignState &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule) override{
		if(!comboBox->GetSelectedItem()){
			return igdeUndo::Ref();
		}
		
		deAnimatorRuleForeignState::eCoordinateFrames value =
			(deAnimatorRuleForeignState::eCoordinateFrames)(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetDestCoordinateFrame() != value ? aeUSetRuleFStaDestCFrame::Ref::New(rule, value) : aeUSetRuleFStaDestCFrame::Ref();
	}
};

class cTextScalePosition : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextScalePosition>;
	cTextScalePosition(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScalePosition() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaPosition::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cTextScaleRotation : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextScaleRotation>;
	cTextScaleRotation(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScaleOrientation() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaRotation::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cTextScaleSize : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextScaleSize>;
	cTextScaleSize(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScaleSize() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaSize::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cTextScaleVertexPositionSet : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextScaleVertexPositionSet>;
	cTextScaleVertexPositionSet(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScaleVertexPositionSet() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaScaleVertexPositionSet::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnablePosition>;
	
public:
	cActionEnablePosition(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleForeignState.EnablePosition", nullptr,
		"@Animator.WPAPanelRuleForeignState.EnablePosition.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleForeignState *rule) override{
		return aeUSetRuleFStaEnablePos::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleForeignState &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableRotation>;
	
public:
	cActionEnableRotation(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleForeignState.EnableRotation", nullptr,
		"@Animator.WPAPanelRuleForeignState.EnableRotation.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleForeignState *rule) override{
		return aeUSetRuleFStaEnableRot::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleForeignState &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableSize>;
	
public:
	cActionEnableSize(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleForeignState.EnableSize", nullptr,
		"@Animator.WPAPanelRuleForeignState.EnableSize.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleForeignState *rule) override{
		return aeUSetRuleFStaEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleForeignState &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableVertexPositionSet>;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleForeignState.EnableVertexPositionSet", nullptr,
		"@Animator.WPAPanelRuleForeignState.EnableVertexPositionSet.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleForeignState *rule) override{
		return aeUSetRuleFStaEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleForeignState &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

}



// Class aeWPAPanelRuleForeignState
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleForeignState::aeWPAPanelRuleForeignState(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertForeignState)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "@Animator.WPAPanelRuleForeignState.ForeignState");
	
	helper.ComboBoxFilter(groupBox, "@Animator.WPAPanelRuleForeignState.Bone", true,
		"@Animator.WPAPanelRuleForeignState.Bone.ToolTip",
		pCBBone, cComboBone::Ref::New(*this));
	pCBBone->SetDefaultSorter();
	
	helper.ComboBoxFilter(groupBox, "@Animator.WPAPanelRuleForeignState.VertexPositionSet", true,
		"@Animator.WPAPanelRuleForeignState.VertexPositionSet.ToolTip",
		pCBVertexPositionSet, cComboVertexPositionSet::Ref::New(*this));
	pCBVertexPositionSet->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "@Animator.WPAPanelRuleForeignState.ScalePosition",
		"@Animator.WPAPanelRuleForeignState.ScalePosition.ToolTip",
		pEditPosition, cTextScalePosition::Ref::New(*this));
	helper.EditFloat(groupBox, "@Animator.WPAPanelRuleForeignState.ScaleRotation",
		"@Animator.WPAPanelRuleForeignState.ScaleRotation.ToolTip",
		pEditRotation, cTextScaleRotation::Ref::New(*this));
	helper.EditFloat(groupBox, "@Animator.WPAPanelRuleForeignState.ScaleSize",
		"@Animator.WPAPanelRuleForeignState.ScaleSize.ToolTip",
		pEditSize, cTextScaleSize::Ref::New(*this));
	helper.EditFloat(groupBox, "@Animator.WPAPanelRuleForeignState.ScaleVertexPositionSet",
		"@Animator.WPAPanelRuleForeignState.ScaleVertexPositionSet.ToolTip",
		pEditVertexPositionSet, cTextScaleVertexPositionSet::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleForeignState.SrcCoordFrame",
		"@Animator.WPAPanelRuleForeignState.SrcCoordFrame.ToolTip",
		pCBSrcCFrame, cComboCoordFrameSource::Ref::New(*this));
	pCBSrcCFrame->SetAutoTranslateItems(true);
	pCBSrcCFrame->AddItem("@Animator.WPAPanelRuleForeignState.CoordFrame.BoneLocal",
		nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfBoneLocal);
	pCBSrcCFrame->AddItem("@Animator.WPAPanelRuleForeignState.CoordFrame.Component",
		nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfComponent);
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleForeignState.DestCoordFrame",
		"@Animator.WPAPanelRuleForeignState.DestCoordFrame.ToolTip",
		pCBDestCFrame, cComboCoordFrameDestination::Ref::New(*this));
	pCBDestCFrame->SetAutoTranslateItems(true);
	pCBDestCFrame->AddItem("@Animator.WPAPanelRuleForeignState.CoordFrame.BoneLocal",
		nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfBoneLocal);
	pCBDestCFrame->AddItem("@Animator.WPAPanelRuleForeignState.CoordFrame.Component",
		nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfComponent);
	
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
}

aeWPAPanelRuleForeignState::~aeWPAPanelRuleForeignState(){
}



// Management
///////////////

void aeWPAPanelRuleForeignState::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selection(pCBBone->GetText());
	
	pCBBone->RemoveAllItems();
	
	if(GetAnimator()){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			for(i=0; i<count; i++){
				pCBBone->AddItem(rig->GetBoneAt(i)->GetName());
			}
		}
		pCBBone->SortItems();
	}
	
	pCBBone->StoreFilterItems();
	pCBBone->SetText(selection);
}

void aeWPAPanelRuleForeignState::UpdateModelVertexPositionSetList(){
	aeWPAPanelRule::UpdateModelVertexPositionSetList();
	
	const decString selection(pCBVertexPositionSet->GetText());
	
	pCBVertexPositionSet->RemoveAllItems();
	
	if(GetAnimator()){
		const deComponent * const component = GetAnimator()->GetEngineComponent();
		const deModel * const model = component ? component->GetModel().Pointer() : nullptr;
		if(model){
			const int count = model->GetVertexPositionSetCount();
			int i;
			for(i=0; i<count; i++){
				pCBVertexPositionSet->AddItem(model->GetVertexPositionSetAt(i)->GetName());
			}
		}
		pCBVertexPositionSet->SortItems();
	}
	
	pCBVertexPositionSet->StoreFilterItems();
	pCBVertexPositionSet->SetText(selection);
}

void aeWPAPanelRuleForeignState::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleForeignState * const rule = (aeRuleForeignState*)GetRule();
	
	if(rule){
		pCBBone->SetText(rule->GetForeignBone());
		pCBVertexPositionSet->SetText(rule->GetForeignVertexPositionSet());
		pEditPosition->SetFloat(rule->GetScalePosition());
		pEditRotation->SetFloat(rule->GetScaleOrientation());
		pEditSize->SetFloat(rule->GetScaleSize());
		pEditVertexPositionSet->SetFloat(rule->GetScaleVertexPositionSet());
		pCBSrcCFrame->SetSelectionWithData((void*)(intptr_t)rule->GetSourceCoordinateFrame());
		pCBDestCFrame->SetSelectionWithData((void*)(intptr_t)rule->GetDestCoordinateFrame());
		
	}else{
		pCBBone->ClearText();
		pCBVertexPositionSet->ClearText();
		pEditPosition->ClearText();
		pEditRotation->ClearText();
		pEditSize->ClearText();
		pEditVertexPositionSet->ClearText();
		pCBSrcCFrame->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleForeignState::ecfComponent);
		pCBDestCFrame->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleForeignState::ecfComponent);
	}
	
	const bool enabled = rule;
	pCBBone->SetEnabled(enabled);
	pCBVertexPositionSet->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditSize->SetEnabled(enabled);
	pEditVertexPositionSet->SetEnabled(enabled);
	pCBSrcCFrame->SetEnabled(enabled);
	pCBDestCFrame->SetEnabled(enabled);
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}

void aeWPAPanelRuleForeignState::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleForeignState * const rule = (aeRuleForeignState*)GetRule();
	if(rule){
		AddTarget("@Animator.Target.Position", rule->GetTargetPosition());
		AddTarget("@Animator.Target.Orientation", rule->GetTargetOrientation());
		AddTarget("@Animator.Target.Size", rule->GetTargetSize());
		AddTarget("@Animator.Target.VertexPositionSet", rule->GetTargetVertexPositionSet());
	}
}
