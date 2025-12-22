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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "reWPConstraint.h"
#include "reWindowProperties.h"
#include "reWPConstraintListener.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/bone/reRigBone.h"
#include "../../rig/constraint/reRigConstraint.h"
#include "../../rig/constraint/reSelectionConstraints.h"
#include "../../rig/constraint/reRigConstraintDof.h"
#include "../../undosys/properties/constraint/reUConstraintDofSetKinematicFriction.h"
#include "../../undosys/properties/constraint/reUConstraintDofSetLowerLimit.h"
#include "../../undosys/properties/constraint/reUConstraintDofSetSpringStiffness.h"
#include "../../undosys/properties/constraint/reUConstraintDofSetStaticFriction.h"
#include "../../undosys/properties/constraint/reUConstraintDofSetUpperLimit.h"
#include "../../undosys/properties/constraint/reUConstraintSetBoneTarget.h"
#include "../../undosys/properties/constraint/reUConstraintSetDampingAngular.h"
#include "../../undosys/properties/constraint/reUConstraintSetDampingLinear.h"
#include "../../undosys/properties/constraint/reUConstraintSetDampingSpring.h"
#include "../../undosys/properties/constraint/reUConstraintSetOffset.h"
#include "../../undosys/properties/constraint/reUConstraintSetOrientation.h"
#include "../../undosys/properties/constraint/reUConstraintSetPosition.h"
#include "../../undosys/properties/constraint/reUConstraintToggleIsRope.h"
#include "../../undosys/properties/constraint/reUConstraintSetBreakingThreshold.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>

static deColliderConstraint::eDegreesOfFreedom vDegreesOfFreedom[] = {
	deColliderConstraint::edofLinearX,
	deColliderConstraint::edofLinearY,
	deColliderConstraint::edofLinearZ,
	deColliderConstraint::edofAngularX,
	deColliderConstraint::edofAngularY,
	deColliderConstraint::edofAngularZ
};



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	reWPConstraint &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(reWPConstraint &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(!rig || !constraint){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, rig, constraint));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, reRig *rig, reRigConstraint *constraint) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPConstraint &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(reWPConstraint &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(!rig || !constraint){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), rig, constraint));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, reRig *rig, reRigConstraint *constraint) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPConstraint &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(reWPConstraint &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	void OnAction() override{
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(!rig || !constraint){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(rig, constraint));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(reRig *rig, reRigConstraint *constraint) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPConstraint &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(reWPConstraint &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(!rig || !constraint){
			return;
		}
		
		igdeUndo::Ref undo(OnTextChanged(comboBox, rig, constraint));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, reRig *rig, reRigConstraint *constraint) = 0;
};



class cComboTarget : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
	typedef deTObjectReference<cComboTarget> Ref;
	cComboTarget(reWPConstraint &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, reRig*, reRigConstraint *constraint){
		if(pPreventUpdate){
			return {};
		}
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *bone = nullptr;
		if(selection){
			bone = (reRigBone*)selection->GetData();
		}
		return bone != constraint->GetConstraintBone()
			? reUConstraintSetBoneTarget::Ref::New(constraint, bone) : igdeUndo::Ref();
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(reWPConstraint &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig*, reRigConstraint *constraint) override{
		return !vector.IsEqualTo(constraint->GetPosition())
			? reUConstraintSetPosition::Ref::New(constraint, vector) : igdeUndo::Ref();
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(reWPConstraint &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig*, reRigConstraint *constraint) override{
		return !vector.IsEqualTo(constraint->GetOrientation())
			? reUConstraintSetOrientation::Ref::New(constraint, vector) : igdeUndo::Ref();
	}
};

class cEditOffset : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditOffset> Ref;
	cEditOffset(reWPConstraint &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig*, reRigConstraint *constraint) override{
		return !vector.IsEqualTo(constraint->GetOffset())
			? reUConstraintSetOffset::Ref::New(constraint, vector) : igdeUndo::Ref();
	}
};

class cTextDofLower : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	typedef deTObjectReference<cTextDofLower> Ref;
	cTextDofLower(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const reRigConstraintDof &dof = constraint->GetDof(pDof);
		const float value = textField->GetFloat();
		return fabsf(value - dof.GetLowerLimit()) > dof.GetThresholdLimits()
			? reUConstraintDofSetLowerLimit::Ref::New(constraint, pDof, value) : igdeUndo::Ref();
	}
};

