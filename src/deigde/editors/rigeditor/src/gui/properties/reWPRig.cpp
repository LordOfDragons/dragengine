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

#include "reWPRig.h"
#include "reWPRigListener.h"
#include "reWindowProperties.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/bone/reRigBone.h"
#include "../../undosys/properties/rig/reURigSetRootBone.h"
#include "../../undosys/properties/rig/reURigToggleDynamic.h"
#include "../../undosys/properties/rig/reURigToggleModelCollision.h"
#include "../../undosys/properties/rig/reURigSetCentralMassPoint.h"

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
	reWPRig &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(reWPRig &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		reRig * const rig = pPanel.GetRig();
		if(!rig){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, rig));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, reRig *rig) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPRig &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(reWPRig &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		reRig * const rig = pPanel.GetRig();
		if(!rig){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), rig));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, reRig *rig) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPRig &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(reWPRig &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	void OnAction() override{
		reRig * const rig = pPanel.GetRig();
		if(!rig){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(rig));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(reRig *rig) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPRig &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(reWPRig &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		reRig * const rig = pPanel.GetRig();
		if(!rig){
			return;
		}
		
		igdeUndo::Ref undo(OnTextChanged(comboBox, rig));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, reRig *rig) = 0;
};



class cComboRootBone : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboRootBone> Ref;
	cComboRootBone(reWPRig &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, reRig *rig){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *bone = nullptr;
		if(selection){
			bone = (reRigBone*)selection->GetData();
		}
		if(bone == rig->GetRootBone()){
			return {};
		}
		return new reURigSetRootBone(rig, bone);
	}
};

class cCheckDynamic : public cBaseAction{
public:
	typedef deTObjectReference<cCheckDynamic> Ref;
	cCheckDynamic(reWPRig &panel) :
	cBaseAction(panel, "Dynamic", "Determines if the rig is afflicted by dynamic physics."){ }
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		return reURigToggleDynamic::Ref::New(rig);
	}
};

class cCheckModelCollision : public cBaseAction{
public:
	typedef deTObjectReference<cCheckModelCollision> Ref;
	cCheckModelCollision(reWPRig &panel) :
	cBaseAction(panel, "Model Collision", "Use model collision instead of shapes."){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		return reURigToggleModelCollision::Ref::New(rig);
	}
};

class cEditCentralMassPoint : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditCentralMassPoint> Ref;
	cEditCentralMassPoint(reWPRig &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, reRig *rig) override{
		if(vector.IsEqualTo(rig->GetCentralMassPoint())){
			return {};
		}
		return reURigSetCentralMassPoint::Ref::New(rig, vector);
	}
};

class cEditMass : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditMass> Ref;
	cEditMass(reWPRig &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, reRig *rig) override{
		rig->SetMass(textField->GetFloat());
		return {};
	}
};

}



// Class reWPRig
//////////////////

// Constructor, destructor
////////////////////////////

reWPRig::reWPRig(reWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = reWPRigListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBox(content, groupBox, "Rig:");
	
	helper.ComboBox(groupBox, "Root Bone:", true, "Name of the root bone.",
		pCBRootBone, cComboRootBone::Ref::New(*this));
	pCBRootBone->SetDefaultSorter();
	
	helper.EditVector(groupBox, "CMP:",
		"Central mass point relative to the collider origin.",
		pEditCentralMassPoint, cEditCentralMassPoint::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkModelCollision, cCheckModelCollision::Ref::New(*this));
	
	
	helper.GroupBox(content, groupBox, "Simulation:");
	helper.CheckBox(groupBox, pChkDynamic, cCheckDynamic::Ref::New(*this));
	helper.EditFloat(groupBox, "Mass:", "Mass in kg.", pEditMass, cEditMass::Ref::New(*this));
}

reWPRig::~reWPRig(){
	SetRig(nullptr);
}



// Management
///////////////

void reWPRig::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig = nullptr;
	}
	
	pRig = rig;
	
	if(rig){
		rig->AddNotifier(pListener);
	}
	
	UpdateRig();
}



void reWPRig::UpdateRootBoneList(){
	pCBRootBone->RemoveAllItems();
	
	if(pRig){
		pRig->GetBones().Visit([&](reRigBone *bone){
			if(!bone->GetParentBone()){
				pCBRootBone->AddItem(bone->GetName(), nullptr, bone);
			}
		});
	}
	
	pCBRootBone->SortItems();
}

void reWPRig::UpdateRig(){
	UpdateRootBoneList();
	
	if(pRig){
		pCBRootBone->SetSelection(pCBRootBone->IndexOfItemWithData(pRig->GetRootBone()));
		pEditCentralMassPoint->SetVector(pRig->GetCentralMassPoint());
		pChkModelCollision->SetChecked(pRig->GetModelCollision());
		pChkDynamic->SetChecked(pRig->GetDynamic());
		pEditMass->SetFloat(pRig->GetMass());
		
	}else{
		pCBRootBone->SetSelection(-1);
		pEditCentralMassPoint->SetVector(decVector());
		pChkModelCollision->SetChecked(false);
		pChkDynamic->SetChecked(false);
		pEditMass->ClearText();
	}
	
	const bool enabled = pRig.IsNotNull();
	pCBRootBone->SetEnabled(enabled);
	pEditCentralMassPoint->SetEnabled(enabled);
	pChkModelCollision->SetEnabled(enabled);
	pEditMass->SetEnabled(enabled);
}
