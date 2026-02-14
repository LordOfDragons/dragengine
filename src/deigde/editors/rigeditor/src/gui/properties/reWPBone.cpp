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

#include "reWPBone.h"
#include "reWPBoneListener.h"
#include "reWindowProperties.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/bone/reRigBone.h"
#include "../../rig/bone/reSelectionBones.h"
#include "../../undosys/properties/bone/reUSetBoneName.h"
#include "../../undosys/properties/bone/reUSetBoneParent.h"
#include "../../undosys/properties/bone/reUSetBonePosition.h"
#include "../../undosys/properties/bone/reUSetBoneOrientation.h"
#include "../../undosys/properties/bone/reUSetBoneCentralMassPoint.h"
#include "../../undosys/properties/bone/reUSetBoneMass.h"
#include "../../undosys/properties/bone/reUSetBoneIKLimitsLower.h"
#include "../../undosys/properties/bone/reUSetBoneIKLimitsUpper.h"
#include "../../undosys/properties/bone/reUSetBoneIKResistance.h"
#include "../../undosys/properties/bone/reUToggleBoneDynamic.h"
#include "../../undosys/properties/bone/reUToggleBoneIKLocked.h"

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



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	reWPBone &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseTextFieldListener>;
	cBaseTextFieldListener(reWPBone &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if(!rig || !bone){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, rig, bone));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, reRig *rig, reRigBone *bone) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPBone &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseEditVectorListener>;
	cBaseEditVectorListener(reWPBone &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if(!rig || !bone){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), rig, bone));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, reRig *rig, reRigBone *bone) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPBone &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(reWPBone &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	void OnAction() override{
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if(!rig || !bone){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(rig, bone));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(reRig *rig, reRigBone *bone) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPBone &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseComboBoxListener>;
	cBaseComboBoxListener(reWPBone &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if(!rig || !bone){
			return;
		}
		
		igdeUndo::Ref undo(OnTextChanged(comboBox, rig, bone));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, reRig *rig, reRigBone *bone) = 0;
};



class cTextName : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextName>;
	cTextName(reWPBone &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig *rig, reRigBone *bone) override{
		const decString &name = textField->GetText();
		if(name == bone->GetName()){
			return {};
		}
		if(rig->GetBones().HasNamed(name)){
			igdeCommonDialogs::ErrorFormat(pPanel, "@Rig.PanelBone.Dialog.InvalidBoneName",
				pPanel.Translate("Rig.PanelBone.Error.DuplicateName").ToUTF8(), name.GetString());
			return {};
		}
		return reUSetBoneName::Ref::New(bone, name);
	}
};

class cComboParent : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboParent>;
	cComboParent(reWPBone &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, reRig *rig, reRigBone *bone) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *parent = nullptr;
		if(selection){
			parent = (reRigBone*)selection->GetData();
		}
		
		if(parent == bone->GetParentBone()){
			return {};
		}
		
		if(parent && !bone->CanHaveParent(parent)){
			igdeCommonDialogs::ErrorFormat(pPanel, "@Rig.PanelBone.Dialog.InvalidParentBone",
				pPanel.Translate("Rig.PanelBone.Error.InvalidParent").ToUTF8(),
				parent->GetName().GetString());
			return {};
		}

		return reUSetBoneParent::Ref::New(bone, parent);
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditPosition>;
	cEditPosition(reWPBone &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig *rig, reRigBone *bone) override{
		if(vector.IsEqualTo(bone->GetPosition())){
			return {};
		}
		return reUSetBonePosition::Ref::New(bone, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditRotation>;
	cEditRotation(reWPBone &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig *rig, reRigBone *bone) override{
		if(vector.IsEqualTo(bone->GetOrientation())){
			return {};
		}
		return reUSetBoneOrientation::Ref::New(bone, vector);
	}
};

class cEditCentralMassPoint : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditCentralMassPoint>;
	cEditCentralMassPoint(reWPBone &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig *rig, reRigBone *bone) override{
		if(vector.IsEqualTo(bone->GetCentralMassPoint())){
			return {};
		}
		return reUSetBoneCentralMassPoint::Ref::New(bone, vector);
	}
};

