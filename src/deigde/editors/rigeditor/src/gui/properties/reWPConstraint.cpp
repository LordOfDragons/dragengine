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
	cBaseTextFieldListener(reWPConstraint &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(! rig || ! constraint){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, rig, constraint)));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig *rig, reRigConstraint *constraint) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPConstraint &pPanel;
	
public:
	cBaseEditVectorListener(reWPConstraint &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(! rig || ! constraint){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector->GetVector(), rig, constraint)));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, reRig *rig, reRigConstraint *constraint) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPConstraint &pPanel;
	
public:
	cBaseAction(reWPConstraint &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(! rig || ! constraint){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(rig, constraint)));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(reRig *rig, reRigConstraint *constraint) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPConstraint &pPanel;
	
public:
	cBaseComboBoxListener(reWPConstraint &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		reRig * const rig = pPanel.GetRig();
		reRigConstraint * const constraint = pPanel.GetConstraint();
		if(! rig || ! constraint){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnTextChanged(comboBox, rig, constraint)));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, reRig *rig, reRigConstraint *constraint) = 0;
};



class cComboTarget : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
	cComboTarget(reWPConstraint &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, reRig*, reRigConstraint *constraint){
		if(pPreventUpdate){
			return nullptr;
		}
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *bone = nullptr;
		if(selection){
			bone = (reRigBone*)selection->GetData();
		}
		return bone != constraint->GetConstraintBone()
			? new reUConstraintSetBoneTarget(constraint, bone) : nullptr;
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	cEditPosition(reWPConstraint &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, reRig*, reRigConstraint *constraint){
		return ! vector.IsEqualTo(constraint->GetPosition())
			? new reUConstraintSetPosition(constraint, vector) : nullptr;
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	cEditRotation(reWPConstraint &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, reRig*, reRigConstraint *constraint){
		return ! vector.IsEqualTo(constraint->GetOrientation())
			? new reUConstraintSetOrientation(constraint, vector) : nullptr;
	}
};

class cEditOffset : public cBaseEditVectorListener{
public:
	cEditOffset(reWPConstraint &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, reRig*, reRigConstraint *constraint){
		return ! vector.IsEqualTo(constraint->GetOffset())
			? new reUConstraintSetOffset(constraint, vector) : nullptr;
	}
};

class cTextDofLower : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	cTextDofLower(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const reRigConstraintDof &dof = constraint->GetDof(pDof);
		const float value = textField->GetFloat();
		return fabsf(value - dof.GetLowerLimit()) > dof.GetThresholdLimits()
			? new reUConstraintDofSetLowerLimit(constraint, pDof, value) : nullptr;
	}
};

class cTextDofUpper : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	cTextDofUpper(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const reRigConstraintDof &dof = constraint->GetDof(pDof);
		const float value = textField->GetFloat();
		return fabsf(value - dof.GetUpperLimit()) > dof.GetThresholdLimits()
			? new reUConstraintDofSetUpperLimit(constraint, pDof, value) : nullptr;
	}
};

class cTextDofFrictionStatic : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	cTextDofFrictionStatic(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetDof(pDof).GetStaticFriction()) > FLOAT_SAFE_EPSILON
			? new reUConstraintDofSetStaticFriction(constraint, pDof, value) : nullptr;
	}
};

class cTextDofFrictionKinematic : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	cTextDofFrictionKinematic(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetDof(pDof).GetKinematicFriction()) > FLOAT_SAFE_EPSILON
			? new reUConstraintDofSetKinematicFriction(constraint, pDof, value) : nullptr;
	}
};

class cTextDofSpringStiffness : public cBaseTextFieldListener{
	deColliderConstraint::eDegreesOfFreedom pDof;
public:
	cTextDofSpringStiffness(reWPConstraint &panel, deColliderConstraint::eDegreesOfFreedom dof) :
	cBaseTextFieldListener(panel), pDof(dof){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetDof(pDof).GetSpringStiffness()) > FLOAT_SAFE_EPSILON
			? new reUConstraintDofSetSpringStiffness(constraint, pDof, value) : nullptr;
	}
};

class cTextDampingLinear : public cBaseTextFieldListener{
public:
	cTextDampingLinear(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetLinearDamping()) > FLOAT_SAFE_EPSILON
			? new reUConstraintSetDampingLinear(constraint, value) : nullptr;
	}
};

class cTextDampingAngular : public cBaseTextFieldListener{
public:
	cTextDampingAngular(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetAngularDamping()) > FLOAT_SAFE_EPSILON
			? new reUConstraintSetDampingAngular(constraint, value) : nullptr;
	}
};

class cTextDampingSpring : public cBaseTextFieldListener{
public:
	cTextDampingSpring(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetSpringDamping()) > FLOAT_SAFE_EPSILON
			? new reUConstraintSetDampingSpring(constraint, value) : nullptr;
	}
};

class cCheckRope : public cBaseAction{
public:
	cCheckRope(reWPConstraint &panel) : cBaseAction(panel, "Use rope physics",
		"Determines if this constraint is a rope constraint."){ }
	
	virtual igdeUndo *OnAction(reRig*, reRigConstraint *constraint){
		return new reUConstraintToggleIsRope(constraint);
	}
};

class cTextBreakingThreshold : public cBaseTextFieldListener{
public:
	cTextBreakingThreshold(reWPConstraint &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, reRig*, reRigConstraint *constraint){
		const float value = textField->GetFloat();
		return fabsf(value - constraint->GetBreakingThreshold()) > FLOAT_SAFE_EPSILON
			? new reUConstraintSetBreakingThreshold(constraint, value) : nullptr;
	}
};