class cTextDofUpper : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	typedef deTObjectReference<cTextDofUpper> Ref;
	cTextDofUpper(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const reRigConstraintDof &dof = constraint->GetDof(pDof);
		const float value = textField->GetFloat();
		return fabsf(value - dof.GetUpperLimit()) > dof.GetThresholdLimits()
			? reUConstraintDofSetUpperLimit::Ref::New(constraint, pDof, value) : igdeUndo::Ref();
	}
};

class cTextDofFrictionStatic : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	typedef deTObjectReference<cTextDofFrictionStatic> Ref;
	cTextDofFrictionStatic(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetDof(pDof).GetStaticFriction()) > FLOAT_SAFE_EPSILON
			? reUConstraintDofSetStaticFriction::Ref::New(constraint, pDof, value) : igdeUndo::Ref();
	}
};

class cTextDofFrictionKinematic : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	typedef deTObjectReference<cTextDofFrictionKinematic> Ref;
	cTextDofFrictionKinematic(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetDof(pDof).GetKinematicFriction()) > FLOAT_SAFE_EPSILON
			? reUConstraintDofSetKinematicFriction::Ref::New(constraint, pDof, value) : igdeUndo::Ref();
	}
};

class cTextDofSpringStiffness : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	typedef deTObjectReference<cTextDofSpringStiffness> Ref;
	cTextDofSpringStiffness(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetDof(pDof).GetSpringStiffness()) > FLOAT_SAFE_EPSILON
			? reUConstraintDofSetSpringStiffness::Ref::New(constraint, pDof, value) : igdeUndo::Ref();
	}
};

class cTextDampingLinear : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextDampingLinear> Ref;
	cTextDampingLinear(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetLinearDamping()) > FLOAT_SAFE_EPSILON
			? reUConstraintSetDampingLinear::Ref::New(constraint, value) : igdeUndo::Ref();
	}
};

class cTextDampingAngular : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextDampingAngular> Ref;
	cTextDampingAngular(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetAngularDamping()) > FLOAT_SAFE_EPSILON
			? reUConstraintSetDampingAngular::Ref::New(constraint, value) : igdeUndo::Ref();
	}
};

class cTextDampingSpring : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextDampingSpring> Ref;
	cTextDampingSpring(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetSpringDamping()) > FLOAT_SAFE_EPSILON
			? reUConstraintSetDampingSpring::Ref::New(constraint, value) : igdeUndo::Ref();
	}
};

class cCheckRope : public cBaseAction{
public:
	typedef deTObjectReference<cCheckRope> Ref;
	cCheckRope(reWPConstraint &panel) : cBaseAction(panel, "Use rope physics",
		"Determines if this constraint is a rope constraint."){ }
	
	igdeUndo::Ref OnAction(reRig*, reRigConstraint *constraint) override{
		return reUConstraintToggleIsRope::Ref::New(constraint);
	}
};

class cTextBreakingThreshold : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBreakingThreshold> Ref;
	cTextBreakingThreshold(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint) override{
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetBreakingThreshold()) > FLOAT_SAFE_EPSILON
			? reUConstraintSetBreakingThreshold::Ref::New(constraint, value) : igdeUndo::Ref();
	}
};

class cCheckShowJointError : public cBaseAction{
public:
	typedef deTObjectReference<cCheckShowJointError> Ref;
	cCheckShowJointError(reWPConstraint &panel) : cBaseAction(panel, "Show joint error",
		"Shows joint errors visually during simulation."){}
	
	igdeUndo::Ref OnAction(reRig*, reRigConstraint *constraint) override{
		constraint->SetShowJointError(!constraint->GetShowJointError());
		return {};
	}
};

}



// Class reWPConstraint
/////////////////////////

// Constructor, destructor
////////////////////////////