class cTextMass : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextMass>;
	cTextMass(reWPBone &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig *rig, reRigBone *bone) override{
		const float mass = textField->GetFloat();
		if(fabsf(mass - bone->GetMass()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return reUSetBoneMass::Ref::New(bone, mass);
	}
};

class cCheckDynamic : public cBaseAction{
public:
	using Ref = deTObjectReference<cCheckDynamic>;
	
public:
	cCheckDynamic(reWPBone &panel) : cBaseAction(panel, "@Rig.PanelBone.Physics.Dynamic",
		"@Rig.PanelBone.Physics.Dynamic.ToolTip"){ }
	
	igdeUndo::Ref OnAction(reRig *rig, reRigBone *bone) override{
		return reUToggleBoneDynamic::Ref::New(bone);
	}
};

class cEditIKLimitsLower : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditIKLimitsLower>;
	cEditIKLimitsLower(reWPBone &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig*, reRigBone *bone) override{
		if(vector.IsEqualTo(bone->GetIKLimitsLower())){
			return {};
		}
		return reUSetBoneIKLimitsLower::Ref::New(bone, vector);
	}
};

class cEditIKLimitsUpper : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditIKLimitsUpper>;
	cEditIKLimitsUpper(reWPBone &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig*, reRigBone *bone) override{
		if(vector.IsEqualTo(bone->GetIKLimitsUpper())){
			return {};
		}
		return reUSetBoneIKLimitsUpper::Ref::New(bone, vector);
	}
};

class cEditIKResistance : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditIKResistance>;
	cEditIKResistance(reWPBone &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig*, reRigBone *bone) override{
		if(vector.IsEqualTo(bone->GetIKResistance())){
			return {};
		}
		return reUSetBoneIKResistance::Ref::New(bone, vector);
	}
};

class cCheckIKLocked : public cBaseAction{
public:
	using Ref = deTObjectReference<cCheckIKLocked>;
	
private:
	const int pAxis;
public:
	cCheckIKLocked(reWPBone &panel, int axis, const char *text) : cBaseAction(panel, text,
		"@Rig.PanelBone.IkLimits.Locked.ToolTip"), pAxis(axis){ }
	
	igdeUndo::Ref OnAction(reRig *rig, reRigBone *bone) override{
		return reUToggleBoneIKLocked::Ref::New(bone, pAxis);
	}
};

}



// Class reWPBone
///////////////////

// Constructor, destructor
////////////////////////////

reWPBone::reWPBone(reWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = reWPBoneListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// structure
	helper.GroupBox(content, groupBox, "@Rig.PanelBone.GroupBox.Structure");
	
	helper.EditString(groupBox, "@Rig.PanelBone.Structure.Name.Label", "@Rig.PanelBone.Structure.Name.ToolTip",
		pEditName, cTextName::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@Rig.PanelBone.Structure.Parent.Label", true,
		"@Rig.PanelBone.Structure.Parent.ToolTip",
		pCBParent, cComboParent::Ref::New(*this));
	pCBParent->SetDefaultSorter();
	
	// geometry
	helper.GroupBox(content, groupBox, "@Rig.PanelBone.GroupBox.Geometry");
	
	helper.EditVector(groupBox, "@Rig.PanelBone.Geometry.Position.Label",
		"@Rig.PanelBone.Geometry.Position.ToolTip",
		pEditPosition, cEditPosition::Ref::New(*this));
	
	helper.EditVector(groupBox, "@Rig.PanelBone.Geometry.Rotation.Label",
		"@Rig.PanelBone.Geometry.Rotation.ToolTip",
		pEditRotation, cEditRotation::Ref::New(*this));
	
	// physics
	helper.GroupBox(content, groupBox, "@Rig.PanelBone.GroupBox.Physics");
	
	helper.EditVector(groupBox, "@Rig.PanelBone.Physics.Cmp.Label",
		"@Rig.PanelBone.Physics.Cmp.ToolTip",
		pEditCentralMassPoint, cEditCentralMassPoint::Ref::New(*this));
	
	helper.EditFloat(groupBox, "@Rig.PanelBone.Physics.Mass.Label", "@Rig.PanelBone.Physics.Mass.ToolTip",
		pEditMass, cTextMass::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkDynamic, cCheckDynamic::Ref::New(*this));
	
	// inverse kinematic limits
	helper.GroupBox(content, groupBox, "@Rig.PanelBone.GroupBox.IkLimits");
	
	helper.EditVector(groupBox, "@Rig.PanelBone.IkLimits.Lower.Label", "@Rig.PanelBone.IkLimits.Lower.ToolTip",
		pEditIKLimitsLower, cEditIKLimitsLower::Ref::New(*this));
	
	helper.EditVector(groupBox, "@Rig.PanelBone.IkLimits.Upper.Label", "@Rig.PanelBone.IkLimits.Upper.ToolTip",
		pEditIKLimitsUpper, cEditIKLimitsUpper::Ref::New(*this));
	
	helper.EditVector(groupBox, "@Rig.PanelBone.IkLimits.Resistance.Label", "@Rig.PanelBone.IkLimits.Resistance.ToolTip",
		pEditIKResistance, cEditIKResistance::Ref::New(*this));
	
	helper.FormLine(groupBox, "@Rig.PanelBone.IkLimits.Locked.Label", "@Rig.PanelBone.IkLimits.Locked.ToolTip", frameLine);
	helper.CheckBox(frameLine, pChkIKLockedX, cCheckIKLocked::Ref::New(*this, 0, "@Rig.PanelBone.IkLimits.X"));
	helper.CheckBox(frameLine, pChkIKLockedY, cCheckIKLocked::Ref::New(*this, 1, "@Rig.PanelBone.IkLimits.Y"));
	helper.CheckBox(frameLine, pChkIKLockedZ, cCheckIKLocked::Ref::New(*this, 2, "@Rig.PanelBone.IkLimits.Z"));
}

