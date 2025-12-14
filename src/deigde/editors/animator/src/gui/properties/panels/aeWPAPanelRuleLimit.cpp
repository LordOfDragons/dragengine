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
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPAPanelRuleLimit &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleLimit *rule) = 0;
	
	void Update() override{
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
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(aeWPAPanelRuleLimit &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleLimit *rule) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPAPanelRuleLimit &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(aeWPAPanelRuleLimit &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator *animator, aeRuleLimit *rule) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleLimit &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(aeWPAPanelRuleLimit &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleLimit * const rule = (aeRuleLimit*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeRuleLimit *rule) = 0;
};


class cEditPositionMinimum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPositionMinimum> Ref;
	cEditPositionMinimum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumPosition())
			? aeURuleLimitSetPosMin::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditPositionMaximum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPositionMaximum> Ref;
	cEditPositionMaximum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumPosition())
			? aeURuleLimitSetPosMax::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditRotationMinimum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotationMinimum> Ref;
	cEditRotationMinimum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumRotation())
			? aeURuleLimitSetRotMin::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditRotationMaximum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotationMaximum> Ref;
	cEditRotationMaximum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumRotation())
			? aeURuleLimitSetRotMax::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditScalingMinimum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditScalingMinimum> Ref;
	cEditScalingMinimum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMinimumScaling())
			? aeURuleLimitSetScaleMin::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditScalingMaximum : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditScalingMaximum> Ref;
	cEditScalingMaximum(aeWPAPanelRuleLimit &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditVector *editVector, aeAnimator*, aeRuleLimit *rule) override{
		return !editVector->GetVector().IsEqualTo(rule->GetMaximumScaling())
			? aeURuleLimitSetScaleMax::Ref::New(rule, editVector->GetVector()) : igdeUndo::Ref();
	}
};

class cEditVertexPositionSetMinimum : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditVertexPositionSetMinimum> Ref;
	cEditVertexPositionSetMinimum(aeWPAPanelRuleLimit &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleLimit *rule) override{
		const float value = textField->GetFloat();
		return fabsf(value - rule->GetMinimumVertexPositionSet()) > FLOAT_SAFE_EPSILON
			? aeURuleLimitSetVertexPositionSetMin::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cEditVertexPositionSetMaximum : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditVertexPositionSetMaximum> Ref;
	cEditVertexPositionSetMaximum(aeWPAPanelRuleLimit &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRuleLimit *rule) override{
		const float value = textField->GetFloat();
		return fabsf(value - rule->GetMaximumVertexPositionSet()) > FLOAT_SAFE_EPSILON
			? aeURuleLimitSetVertexPositionSetMax::Ref::New(rule, value) : igdeUndo::Ref();
	}
};

class cComboCoordFrame : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCoordFrame> Ref;
	cComboCoordFrame(aeWPAPanelRuleLimit &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleLimit *rule) override{
		if(!comboBox->GetSelectedItem()){
			return {};
		}
		
		deAnimatorRuleLimit::eCoordinateFrames value = (deAnimatorRuleLimit::eCoordinateFrames)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetCoordinateFrame() != value ? aeURuleLimitSetCFrame::Ref::New(rule, value) : aeURuleLimitSetCFrame::Ref();
	}
};


class cActionEnablePositionMinX : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePositionMinX> Ref;
	
public:
	cActionEnablePositionMinX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", nullptr, "Determines if the x position of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnablePosXMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionXMin());
	}
};

class cActionEnablePositionMinY : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePositionMinY> Ref;
	
public:
	cActionEnablePositionMinY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", nullptr, "Determines if the y position of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnablePosYMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionYMin());
	}
};

class cActionEnablePositionMinZ : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePositionMinZ> Ref;
	
public:
	cActionEnablePositionMinZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", nullptr, "Determines if the z position of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnablePosZMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionZMin());
	}
};

class cActionEnablePositionMaxX : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePositionMaxX> Ref;
	
public:
	cActionEnablePositionMaxX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", nullptr, "Determines if the x position of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnablePosXMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionXMax());
	}
};

class cActionEnablePositionMaxY : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePositionMaxY> Ref;
	
public:
	cActionEnablePositionMaxY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", nullptr, "Determines if the y position of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnablePosYMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionYMax());
	}
};

