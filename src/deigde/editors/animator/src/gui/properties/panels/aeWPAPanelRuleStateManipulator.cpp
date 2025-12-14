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

#include "aeWPAPanelRuleStateManipulator.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleStateManipulator.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMinPosition.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMaxPosition.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMinRotation.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMaxRotation.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMinSize.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMaxSize.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMinVertexPositionSet.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMaxVertexPositionSet.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModEnablePos.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModEnableRot.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModEnableSize.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModEnableVertexPositionSet.h"

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

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleStateManipulator &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPAPanelRuleStateManipulator &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateManipulator * const rule = (aeRuleStateManipulator*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleStateManipulator *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateManipulator * const rule = (aeRuleStateManipulator*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleStateManipulator &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPAPanelRuleStateManipulator &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(aeWPAPanelRuleStateManipulator &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateManipulator * const rule = (aeRuleStateManipulator*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator *animator, aeRuleStateManipulator *rule) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleStateManipulator &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(aeWPAPanelRuleStateManipulator &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateManipulator * const rule = (aeRuleStateManipulator*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleStateManipulator *rule) = 0;
};


class cEditPositionMinimum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPositionMinimum> Ref;
	cEditPositionMinimum(aeWPAPanelRuleStateManipulator &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumPosition())
			? aeUSetRuleSModMinPosition::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditPositionMaximum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPositionMaximum> Ref;
	cEditPositionMaximum(aeWPAPanelRuleStateManipulator &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumPosition())
			? aeUSetRuleSModMaxPosition::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditRotationMinimum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotationMinimum> Ref;
	cEditRotationMinimum(aeWPAPanelRuleStateManipulator &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumRotation())
			? aeUSetRuleSModMinRotation::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditRotationMaximum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotationMaximum> Ref;
	cEditRotationMaximum(aeWPAPanelRuleStateManipulator &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumRotation())
			? aeUSetRuleSModMaxRotation::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditScalingMinimum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditScalingMinimum> Ref;
	cEditScalingMinimum(aeWPAPanelRuleStateManipulator &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumSize())
			? aeUSetRuleSModMinSize::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditScalingMaximum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditScalingMaximum> Ref;
	cEditScalingMaximum(aeWPAPanelRuleStateManipulator &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumSize())
			? aeUSetRuleSModMaxSize::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditVertexPositionSetMinimum : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditVertexPositionSetMinimum> Ref;
	cEditVertexPositionSetMinimum(aeWPAPanelRuleStateManipulator &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleStateManipulator *rule) override{
		const float value = textField->GetFloat();
		return fabsf(value - rule->GetMinimumVertexPositionSet()) > FLOAT_SAFE_EPSILON
			? aeUSetRuleSModMinVertexPositionSet::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cEditVertexPositionSetMaximum : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditVertexPositionSetMaximum> Ref;
	cEditVertexPositionSetMaximum(aeWPAPanelRuleStateManipulator &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleStateManipulator *rule) override{
		const float value = textField->GetFloat();
		return fabsf(value - rule->GetMaximumVertexPositionSet()) > FLOAT_SAFE_EPSILON
			? aeUSetRuleSModMaxVertexPositionSet::Ref::New(rule, value) : igdeUndo::Ref();
	}
};


class cActionEnablePosition : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePosition> Ref;
	
public:
	cActionEnablePosition(aeWPAPanelRuleStateManipulator &panel) : cBaseAction(panel,
		"Enable position manipulation", nullptr, "Determines if the position is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateManipulator *rule) override{
		return aeUSetRuleSModEnablePos::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateManipulator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotation> Ref;
	
public:
	cActionEnableRotation(aeWPAPanelRuleStateManipulator &panel) : cBaseAction(panel,
		"Enable rotation manipulation", nullptr, "Determines if the rotation is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateManipulator *rule) override{
		return aeUSetRuleSModEnableRot::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateManipulator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableSize> Ref;
	
public:
	cActionEnableSize(aeWPAPanelRuleStateManipulator &panel) : cBaseAction(panel,
		"Enable size manipulation", nullptr, "Determines if the size is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateManipulator *rule) override{
		return aeUSetRuleSModEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateManipulator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableVertexPositionSet> Ref;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleStateManipulator &panel) : cBaseAction(panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if vertex position set is modified or kept as it is"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleStateManipulator *rule) override{
		return aeUSetRuleSModEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleStateManipulator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

}



// Class aeWPAPanelRuleStateManipulator
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleStateManipulator::aeWPAPanelRuleStateManipulator(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertStateManipulator)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "State Manipulator:");
	
	helper.EditVector(groupBox, "Min Position:", "Minimum position",
		pEditMinPos, cEditPositionMinimum::Ref::New(*this));
	helper.EditVector(groupBox, "Max Position:", "Maximum position",
		pEditMaxPos, cEditPositionMaximum::Ref::New(*this));
	
	helper.EditVector(groupBox, "Min Rotation:", "Minimum rotation",
		pEditMinRot, cEditRotationMinimum::Ref::New(*this));
	helper.EditVector(groupBox, "Max Rotation:", "Maximum rotation",
		pEditMaxRot, cEditRotationMaximum::Ref::New(*this));
	
	helper.EditVector(groupBox, "Min Scaling:", "Minimum scaling",
		pEditMinSize, cEditScalingMinimum::Ref::New(*this));
	helper.EditVector(groupBox, "Max Scaling:", "Maximum scaling",
		pEditMaxSize, cEditScalingMaximum::Ref::New(*this));
	
	helper.EditFloat(groupBox, "Min Vertex Position Set:", "Minimum vertex position set",
		pEditMinVertexPositionSet, cEditVertexPositionSetMinimum::Ref::New(*this));
	helper.EditFloat(groupBox, "Max Vertex Position Set:", "Maximum vertex position set",
		pEditMaxVertexPositionSet, cEditVertexPositionSetMaximum::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
}

aeWPAPanelRuleStateManipulator::~aeWPAPanelRuleStateManipulator(){
}



// Management
///////////////

void aeWPAPanelRuleStateManipulator::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleStateManipulator * const rule = (aeRuleStateManipulator*)GetRule();
	
	if(rule){
		pEditMinPos->SetVector(rule->GetMinimumPosition());
		pEditMaxPos->SetVector(rule->GetMaximumPosition());
		pEditMinRot->SetVector(rule->GetMinimumRotation());
		pEditMaxRot->SetVector(rule->GetMaximumRotation());
		pEditMinSize->SetVector(rule->GetMinimumSize());
		pEditMaxSize->SetVector(rule->GetMaximumSize());
		pEditMinVertexPositionSet->SetFloat(rule->GetMinimumVertexPositionSet());
		pEditMaxVertexPositionSet->SetFloat(rule->GetMaximumVertexPositionSet());
		
	}else{
		pEditMinPos->SetVector(decVector());
		pEditMaxPos->SetVector(decVector());
		pEditMinRot->SetVector(decVector());
		pEditMaxRot->SetVector(decVector());
		pEditMinSize->SetVector(decVector());
		pEditMaxSize->SetVector(decVector());
		pEditMinVertexPositionSet->SetFloat(0.0f);
		pEditMaxVertexPositionSet->SetFloat(1.0f);
	}
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}

void aeWPAPanelRuleStateManipulator::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleStateManipulator * const rule = (aeRuleStateManipulator*)GetRule();
	if(rule){
		AddTarget("Position", &rule->GetTargetPosition());
		AddTarget("Rotation ", &rule->GetTargetRotation());
		AddTarget("Size", &rule->GetTargetSize());
		AddTarget("Vertex Position Set", &rule->GetTargetVertexPositionSet());
	}
}