class cCheckShowJointError : public cBaseAction{
public:
	cCheckShowJointError(reWPConstraint &panel) : cBaseAction(panel, "Show joint error",
		"Shows joint errors visually during simulation."){}
	
	virtual igdeUndo *OnAction(reRig*, reRigConstraint *constraint){
		constraint->SetShowJointError(! constraint->GetShowJointError());
		return nullptr;
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
pRig(NULL),
pConstraint(NULL),
pListener(NULL),
pPreventUpdate(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new reWPConstraintListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	// structure
	helper.GroupBox(content, groupBox, "Link Partners:");
	
	helper.EditString(groupBox, "Parent:", "Name of parent bone or empty string for entire rig.",
		pEditBoneParent, NULL);
	pEditBoneParent->SetEditable(false);
	
	helper.ComboBox(groupBox, "Target:", true, "Name of target bone or empty string for the world.",
		pCBBoneTarget, new cComboTarget(*this, pPreventUpdate));
	pCBBoneTarget->SetDefaultSorter();
	
	
	// structure
	helper.GroupBox(content, groupBox, "Geometry:");
	
	helper.EditVector(groupBox, "Position:",
		"Position of the constraint relative to the bone local coordinate system.",
		pEditPosition, new cEditPosition(*this));
	helper.EditVector(groupBox, "Rotation:",
		"Rotation of the constraint relative to the bone local coordinate system.",
		pEditRotation, new cEditRotation(*this));
	helper.EditVector(groupBox, "Offset:",
		"Offset of the bone local coordinate system relative to the constraint relative cooridnate system.",
		pEditOffset, new cEditOffset(*this));
	
	
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
			pEditDofLower[dof], new cTextDofLower(*this, vDegreesOfFreedom[dof]));
		helper.EditFloat(frameLine, "Upper limit relative to constraint coordinate system.",
			pEditDofUpper[dof], new cTextDofUpper(*this, vDegreesOfFreedom[dof]));
		
		helper.FormLine(groupBox, "Friction:",
			"Static friction force and kinematic friction factor for joint inner friction.",
			frameLine);
			
		helper.EditFloat(frameLine, "Static friction force in newton per meter.",
			pEditDofStaFric[dof], new cTextDofFrictionStatic(*this, vDegreesOfFreedom[dof]));
		helper.EditFloat(frameLine, "Kinematic friction factor.",
			pEditDofKinFric[dof], new cTextDofFrictionKinematic(*this, vDegreesOfFreedom[dof]));
		
		helper.EditString(groupBox, "Stiffness:", "Spring stiffness in newton per meter.",
			pEditDofSprStiff[dof], new cTextDofSpringStiffness(*this, vDegreesOfFreedom[dof]));
	}
	
	
	// damping
	helper.GroupBox(content, groupBox, "Damping:", true);
	
	helper.EditString(groupBox, "Linear:", "Damping of linear degrees of freedom.",
		pEditDampLin, new cTextDampingLinear(*this));
	helper.EditString(groupBox, "Angular:", "Damping of angular degrees of freedom.",
		pEditDampAng, new cTextDampingAngular(*this));
	helper.EditString(groupBox, "Spring:",
		"Damping of spring degrees of freedom both linear and angular.",
		pEditDampSpr, new cTextDampingSpring(*this));
	
	
	// rope
	helper.GroupBox(content, groupBox, "Rope:", true);
	helper.CheckBox(groupBox, pChkRope, new cCheckRope(*this), true);
	
	
	// breaking
	helper.GroupBox(content, groupBox, "Breaking:", true);
	
	helper.EditString(groupBox, "Breaking:",
		"Breaking threshold impuls in newton seconds or 0 to disable.",
		pEditBreakingThreshold, new cTextBreakingThreshold(*this));
	
	
	// debugging
	helper.GroupBox(content, groupBox, "Debugging:", true);
	helper.CheckBox(groupBox, pChkShowJointError, new cCheckShowJointError(*this), true);
}

reWPConstraint::~reWPConstraint(){
	SetRig(NULL);
	
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void reWPConstraint::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	SetConstraint(NULL);
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig->FreeReference();
		pRig = NULL;
	}
	
	pRig = rig;
	
	if(rig){
		rig->AddNotifier(pListener);
		rig->AddReference();
		
		SetConstraint(rig->GetSelectionConstraints()->GetActiveConstraint());
	}
}

void reWPConstraint::SetConstraint(reRigConstraint *constraint){
	if(constraint == pConstraint){
		return;
	}
	
	if(pConstraint){
		pConstraint->FreeReference();
	}
	
	pConstraint = constraint;
	
	if(constraint){
		constraint->AddReference();
	}
	
	UpdateBoneLists();
	UpdateConstraint();
}

void reWPConstraint::UpdateBoneLists(){
	reRigBone * const selection = pCBBoneTarget->GetSelectedItem()
		? (reRigBone*)pCBBoneTarget->GetSelectedItem()->GetData() : nullptr;
	pPreventUpdate = true;
	
	try{
		pCBBoneTarget->RemoveAllItems();
		pCBBoneTarget->AddItem("< World >", NULL, NULL);
		
		if(pRig && pConstraint){
			const int count = pRig->GetBoneCount();
			int i;
			
			for(i=0; i<count; i++){
				reRigBone * const bone = pRig->GetBoneAt(i);
				
				if(pConstraint->GetRigBone() == bone){
					continue; // not yourself
				}
				
				pCBBoneTarget->AddItem(bone->GetName(), NULL, bone);
			}
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
		pCBBoneTarget->SetSelection(pCBBoneTarget->IndexOfItemWithData(NULL));
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
	const bool enabled = pConstraint != NULL;
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