class cActionEnablePositionMaxZ : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePositionMaxZ> Ref;
	
public:
	cActionEnablePositionMaxZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", nullptr, "Determines if the z position of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnablePosZMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePositionZMax());
	}
};


class cActionEnableRotationMinX : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotationMinX> Ref;
	
public:
	cActionEnableRotationMinX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", nullptr, "Determines if the x rotation of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableRotXMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationXMin());
	}
};

class cActionEnableRotationMinY : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotationMinY> Ref;
	
public:
	cActionEnableRotationMinY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", nullptr, "Determines if the y rotation of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableRotYMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationYMin());
	}
};

class cActionEnableRotationMinZ : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotationMinZ> Ref;
	
public:
	cActionEnableRotationMinZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", nullptr, "Determines if the z rotation of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableRotZMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationZMin());
	}
};

class cActionEnableRotationMaxX : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotationMaxX> Ref;
	
public:
	cActionEnableRotationMaxX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", nullptr, "Determines if the x rotation of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableRotXMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationXMax());
	}
};

class cActionEnableRotationMaxY : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotationMaxY> Ref;
	
public:
	cActionEnableRotationMaxY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", nullptr, "Determines if the y rotation of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableRotYMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationYMax());
	}
};

class cActionEnableRotationMaxZ : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableRotationMaxZ> Ref;
	
public:
	cActionEnableRotationMaxZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", nullptr, "Determines if the z rotation of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableRotZMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableRotationZMax());
	}
};


class cActionEnableScalingMinX : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableScalingMinX> Ref;
	
public:
	cActionEnableScalingMinX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", nullptr, "Determines if the x scaling of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableScaleXMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingXMin());
	}
};

class cActionEnableScalingMinY : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableScalingMinY> Ref;
	
public:
	cActionEnableScalingMinY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", nullptr, "Determines if the y scaling of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableScaleYMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingYMin());
	}
};

class cActionEnableScalingMinZ : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableScalingMinZ> Ref;
	
public:
	cActionEnableScalingMinZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", nullptr, "Determines if the z scaling of bones is limited against the minimum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableScaleZMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingZMin());
	}
};

class cActionEnableScalingMaxX : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableScalingMaxX> Ref;
	
public:
	cActionEnableScalingMaxX(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"X", nullptr, "Determines if the x scaling of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableScaleXMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingXMax());
	}
};

class cActionEnableScalingMaxY : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableScalingMaxY> Ref;
	
public:
	cActionEnableScalingMaxY(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Y", nullptr, "Determines if the y scaling of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableScaleYMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingYMax());
	}
};

class cActionEnableScalingMaxZ : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableScalingMaxZ> Ref;
	
public:
	cActionEnableScalingMaxZ(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Z", nullptr, "Determines if the z scaling of bones is limited against the maximum"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableScaleZMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableScalingZMax());
	}
};

class cActionEnableVertexPositionSetMin : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableVertexPositionSetMin> Ref;
	
public:
	cActionEnableVertexPositionSetMin(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Minimum", nullptr, "Scaling of vertex position sets is limited against the minimum"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableVertexPositionSetMin::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSetMin());
	}
};

class cActionEnableVertexPositionSetMax : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableVertexPositionSetMax> Ref;
	
public:
	cActionEnableVertexPositionSetMax(aeWPAPanelRuleLimit &panel) : cBaseAction(panel,
		"Maximum", nullptr, "Scaling of vertex position sets is limited against the maximum"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleLimit *rule) override{
		return aeURuleLimitSetEnableVertexPositionSetMax::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleLimit &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSetMax());
	}
};


