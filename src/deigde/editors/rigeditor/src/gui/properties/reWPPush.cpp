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

#include "reWPPush.h"
#include "reWindowProperties.h"
#include "reWPPushListener.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/push/reRigPush.h"
#include "../../rig/push/reSelectionPushes.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
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
	reWPPush &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseTextFieldListener>;
	cBaseTextFieldListener(reWPPush &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		reRig * const rig = pPanel.GetRig();
		reRigPush * const push = pPanel.GetPush();
		if(rig && push){
			OnChanged(textField, rig, push);
		}
	}
	
	virtual void OnChanged(igdeTextField *textField, reRig *rig, reRigPush *push) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPPush &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseEditVectorListener>;
	cBaseEditVectorListener(reWPPush &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		reRig * const rig = pPanel.GetRig();
		reRigPush * const push = pPanel.GetPush();
		if(rig && push){
			OnChanged(editVector->GetVector(), rig, push);
		}
	}
	
	virtual void OnChanged(const decVector &vector, reRig *rig, reRigPush *push) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPPush &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseComboBoxListener>;
	cBaseComboBoxListener(reWPPush &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		reRig * const rig = pPanel.GetRig();
		reRigPush * const push = pPanel.GetPush();
		if(rig && push){
			OnTextChanged(comboBox, rig, push);
		}
	}
	
	virtual void OnTextChanged(igdeComboBox *comboBox, reRig *rig, reRigPush *push) = 0;
};



class cComboType : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboType>;
	cComboType(reWPPush &panel) : cBaseComboBoxListener(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox, reRig *rig, reRigPush *push) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(selection){
			push->SetType((reRigPush::ePushTypes)(intptr_t)selection->GetData());
		}
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditPosition>;
	cEditPosition(reWPPush &panel) : cBaseEditVectorListener(panel){}
	
	void OnChanged(const decVector &vector, reRig *rig, reRigPush *push) override{
		push->SetPosition(vector);
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	using Ref = deTObjectReference<cEditRotation>;
	cEditRotation(reWPPush &panel) : cBaseEditVectorListener(panel){}
	
	void OnChanged(const decVector &vector, reRig *rig, reRigPush *push) override{
		push->SetOrientation(vector);
	}
};

class cTextImpuls : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextImpuls>;
	cTextImpuls(reWPPush &panel) : cBaseTextFieldListener(panel){}
	
	void OnChanged(igdeTextField *textField, reRig *rig, reRigPush *push) override{
		push->SetImpuls(textField->GetFloat());
	}
};

class cTextRayCount : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextRayCount>;
	cTextRayCount(reWPPush &panel) : cBaseTextFieldListener(panel){}
	
	void OnChanged(igdeTextField *textField, reRig *rig, reRigPush *push) override{
		push->SetRayCount(textField->GetInteger());
	}
};

class cTextConeAngle : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextConeAngle>;
	cTextConeAngle(reWPPush &panel) : cBaseTextFieldListener(panel){}
	
	void OnChanged(igdeTextField *textField, reRig *rig, reRigPush *push) override{
		push->SetConeAngle(textField->GetFloat());
	}
};

}



// Class reWPPush
////////////////////

// Constructor, destructor
////////////////////////////

reWPPush::reWPPush(reWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = reWPPushListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// structure
	helper.GroupBox(content, groupBox, "@Rig.PanelPush.GroupBox.Geometry");
	
	helper.ComboBox(groupBox, "@Rig.PanelPush.Geometry.Type", "@Rig.PanelPush.Geometry.Type.ToolTip", pCBType, cComboType::Ref::New(*this));
	pCBType->SetAutoTranslateItems(true);
	pCBType->AddItem("@Rig.PanelPush.Type.SimplePush", nullptr, (void*)(intptr_t)reRigPush::eptSimple);
	pCBType->AddItem("@Rig.PanelPush.Type.Explosion", nullptr, (void*)(intptr_t)reRigPush::eptExplosion);
	
	helper.EditVector(groupBox, "@Rig.PanelPush.Geometry.Position", "@Rig.PanelPush.Geometry.Position.ToolTip",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "@Rig.PanelPush.Geometry.Rotation", "@Rig.PanelPush.Geometry.Rotation.ToolTip",
		pEditRotation, cEditRotation::Ref::New(*this));
	
	helper.EditFloat(groupBox, "@Rig.PanelPush.Geometry.Impuls", "@Rig.PanelPush.Geometry.Impuls.ToolTip",
		pEditImpuls, cTextImpuls::Ref::New(*this));
	helper.EditInteger(groupBox, "@Rig.PanelPush.Geometry.RayCount", "@Rig.PanelPush.Geometry.RayCount.ToolTip",
		pEditRayCount, cTextRayCount::Ref::New(*this));
	helper.EditFloat(groupBox, "@Rig.PanelPush.Geometry.Cone", "@Rig.PanelPush.Geometry.Cone.ToolTip",
		pEditConeAngle, cTextConeAngle::Ref::New(*this));
}

reWPPush::~reWPPush(){
	SetRig(nullptr);
}



// Management
///////////////

void reWPPush::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	SetPush(nullptr);
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig = nullptr;
	}
	
	pRig = rig;
	
	if(rig){
		rig->AddNotifier(pListener);
		SetPush(rig->GetSelectionPushes()->GetActivePush());
	}
}

void reWPPush::SetPush(reRigPush *push){
	if(push == pPush){
		return;
	}
	pPush = push;
	UpdatePush();
}

void reWPPush::UpdatePush(){
	if(pPush){
		pCBType->SetSelection(pCBType->IndexOfItemWithData((void*)(intptr_t)pPush->GetType()));
		pEditPosition->SetVector(pPush->GetPosition());
		pEditRotation->SetVector(pPush->GetOrientation());
		pEditImpuls->SetFloat(pPush->GetImpuls());
		pEditRayCount->SetInteger(pPush->GetRayCount());
		pEditConeAngle->SetFloat(pPush->GetConeAngle());
		
	}else{
		pCBType->SetSelection(pCBType->IndexOfItemWithData((void*)(intptr_t)reRigPush::eptSimple));
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditImpuls->ClearText();
		pEditRayCount->ClearText();
		pEditConeAngle->ClearText();
	}
	
	const bool enabled = pPush.IsNotNull();
	pCBType->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditImpuls->SetEnabled(enabled);
	pEditRayCount->SetEnabled(enabled);
	pEditConeAngle->SetEnabled(enabled);
}
