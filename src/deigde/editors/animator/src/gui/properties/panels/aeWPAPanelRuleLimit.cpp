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

#include "aeWPAPanelRuleLimit.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleLimit.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetPosMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetPosMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetRotMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetRotMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetScaleMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetScaleMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetVertexPositionSetMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetVertexPositionSetMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetCFrame.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnablePosXMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnablePosXMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnablePosYMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnablePosYMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnablePosZMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnablePosZMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableRotXMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableRotXMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableRotYMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableRotYMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableRotZMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableRotZMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableScaleXMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableScaleXMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableScaleYMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableScaleYMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableScaleZMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableScaleZMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableVertexPositionSetMin.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetEnableVertexPositionSetMax.h"
#include "../../../undosys/rule/limit/aeURuleLimitSetTargetBone.h"
#include "../../../animatoreditor.h"

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
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
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
	aeWPAPanelRuleLimit &pPanel;
	
public:
	cBaseAction(aeWPAPanelRuleLimit &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeRuleLimit *rule) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleLimit &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleLimit &pPanel;
	
public:
	cBaseComboBoxListener(aeWPAPanelRuleLimit &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(comboBox, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleLimit *rule) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPAPanelRuleLimit &pPanel;
	
public:
	cBaseEditVectorListener(aeWPAPanelRuleLimit &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeEditVector *editVector, aeAnimator *animator, aeRuleLimit *rule) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleLimit &pPanel;
	
public:
	cBaseTextFieldListener(aeWPAPanelRuleLimit &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleLimit *rule) = 0;
};


class cEditPositionMinimum : public cBaseEditVectorListener{
public:
	cEditPositionMinimum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule){
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumPosition())
			? new aeURuleLimitSetPosMin(rule, editVector->GetVector()) : NULL;
	}
};

class cEditPositionMaximum : public cBaseEditVectorListener{
public:
	cEditPositionMaximum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule){
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumPosition())
			? new aeURuleLimitSetPosMax(rule, editVector->GetVector()) : NULL;
	}
};

class cEditRotationMinimum : public cBaseEditVectorListener{
public:
	cEditRotationMinimum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule){
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumRotation())
			? new aeURuleLimitSetRotMin(rule, editVector->GetVector()) : NULL;
	}
};

class cEditRotationMaximum : public cBaseEditVectorListener{
public:
	cEditRotationMaximum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule){
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumRotation())
			? new aeURuleLimitSetRotMax(rule, editVector->GetVector()) : NULL;
	}
};

class cEditScalingMinimum : public cBaseEditVectorListener{
public:
	cEditScalingMinimum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule){
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumScaling())
			? new aeURuleLimitSetScaleMin(rule, editVector->GetVector()) : NULL;
	}
};

class cEditScalingMaximum : public cBaseEditVectorListener{
public:
	cEditScalingMaximum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule){
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumScaling())
			? new aeURuleLimitSetScaleMax(rule, editVector->GetVector()) : NULL;
	}
};

class cEditVertexPositionSetMinimum : public cBaseTextFieldListener{
public:
	cEditVertexPositionSetMinimum(aeWPAPanelRuleLimit &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeRuleLimit *rule){
		const float value = textField->GetFloat();
		return fabsf(value - rule->GetMinimumVertexPositionSet()) > FLOAT_SAFE_EPSILON
			? new aeURuleLimitSetVertexPositionSetMin(rule, value) : nullptr;
	}
};

class cEditVertexPositionSetMaximum : public cBaseTextFieldListener{
public:
	cEditVertexPositionSetMaximum(aeWPAPanelRuleLimit &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeRuleLimit *rule){
		const float value = textField->GetFloat();
		return fabsf(value - rule->GetMaximumVertexPositionSet()) > FLOAT_SAFE_EPSILON
			? new aeURuleLimitSetVertexPositionSetMax(rule, value) : nullptr;
	}
};

class cComboCoordFrame : public cBaseComboBoxListener{
public:
	cComboCoordFrame(aeWPAPanelRuleLimit &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleLimit *rule){
		if(!comboBox->GetSelectedItem()){
			return NULL;
		}
		
		deAnimatorRuleLimit::eCoordinateFrames value = (deAnimatorRuleLimit::eCoordinateFrames)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetCoordinateFrame() != value ? new aeURuleLimitSetCFrame(rule, value) : NULL;
	}
};


