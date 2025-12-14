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

#include "aeWPAPanelRuleInverseKinematic.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleInverseKinematic.h"
#include "../../../undosys/rule/ik/aeURuleIKSetGoalPosition.h"
#include "../../../undosys/rule/ik/aeURuleIKSetGoalRotation.h"
#include "../../../undosys/rule/ik/aeURuleIKSetLocalPosition.h"
#include "../../../undosys/rule/ik/aeURuleIKSetLocalRotation.h"
#include "../../../undosys/rule/ik/aeURuleIKSetAdjustOrientation.h"
#include "../../../undosys/rule/ik/aeURuleIKSetSolverBone.h"
#include "../../../undosys/rule/ik/aeURuleIKSetUseSolverBone.h"
#include "../../../undosys/rule/ik/aeURuleIKSetReachCenter.h"
#include "../../../undosys/rule/ik/aeURuleIKSetReachRange.h"
#include "../../../undosys/rule/ik/aeURuleIKSetReachBone.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPAPanelRuleInverseKinematic &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = (aeRuleInverseKinematic*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleInverseKinematic *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = (aeRuleInverseKinematic*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleInverseKinematic &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(aeWPAPanelRuleInverseKinematic &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = (aeRuleInverseKinematic*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleInverseKinematic *rule) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(aeWPAPanelRuleInverseKinematic &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = (aeRuleInverseKinematic*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleInverseKinematic *rule) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(aeWPAPanelRuleInverseKinematic &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = (aeRuleInverseKinematic*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator *animator, aeRuleInverseKinematic *rule) = 0;
};


class cEditGoalPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditGoalPosition> Ref;
	cEditGoalPosition(aeWPAPanelRuleInverseKinematic &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetGoalPosition())
			? aeURuleIKSetGoalPosition::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditGoalRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditGoalRotation> Ref;
	cEditGoalRotation(aeWPAPanelRuleInverseKinematic &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetGoalOrientation())
			? aeURuleIKSetGoalRotation::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditLocalPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditLocalPosition> Ref;
	cEditLocalPosition(aeWPAPanelRuleInverseKinematic &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetLocalPosition())
			? aeURuleIKSetLocalPosition::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditLocalRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditLocalRotation> Ref;
	cEditLocalRotation(aeWPAPanelRuleInverseKinematic &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetLocalOrientation())
			? aeURuleIKSetLocalRotation::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cActionAdjustRotation : public cBaseAction{
public:
	typedef deTObjectReference<cActionAdjustRotation> Ref;
	
public:
	cActionAdjustRotation(aeWPAPanelRuleInverseKinematic &panel) : cBaseAction(panel,
		"Adjust Orientation", nullptr, "Determines if the bone orientation is adjusted"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleInverseKinematic *rule) override{
		return aeURuleIKSetAdjustOrientation::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleInverseKinematic &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetAdjustOrientation());
	}
};

class cComboSolverBone : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboSolverBone> Ref;
	cComboSolverBone(aeWPAPanelRuleInverseKinematic &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleInverseKinematic *rule) override{
		return comboBox->GetText() != rule->GetSolverBone()
			? aeURuleIKSetSolverBone::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cActionUseSolverBone : public cBaseAction{
public:
	typedef deTObjectReference<cActionUseSolverBone> Ref;
	
public:
	cActionUseSolverBone(aeWPAPanelRuleInverseKinematic &panel) : cBaseAction(panel,
		"Use solver bone", nullptr, "Determines if the solver bone is used as reference "
		"coordinate system or the component itself"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleInverseKinematic *rule) override{
		return aeURuleIKSetUseSolverBone::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleInverseKinematic &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetUseSolverBone());
	}
};

class cTextReachRange : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextReachRange> Ref;
	cTextReachRange(aeWPAPanelRuleInverseKinematic &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleInverseKinematic *rule) override{
		const float value = textField->GetFloat();
		return fabsf(rule->GetReachRange() - value) > FLOAT_SAFE_EPSILON
			? aeURuleIKSetReachRange::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cComboReachBone : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboReachBone> Ref;
	cComboReachBone(aeWPAPanelRuleInverseKinematic &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleInverseKinematic *rule) override{
		return comboBox->GetText() != rule->GetReachBone()
			? aeURuleIKSetReachBone::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};

class cEditReachCenter : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditReachCenter> Ref;
	cEditReachCenter(aeWPAPanelRuleInverseKinematic &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetReachCenter())
			? aeURuleIKSetReachCenter::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

}