reWPConstraint::reWPConstraint(reWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pPreventUpdate(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = reWPConstraintListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// structure
	helper.GroupBox(content, groupBox, "Link Partners:");
	
	helper.EditString(groupBox, "Parent:", "Name of parent bone or empty string for entire rig.",
		pEditBoneParent, {});
	pEditBoneParent->SetEditable(false);
	
	helper.ComboBox(groupBox, "Target:", true, "Name of target bone or empty string for the world.",
		pCBBoneTarget, cComboTarget::Ref::New(*this, pPreventUpdate));
	pCBBoneTarget->SetDefaultSorter();
	
	
	// structure
	helper.GroupBox(content, groupBox, "Geometry:");
	
	helper.EditVector(groupBox, "Position:",
		"Position of the constraint relative to the bone local coordinate system.",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:",
		"Rotation of the constraint relative to the bone local coordinate system.",
		pEditRotation, cEditRotation::Ref::New(*this));
	helper.EditVector(groupBox, "Offset:",
		"Offset of the bone local coordinate system relative to the constraint relative cooridnate system.",
		pEditOffset, cEditOffset::Ref::New(*this));
	
	
	// degree of freedoms
	const char * const dofGroupText[6] = {
		"DOF Linear X:", "DOF Linear Y:", "DOF Linear Z:",
		"DOF Angular X:", "DOF Angular Y:", "DOF Angular Z:"
	};
	int dof;
	
	for(dof=0; dof<6; dof++){
		helper.GroupBox(content, groupBox, dofGroupText[dof], dof < 3);
		
		helper.FormLine(groupBox, "Range:",
			"Lower and upper limit relative to constraint coordinate system. "
			"locked(lower=upper) limited(upper>lower) free(upper<lower)", frameLine);
		helper.EditFloat(frameLine, "Lower limit relative to constraint coordinate system.",
			pEditDofLower[dof], cTextDofLower::Ref::New(*this, vDegreesOfFreedom[dof]));
		helper.EditFloat(frameLine, "Upper limit relative to constraint coordinate system.",
			pEditDofUpper[dof], cTextDofUpper::Ref::New(*this, vDegreesOfFreedom[dof]));
		
		helper.FormLine(groupBox, "Friction:",
			"Static friction force and kinematic friction factor for joint inner friction.",
			frameLine);
			
		helper.EditFloat(frameLine, "Static friction force in newton per meter.",
			pEditDofStaFric[dof], cTextDofFrictionStatic::Ref::New(*this, vDegreesOfFreedom[dof]));
		helper.EditFloat(frameLine, "Kinematic friction factor.",
			pEditDofKinFric[dof], cTextDofFrictionKinematic::Ref::New(*this, vDegreesOfFreedom[dof]));
		
		helper.EditString(groupBox, "Stiffness:", "Spring stiffness in newton per meter.",
			pEditDofSprStiff[dof], cTextDofSpringStiffness::Ref::New(*this, vDegreesOfFreedom[dof]));
	}
	
	
	// damping
	helper.GroupBox(content, groupBox, "Damping:", true);
	
	helper.EditString(groupBox, "Linear:", "Damping of linear degrees of freedom.",
		pEditDampLin, cTextDampingLinear::Ref::New(*this));
	helper.EditString(groupBox, "Angular:", "Damping of angular degrees of freedom.",
		pEditDampAng, cTextDampingAngular::Ref::New(*this));
	helper.EditString(groupBox, "Spring:",
		"Damping of spring degrees of freedom both linear and angular.",
		pEditDampSpr, cTextDampingSpring::Ref::New(*this));
	
	
	// rope
	helper.GroupBox(content, groupBox, "Rope:", true);
	helper.CheckBox(groupBox, pChkRope, cCheckRope::Ref::New(*this));
	
	
	// breaking
	helper.GroupBox(content, groupBox, "Breaking:", true);
	
	helper.EditString(groupBox, "Breaking:",
		"Breaking threshold impuls in newton seconds or 0 to disable.",
		pEditBreakingThreshold, cTextBreakingThreshold::Ref::New(*this));
	
	
	// debugging
	helper.GroupBox(content, groupBox, "Debugging:", true);
	helper.CheckBox(groupBox, pChkShowJointError, cCheckShowJointError::Ref::New(*this));
}

reWPConstraint::~reWPConstraint(){
	SetRig(nullptr);
}



// Management
///////////////

void reWPConstraint::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	SetConstraint(nullptr);
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig = nullptr;
	}
	
	pRig = rig;
	
	if(rig){
		rig->AddNotifier(pListener);
		SetConstraint(rig->GetSelectionConstraints()->GetActiveConstraint());
	}
}

void reWPConstraint::SetConstraint(reRigConstraint *constraint){
	if(constraint == pConstraint){
		return;
	}
	pConstraint = constraint;
	UpdateBoneLists();
	UpdateConstraint();
}