class cActionEnablePositionMinX : public cBaseAction{
public:
	cActionEnablePositionMinX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", NULL, "Determines if the x position of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnablePosXMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionXMin());
	}
};

class cActionEnablePositionMinY : public cBaseAction{
public:
	cActionEnablePositionMinY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", NULL, "Determines if the y position of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnablePosYMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionYMin());
	}
};

class cActionEnablePositionMinZ : public cBaseAction{
public:
	cActionEnablePositionMinZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", NULL, "Determines if the z position of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnablePosZMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionZMin());
	}
};

class cActionEnablePositionMaxX : public cBaseAction{
public:
	cActionEnablePositionMaxX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", NULL, "Determines if the x position of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnablePosXMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionXMax());
	}
};

class cActionEnablePositionMaxY : public cBaseAction{
public:
	cActionEnablePositionMaxY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", NULL, "Determines if the y position of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnablePosYMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionYMax());
	}
};

class cActionEnablePositionMaxZ : public cBaseAction{
public:
	cActionEnablePositionMaxZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", NULL, "Determines if the z position of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnablePosZMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionZMax());
	}
};


class cActionEnableRotationMinX : public cBaseAction{
public:
	cActionEnableRotationMinX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", NULL, "Determines if the x rotation of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableRotXMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationXMin());
	}
};

class cActionEnableRotationMinY : public cBaseAction{
public:
	cActionEnableRotationMinY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", NULL, "Determines if the y rotation of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableRotYMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationYMin());
	}
};

class cActionEnableRotationMinZ : public cBaseAction{
public:
	cActionEnableRotationMinZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", NULL, "Determines if the z rotation of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableRotZMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationZMin());
	}
};

class cActionEnableRotationMaxX : public cBaseAction{
public:
	cActionEnableRotationMaxX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", NULL, "Determines if the x rotation of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableRotXMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationXMax());
	}
};

class cActionEnableRotationMaxY : public cBaseAction{
public:
	cActionEnableRotationMaxY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", NULL, "Determines if the y rotation of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableRotYMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationYMax());
	}
};

class cActionEnableRotationMaxZ : public cBaseAction{
public:
	cActionEnableRotationMaxZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", NULL, "Determines if the z rotation of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableRotZMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationZMax());
	}
};


class cActionEnableScalingMinX : public cBaseAction{
public:
	cActionEnableScalingMinX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", NULL, "Determines if the x scaling of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableScaleXMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingXMin());
	}
};

class cActionEnableScalingMinY : public cBaseAction{
public:
	cActionEnableScalingMinY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", NULL, "Determines if the y scaling of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableScaleYMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingYMin());
	}
};

class cActionEnableScalingMinZ : public cBaseAction{
public:
	cActionEnableScalingMinZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", NULL, "Determines if the z scaling of bones is limited against the minimum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableScaleZMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingZMin());
	}
};

class cActionEnableScalingMaxX : public cBaseAction{
public:
	cActionEnableScalingMaxX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", NULL, "Determines if the x scaling of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableScaleXMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingXMax());
	}
};

class cActionEnableScalingMaxY : public cBaseAction{
public:
	cActionEnableScalingMaxY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", NULL, "Determines if the y scaling of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableScaleYMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingYMax());
	}
};

class cActionEnableScalingMaxZ : public cBaseAction{
public:
	cActionEnableScalingMaxZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", NULL, "Determines if the z scaling of bones is limited against the maximum"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableScaleZMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingZMax());
	}
};

class cActionEnableVertexPositionSetMin : public cBaseAction{
public:
	cActionEnableVertexPositionSetMin(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Minimum", nullptr, "Scaling of vertex position sets is limited against the minimum"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableVertexPositionSetMin(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSetMin());
	}
};

class cActionEnableVertexPositionSetMax : public cBaseAction{
public:
	cActionEnableVertexPositionSetMax(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Maximum", nullptr, "Scaling of vertex position sets is limited against the maximum"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRuleLimit *rule){
		return new aeURuleLimitSetEnableVertexPositionSetMax(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSetMax());
	}
};


