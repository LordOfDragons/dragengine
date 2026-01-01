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
	typedef deTObjectReference<cBaseAction> Ref;
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
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(aeWPAPanelRuleForeignState &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
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
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(aeWPAPanelRuleForeignState &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
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
	typedef deTObjectReference<cComboBone> Ref;
	cComboBone(aeWPAPanelRuleForeignState &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule) override{
		return rule->GetForeignBone() != comboBox->GetText()
			? aeUSetRuleFStaBone::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cComboVertexPositionSet : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboVertexPositionSet> Ref;
	cComboVertexPositionSet(aeWPAPanelRuleForeignState &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule) override{
		return rule->GetForeignVertexPositionSet() != comboBox->GetText()
			? aeUSetRuleFStaVertexPositionSet::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cComboCoordFrameSource : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCoordFrameSource> Ref;
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
	typedef deTObjectReference<cComboCoordFrameDestination> Ref;
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
	typedef deTObjectReference<cTextScalePosition> Ref;
	cTextScalePosition(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScalePosition() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaPosition::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cTextScaleRotation : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextScaleRotation> Ref;
	cTextScaleRotation(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScaleOrientation() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaRotation::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cTextScaleSize : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextScaleSize> Ref;
	cTextScaleSize(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScaleSize() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaSize::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cTextScaleVertexPositionSet : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextScaleVertexPositionSet> Ref;
	cTextScaleVertexPositionSet(aeWPAPanelRuleForeignState &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetScaleVertexPositionSet() - value) > FLOAT_SAFE_EPSILON
			? aeUSetRuleFStaScaleVertexPositionSet::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePosition> Ref;
	
public:
	cActionEnablePosition(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"Enable position manipulation", nullptr, "Determines if the position is modified or kept as it is"){ }
	
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
	typedef deTObjectReference<cActionEnableRotation> Ref;
	
public:
	cActionEnableRotation(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"Enable rotation manipulation", nullptr, "Determines if the rotation is modified or kept as it is"){ }
	
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
	typedef deTObjectReference<cActionEnableSize> Ref;
	
public:
	cActionEnableSize(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"Enable size manipulation", nullptr, "Determines if the size is modified or kept as it is"){ }
	
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
	typedef deTObjectReference<cActionEnableVertexPositionSet> Ref;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleForeignState &panel) : cBaseAction(panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if the vertex position set is modified or kept as it is"){ }
	
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
	
	
	helper.GroupBox(*this, groupBox, "Foreign State:");
	
	helper.ComboBoxFilter(groupBox, "Bone:", true, "Sets the bone to retrieve the state from",
		pCBBone, cComboBone::Ref::New(*this));
	pCBBone->SetDefaultSorter();
	
	helper.ComboBoxFilter(groupBox, "Vertex Position Set:", true,
		"Sets the vertex position set to retrieve the state from",
		pCBVertexPositionSet, cComboVertexPositionSet::Ref::New(*this));
	pCBVertexPositionSet->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "Scale Position:", "Scaling applied to the foreign position",
		pEditPosition, cTextScalePosition::Ref::New(*this));
	helper.EditFloat(groupBox, "Scale Rotation:", "Scaling applied to the foreign orientation",
		pEditRotation, cTextScaleRotation::Ref::New(*this));
	helper.EditFloat(groupBox, "Scale Size:", "Scaling applied to the foreign size",
		pEditSize, cTextScaleSize::Ref::New(*this));
	helper.EditFloat(groupBox, "Scale Vertex Position Set:",
		"Scaling applied to the foreign vertex position set",
		pEditVertexPositionSet, cTextScaleVertexPositionSet::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Src. Coord-Frame:", "Sets the source coordinate frame to use",
		pCBSrcCFrame, cComboCoordFrameSource::Ref::New(*this));
	pCBSrcCFrame->AddItem("Bone Local", nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfBoneLocal);
	pCBSrcCFrame->AddItem("Component", nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfComponent);
	
	helper.ComboBox(groupBox, "Dest. Coord-Frame:", "Sets the destination coordinate frame to use",
		pCBDestCFrame, cComboCoordFrameDestination::Ref::New(*this));
	pCBDestCFrame->AddItem("Bone Local", nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfBoneLocal);
	pCBDestCFrame->AddItem("Component", nullptr, (void*)(intptr_t)deAnimatorRuleForeignState::ecfComponent);
	
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
				pCBBone->AddItem(rig->GetBoneAt(i).GetName());
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
		const deModel * const model = component ? component->GetModel() : nullptr;
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
		AddTarget("Position", rule->GetTargetPosition());
		AddTarget("Orientation", rule->GetTargetOrientation());
		AddTarget("Size", rule->GetTargetSize());
		AddTarget("Vertex Position Set", rule->GetTargetVertexPositionSet());
	}
}