reWPBone::~reWPBone(){
	SetRig(nullptr);
}



// Management
///////////////

void reWPBone::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	SetBone(nullptr);
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig = nullptr;
	}
	
	pRig = rig;
	
	if(rig){
		rig->AddNotifier(pListener);
		SetBone(rig->GetSelectionBones()->GetActiveBone());
	}
}

void reWPBone::SetBone(reRigBone *bone){
	if(bone == pBone){
		return;
	}
	pBone = bone;
	UpdateBone();
}



void reWPBone::UpdateParentBoneList(){
	pCBParent->RemoveAllItems();
	pCBParent->AddItem(Translate("Rig.PanelBone.Parent.NoParent").ToUTF8(), nullptr, nullptr);
	
	if(pRig && pBone){
		pRig->GetBones().Visit([&](reRigBone *bone){
			if(pBone->CanHaveParent(bone)){
				pCBParent->AddItem(bone->GetName(), nullptr, bone);
			}
		});
	}
	
	pCBParent->SortItems();
}

void reWPBone::UpdateBone(){
	UpdateParentBoneList();
	
	if(pBone){
		pEditName->SetText(pBone->GetName());
		pCBParent->SetSelection(pCBParent->IndexOfItemWithData(pBone->GetParentBone()));
		pEditPosition->SetVector(pBone->GetPosition());
		pEditRotation->SetVector(pBone->GetOrientation());
		pEditCentralMassPoint->SetVector(pBone->GetCentralMassPoint());
		pEditIKLimitsLower->SetVector(pBone->GetIKLimitsLower());
		pEditIKLimitsUpper->SetVector(pBone->GetIKLimitsUpper());
		pEditIKResistance->SetVector(pBone->GetIKResistance());
		pEditMass->SetFloat(pBone->GetMass());
		pChkDynamic->SetChecked(pBone->GetDynamic());
		pChkIKLockedX->SetChecked(pBone->GetIKLockedX());
		pChkIKLockedY->SetChecked(pBone->GetIKLockedY());
		pChkIKLockedZ->SetChecked(pBone->GetIKLockedZ());
		
	}else{
		pEditName->ClearText();
		pCBParent->SetSelection(pCBParent->IndexOfItemWithData(nullptr));
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditCentralMassPoint->SetVector(decVector());
		pEditIKLimitsLower->SetVector(decVector());
		pEditIKLimitsUpper->SetVector(decVector());
		pEditIKResistance->SetVector(decVector());
		pEditMass->ClearText();
		pChkDynamic->SetChecked(false);
		pChkIKLockedX->SetChecked(false);
		pChkIKLockedY->SetChecked(false);
		pChkIKLockedZ->SetChecked(false);
	}
	
	const bool enabled = pBone.IsNotNull();
	pEditName->SetEnabled(enabled);
	pCBParent->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditCentralMassPoint->SetEnabled(enabled);
	pEditIKLimitsLower->SetEnabled(enabled);
	pEditIKLimitsUpper->SetEnabled(enabled);
	pEditIKResistance->SetEnabled(enabled);
	pEditMass->SetEnabled(enabled);
	pChkDynamic->SetEnabled(enabled);
	pChkIKLockedX->SetEnabled(enabled);
	pChkIKLockedY->SetEnabled(enabled);
	pChkIKLockedZ->SetEnabled(enabled);
}