class cComboTargetBone : public cBaseComboBoxListener{
public:
	cComboTargetBone(aeWPAPanelRuleLimit &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleLimit *rule){
		return comboBox->GetText() != rule->GetTargetBone()
			? new aeURuleLimitSetTargetBone(rule, comboBox->GetText()) : NULL;
	}
};

}



// Class aeWPAPanelRuleLimit
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleLimit::aeWPAPanelRuleLimit(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertLimit)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer *groupBox, formLine;
	
	
	helper.GroupBox(*this, groupBox, "Limit:");
	
	helper.EditVector(groupBox, "Min Position:", "Minimum position",
		pEditMinPos, new cEditPositionMinimum(*this));
	helper.EditVector(groupBox, "Max Position:", "Maximum position",
		pEditMaxPos, new cEditPositionMaximum(*this));
	
	helper.EditVector(groupBox, "Min Rotation:", "Minimum rotation",
		pEditMinRot, new cEditRotationMinimum(*this));
	helper.EditVector(groupBox, "Max Rotation:", "Maximum rotation",
		pEditMaxRot, new cEditRotationMaximum(*this));
	
	helper.EditVector(groupBox, "Min Scaling:", "Minimum scaling",
		pEditMinScale, new cEditScalingMinimum(*this));
	helper.EditVector(groupBox, "Max Scaling:", "Maximum scaling",
		pEditMaxScale, new cEditScalingMaximum(*this));
	
	helper.EditFloat(groupBox, "Min Vertex Position Set:", "Minimum vertex position set",
		pEditMinVertexPositionSet, new cEditVertexPositionSetMinimum(*this));
	helper.EditFloat(groupBox, "Max Vertex Position Set:", "Maximum vertex position set",
		pEditMaxVertexPositionSet, new cEditVertexPositionSetMaximum(*this));
	
	helper.ComboBox(groupBox, "Coord-Frame:", "Sets the coordinate frame to use for rotation",
		pCBCoordFrame, new cComboCoordFrame(*this));
	pCBCoordFrame->AddItem("Bone Local", NULL, (void*)(intptr_t)deAnimatorRuleLimit::ecfBoneLocal);
	pCBCoordFrame->AddItem("Component", NULL, (void*)(intptr_t)deAnimatorRuleLimit::ecfComponent);
	pCBCoordFrame->AddItem("Target Bone", NULL, (void*)(intptr_t)deAnimatorRuleLimit::ecfTargetBone);
	
	helper.ComboBoxFilter(groupBox, "Target Bone:", true, "Sets the bone to use as coordinate frame",
		pCBTargetBone, new cComboTargetBone(*this));
	pCBTargetBone->SetDefaultSorter();
	
	helper.FormLine(groupBox, "Enable Min Pos", "Limit minimum position", formLine);
	helper.CheckBoxOnly(formLine, pChkEnablePosXMin, new cActionEnablePositionMinX(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnablePosYMin, new cActionEnablePositionMinY(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnablePosZMin, new cActionEnablePositionMinZ(*this), true);
	
	helper.FormLine(groupBox, "Enable Max Pos", "Limit maximum position", formLine);
	helper.CheckBoxOnly(formLine, pChkEnablePosXMax, new cActionEnablePositionMaxX(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnablePosYMax, new cActionEnablePositionMaxY(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnablePosZMax, new cActionEnablePositionMaxZ(*this), true);
	
	helper.FormLine(groupBox, "Enable Min Rot", "Limit minimum rotation", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableRotXMin, new cActionEnableRotationMinX(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableRotYMin, new cActionEnableRotationMinY(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableRotZMin, new cActionEnableRotationMinZ(*this), true);
	
	helper.FormLine(groupBox, "Enable Max Rot", "Limit maximum rotation", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableRotXMax, new cActionEnableRotationMaxX(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableRotYMax, new cActionEnableRotationMaxY(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableRotZMax, new cActionEnableRotationMaxZ(*this), true);
	
	helper.FormLine(groupBox, "Enable Min Scale", "Limit minimum scaling", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableScaleXMin, new cActionEnableScalingMinX(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableScaleYMin, new cActionEnableScalingMinY(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableScaleZMin, new cActionEnableScalingMinZ(*this), true);
	
	helper.FormLine(groupBox, "Enable Max Scale", "Limit maximum scaling", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableScaleXMax, new cActionEnableScalingMaxX(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableScaleYMax, new cActionEnableScalingMaxY(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableScaleZMax, new cActionEnableScalingMaxZ(*this), true);
	
	helper.FormLine(groupBox, "Enable Vertex Position Set", "Limit vertex position set", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableVertexPositionSetMin, new cActionEnableVertexPositionSetMin(*this), true);
	helper.CheckBoxOnly(formLine, pChkEnableVertexPositionSetMax, new cActionEnableVertexPositionSetMax(*this), true);
}

aeWPAPanelRuleLimit::~aeWPAPanelRuleLimit(){
}



// Management
///////////////

void aeWPAPanelRuleLimit::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selection(pCBTargetBone->GetText());
	
	pCBTargetBone->RemoveAllItems();
	
	if(GetAnimator()){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			for(i=0; i<count; i++){
				pCBTargetBone->AddItem(rig->GetBoneAt(i).GetName());
			}
		}
		pCBTargetBone->SortItems();
	}
	
	pCBTargetBone->StoreFilterItems();
	pCBTargetBone->SetText(selection);
}

void aeWPAPanelRuleLimit::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleLimit * const rule = (aeRuleLimit*)GetRule();
	
	if(rule){
		pEditMinPos->SetVector(rule->GetMinimumPosition());
		pEditMaxPos->SetVector(rule->GetMaximumPosition());
		pEditMinRot->SetVector(rule->GetMinimumRotation());
		pEditMaxRot->SetVector(rule->GetMaximumRotation());
		pEditMinScale->SetVector(rule->GetMinimumScaling());
		pEditMaxScale->SetVector(rule->GetMaximumScaling());
		pEditMinVertexPositionSet->SetFloat(rule->GetMinimumVertexPositionSet());
		pEditMaxVertexPositionSet->SetFloat(rule->GetMaximumVertexPositionSet());
		pCBCoordFrame->SetSelectionWithData((void*)(intptr_t)rule->GetCoordinateFrame());
		pCBTargetBone->SetText(rule->GetTargetBone());
		
	}else{
		pEditMinPos->SetVector(decVector());
		pEditMaxPos->SetVector(decVector());
		pEditMinRot->SetVector(decVector());
		pEditMaxRot->SetVector(decVector());
		pEditMinScale->SetVector(decVector());
		pEditMaxScale->SetVector(decVector());
		pEditMinVertexPositionSet->SetFloat(0.0f);
		pEditMaxVertexPositionSet->SetFloat(1.0f);
		pCBCoordFrame->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleLimit::ecfComponent);
		pCBTargetBone->ClearText();
	}
	
	const bool enabled = rule;
	pEditMinPos->SetEnabled(enabled);
	pEditMaxPos->SetEnabled(enabled);
	pEditMinRot->SetEnabled(enabled);
	pEditMaxRot->SetEnabled(enabled);
	pEditMinScale->SetEnabled(enabled);
	pEditMaxScale->SetEnabled(enabled);
	pEditMinVertexPositionSet->SetEnabled(enabled);
	pEditMaxVertexPositionSet->SetEnabled(enabled);
	pCBCoordFrame->SetEnabled(enabled);
	pCBTargetBone->SetEnabled(enabled);
	
	pChkEnablePosXMin->GetAction()->Update();
	pChkEnablePosXMax->GetAction()->Update();
	pChkEnablePosYMin->GetAction()->Update();
	pChkEnablePosYMax->GetAction()->Update();
	pChkEnablePosZMin->GetAction()->Update();
	pChkEnablePosZMax->GetAction()->Update();
	
	pChkEnableRotXMin->GetAction()->Update();
	pChkEnableRotXMax->GetAction()->Update();
	pChkEnableRotYMin->GetAction()->Update();
	pChkEnableRotYMax->GetAction()->Update();
	pChkEnableRotZMin->GetAction()->Update();
	pChkEnableRotZMax->GetAction()->Update();
	
	pChkEnableScaleXMin->GetAction()->Update();
	pChkEnableScaleXMax->GetAction()->Update();
	pChkEnableScaleYMin->GetAction()->Update();
	pChkEnableScaleYMax->GetAction()->Update();
	pChkEnableScaleZMin->GetAction()->Update();
	pChkEnableScaleZMax->GetAction()->Update();
	
	pChkEnableVertexPositionSetMin->GetAction()->Update();
	pChkEnableVertexPositionSetMax->GetAction()->Update();
}