class cComboTargetBone : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTargetBone> Ref;
	cComboTargetBone(aeWPAPanelRuleLimit &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleLimit *rule) override{
		return comboBox->GetText() != rule->GetTargetBone()
			? aeURuleLimitSetTargetBone::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
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
	igdeContainer::Ref groupBox, formLine;
	
	
	helper.GroupBox(*this, groupBox, "Limit:");
	
	helper.EditVector(groupBox, "Min Position:", "Minimum position",
		pEditMinPos, cEditPositionMinimum::Ref::New(*this));
	helper.EditVector(groupBox, "Max Position:", "Maximum position",
		pEditMaxPos, cEditPositionMaximum::Ref::New(*this));
	
	helper.EditVector(groupBox, "Min Rotation:", "Minimum rotation",
		pEditMinRot, cEditRotationMinimum::Ref::New(*this));
	helper.EditVector(groupBox, "Max Rotation:", "Maximum rotation",
		pEditMaxRot, cEditRotationMaximum::Ref::New(*this));
	
	helper.EditVector(groupBox, "Min Scaling:", "Minimum scaling",
		pEditMinScale, cEditScalingMinimum::Ref::New(*this));
	helper.EditVector(groupBox, "Max Scaling:", "Maximum scaling",
		pEditMaxScale, cEditScalingMaximum::Ref::New(*this));
	
	helper.EditFloat(groupBox, "Min Vertex Position Set:", "Minimum vertex position set",
		pEditMinVertexPositionSet, cEditVertexPositionSetMinimum::Ref::New(*this));
	helper.EditFloat(groupBox, "Max Vertex Position Set:", "Maximum vertex position set",
		pEditMaxVertexPositionSet, cEditVertexPositionSetMaximum::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Coord-Frame:", "Sets the coordinate frame to use for rotation",
		pCBCoordFrame, cComboCoordFrame::Ref::New(*this));
	pCBCoordFrame->AddItem("Bone Local", nullptr, (void*)(intptr_t)deAnimatorRuleLimit::ecfBoneLocal);
	pCBCoordFrame->AddItem("Component", nullptr, (void*)(intptr_t)deAnimatorRuleLimit::ecfComponent);
	pCBCoordFrame->AddItem("Target Bone", nullptr, (void*)(intptr_t)deAnimatorRuleLimit::ecfTargetBone);
	
	helper.ComboBoxFilter(groupBox, "Target Bone:", true, "Sets the bone to use as coordinate frame",
		pCBTargetBone, cComboTargetBone::Ref::New(*this));
	pCBTargetBone->SetDefaultSorter();
	
	helper.FormLine(groupBox, "Enable Min Pos", "Limit minimum position", formLine);
	helper.CheckBoxOnly(formLine, pChkEnablePosXMin, cActionEnablePositionMinX::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnablePosYMin, cActionEnablePositionMinY::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnablePosZMin, cActionEnablePositionMinZ::Ref::New(*this));
	
	helper.FormLine(groupBox, "Enable Max Pos", "Limit maximum position", formLine);
	helper.CheckBoxOnly(formLine, pChkEnablePosXMax, cActionEnablePositionMaxX::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnablePosYMax, cActionEnablePositionMaxY::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnablePosZMax, cActionEnablePositionMaxZ::Ref::New(*this));
	
	helper.FormLine(groupBox, "Enable Min Rot", "Limit minimum rotation", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableRotXMin, cActionEnableRotationMinX::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableRotYMin, cActionEnableRotationMinY::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableRotZMin, cActionEnableRotationMinZ::Ref::New(*this));
	
	helper.FormLine(groupBox, "Enable Max Rot", "Limit maximum rotation", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableRotXMax, cActionEnableRotationMaxX::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableRotYMax, cActionEnableRotationMaxY::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableRotZMax, cActionEnableRotationMaxZ::Ref::New(*this));
	
	helper.FormLine(groupBox, "Enable Min Scale", "Limit minimum scaling", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableScaleXMin, cActionEnableScalingMinX::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableScaleYMin, cActionEnableScalingMinY::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableScaleZMin, cActionEnableScalingMinZ::Ref::New(*this));
	
	helper.FormLine(groupBox, "Enable Max Scale", "Limit maximum scaling", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableScaleXMax, cActionEnableScalingMaxX::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableScaleYMax, cActionEnableScalingMaxY::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableScaleZMax, cActionEnableScalingMaxZ::Ref::New(*this));
	
	helper.FormLine(groupBox, "Enable Vertex Position Set", "Limit vertex position set", formLine);
	helper.CheckBoxOnly(formLine, pChkEnableVertexPositionSetMin, cActionEnableVertexPositionSetMin::Ref::New(*this));
	helper.CheckBoxOnly(formLine, pChkEnableVertexPositionSetMax, cActionEnableVertexPositionSetMax::Ref::New(*this));
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