void reWPConstraint::UpdateBoneLists(){
	reRigBone * const selection = pCBBoneTarget->GetSelectedItem()
		? (reRigBone*)pCBBoneTarget->GetSelectedItem()->GetData() : nullptr;
	pPreventUpdate = true;
	
	try{
		pCBBoneTarget->RemoveAllItems();
		pCBBoneTarget->AddItem("< World >", nullptr, nullptr);
		
		if(pRig && pConstraint){
			pRig->GetBones().Visit([&](reRigBone *bone){
				if(pConstraint->GetRigBone() != bone){
					pCBBoneTarget->AddItem(bone->GetName(), nullptr, bone);
				}
			});
		}
		
		pCBBoneTarget->SortItems();
		
		if(selection){
			pCBBoneTarget->SetSelection(pCBBoneTarget->IndexOfItemWithData(selection));
		}
		
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
}

void reWPConstraint::UpdateConstraint(){
	int dof;
	
	if(pConstraint){
		const reRigBone * const parentBone = pConstraint->GetRigBone();
		if(parentBone){
			pEditBoneParent->SetText(parentBone->GetName());
			
		}else{
			pEditBoneParent->SetText("< Rig >");
		}
		
		pCBBoneTarget->SetSelection(pCBBoneTarget->IndexOfItemWithData(
			pConstraint->GetConstraintBone()));
		
		pEditPosition->SetVector(pConstraint->GetPosition());
		pEditRotation->SetVector(pConstraint->GetOrientation());
		pEditOffset->SetVector(pConstraint->GetOffset());
		
		for(dof=0; dof<6; dof++){
			const reRigConstraintDof &rigDof = pConstraint->GetDof(vDegreesOfFreedom[dof]);
			pEditDofLower[dof]->SetFloat(rigDof.GetLowerLimit());
			pEditDofUpper[dof]->SetFloat(rigDof.GetUpperLimit());
			pEditDofStaFric[dof]->SetFloat(rigDof.GetStaticFriction());
			pEditDofKinFric[dof]->SetFloat(rigDof.GetKinematicFriction());
			pEditDofSprStiff[dof]->SetFloat(rigDof.GetSpringStiffness());
		}
		
		pEditDampLin->SetFloat(pConstraint->GetLinearDamping());
		pEditDampAng->SetFloat(pConstraint->GetAngularDamping());
		pEditDampSpr->SetFloat(pConstraint->GetSpringDamping());
		
		pChkRope->SetChecked(pConstraint->GetIsRope());
		
		pEditBreakingThreshold->SetFloat(pConstraint->GetBreakingThreshold());
		
		pChkShowJointError->SetChecked(pConstraint->GetShowJointError());
		
	}else{
		pEditBoneParent->ClearText();
		pCBBoneTarget->SetSelection(pCBBoneTarget->IndexOfItemWithData(nullptr));
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditOffset->SetVector(decVector());
		
		for(dof=0; dof<6; dof++){
			pEditDofLower[dof]->ClearText();
			pEditDofUpper[dof]->ClearText();
			pEditDofStaFric[dof]->ClearText();
			pEditDofKinFric[dof]->ClearText();
			pEditDofSprStiff[dof]->ClearText();
		}
		
		pEditDampLin->ClearText();
		pEditDampAng->ClearText();
		pEditDampSpr->ClearText();
		
		pChkRope->SetChecked(false);
		
		pEditBreakingThreshold->ClearText();
		
		pChkShowJointError->SetChecked(false);
	}
	
	// enable UI
	const bool enabled = pConstraint.IsNotNull();
	pEditBoneParent->SetEnabled(enabled);
	pCBBoneTarget->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditOffset->SetEnabled(enabled);
	
	for(dof=0; dof<6; dof++){
		pEditDofLower[dof]->SetEnabled(enabled);
		pEditDofUpper[dof]->SetEnabled(enabled);
		pEditDofStaFric[dof]->SetEnabled(enabled);
		pEditDofKinFric[dof]->SetEnabled(enabled);
		pEditDofSprStiff[dof]->SetEnabled(enabled);
	}
	
	pEditDampLin->SetEnabled(enabled);
	pEditDampAng->SetEnabled(enabled);
	pEditDampSpr->SetEnabled(enabled);
	
	pChkRope->SetEnabled(enabled);
	
	pEditBreakingThreshold->SetEnabled(enabled);
	
	pChkShowJointError->SetEnabled(enabled);
}