// Class aeWPAPanelRuleInverseKinematic
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleInverseKinematic::aeWPAPanelRuleInverseKinematic(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertInverseKinematic)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "Inverse Kinematic:");
	
	helper.EditVector(groupBox, "Goal Position:", "Sets the position to reach",
		pEditGoalPos, cEditGoalPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Goal Orientation:", "Sets the orientation to reach",
		pEditGoalRot, cEditGoalRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkAdjustRotation, cActionAdjustRotation::Ref::New(*this));
	
	helper.EditVector(groupBox, "Local Position:",
		"Sets the goal position relative to the bone local coordinate system",
		pEditLocalPos, cEditLocalPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Local Orientation:",
		"Sets the goal orientation relative to the bone local coordinate system",
		pEditLocalRot, cEditLocalRotation::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "Solver Bone:", true,
		"Sets the solver bone to use as reference coordinate system",
		pCBSolverBone, cComboSolverBone::Ref::New(*this));
	pCBSolverBone->SetDefaultSorter();
	helper.CheckBox(groupBox, pChkUseSolverBone, cActionUseSolverBone::Ref::New(*this));
	
	
	// reach
	helper.GroupBox(*this, groupBox, "Reach:", true);
	
	helper.EditFloat(groupBox, "Range:",
		"Maximum allowed range for IK target to be located from the reach center",
		pEditReachRange, cTextReachRange::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "Bone:", true,
		"Bone to use as center for reach calculation or empty string to use constant center",
		pCBReachBone, cComboReachBone::Ref::New(*this));
	pCBReachBone->SetDefaultSorter();
	
	helper.EditVector(groupBox, "Center:", "Center to use for reach calculation if bone is not set",
		pEditReachCenter, cEditReachCenter::Ref::New(*this));
}

aeWPAPanelRuleInverseKinematic::~aeWPAPanelRuleInverseKinematic(){
}



// Management
///////////////

void aeWPAPanelRuleInverseKinematic::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selectionSolver(pCBSolverBone->GetText());
	const decString selectionReach(pCBReachBone->GetText());
	
	pCBSolverBone->RemoveAllItems();
	pCBReachBone->RemoveAllItems();
	
	if(GetAnimator()){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			for(i=0; i<count; i++){
				pCBSolverBone->AddItem(rig->GetBoneAt(i).GetName());
				pCBReachBone->AddItem(rig->GetBoneAt(i).GetName());
			}
		}
		pCBSolverBone->SortItems();
		pCBReachBone->SortItems();
	}
	
	pCBSolverBone->StoreFilterItems();
	pCBSolverBone->SetText(selectionSolver);
	
	pCBReachBone->StoreFilterItems();
	pCBReachBone->SetText(selectionReach);
}

void aeWPAPanelRuleInverseKinematic::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	aeRuleInverseKinematic * const rule = (aeRuleInverseKinematic*)GetRule();
	
	if(rule){
		pEditGoalPos->SetVector(rule->GetGoalPosition());
		pEditGoalRot->SetVector(rule->GetGoalOrientation());
		pEditLocalPos->SetVector(rule->GetLocalPosition());
		pEditLocalRot->SetVector(rule->GetLocalOrientation());
		pCBSolverBone->SetText(rule->GetSolverBone());
		pEditReachRange->SetFloat(rule->GetReachRange());
		pEditReachCenter->SetVector(rule->GetReachCenter());
		pCBReachBone->SetText(rule->GetReachBone());
		
	}else{
		pEditGoalPos->SetVector(decVector());
		pEditGoalRot->SetVector(decVector());
		pEditLocalPos->SetVector(decVector());
		pEditLocalRot->SetVector(decVector());
		pCBSolverBone->ClearText();
		pEditReachRange->ClearText();
		pEditReachCenter->SetVector(decVector());
		pCBReachBone->ClearText();
	}
	
	const bool enabled = rule;
	pEditGoalPos->SetEnabled(enabled);
	pEditGoalRot->SetEnabled(enabled);
	pEditLocalPos->SetEnabled(enabled);
	pEditLocalRot->SetEnabled(enabled);
	pCBSolverBone->SetEnabled(enabled);
	pEditReachRange->SetEnabled(enabled);
	pEditReachCenter->SetEnabled(enabled);
	pCBReachBone->SetEnabled(enabled);
	
	pChkAdjustRotation->GetAction()->Update();
	pChkUseSolverBone->GetAction()->Update();
}

void aeWPAPanelRuleInverseKinematic::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleInverseKinematic * const rule = (aeRuleInverseKinematic*)GetRule();
	if(rule){
		AddTarget("Goal Position", &rule->GetTargetGoalPosition());
		AddTarget("Goal Orientation", &rule->GetTargetGoalOrientation());
		AddTarget("Local Position", &rule->GetTargetLocalPosition());
		AddTarget("Local Orientation", &rule->GetTargetLocalOrientation());
		AddTarget("Reach Range", &rule->GetTargetReachRange());
		AddTarget("Reach Center", &rule->GetTargetReachCenter());
	}
}
