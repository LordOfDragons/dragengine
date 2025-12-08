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

#include "aeWPController.h"
#include "aeWPControllerListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/locomotion/aeAnimatorLocomotion.h"
#include "../../animator/controller/aeController.h"
#include "../../clipboard/aeClipboardDataController.h"
#include "../../undosys/controller/aeUAddController.h"
#include "../../undosys/controller/aeURemoveController.h"
#include "../../undosys/controller/aeUMoveControllerUp.h"
#include "../../undosys/controller/aeUMoveControllerDown.h"
#include "../../undosys/controller/aeUControllerSetName.h"
#include "../../undosys/controller/aeUControllerSetMinimumValue.h"
#include "../../undosys/controller/aeUControllerSetMaximumValue.h"
#include "../../undosys/controller/aeUControllerSetFromMove.h"
#include "../../undosys/controller/aeUControllerToggleClamp.h"
#include "../../undosys/controller/aeUControllerToggleFrozen.h"
#include "../../undosys/controller/aeUControllerPaste.h"
#include "../../undosys/controller/aeUControllerSetDefaultValue.h"
#include "../../undosys/controller/aeUControllerSetDefaultVector.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPController &pPanel;
	
public:
	cBaseAction(aeWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeController * const controller = pPanel.GetController();
		if(!animator || !controller){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(animator, controller)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeController *controller) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeController * const controller = pPanel.GetController();
		if(animator && controller){
			Update(*animator, *controller);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeController &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseTextField : public igdeTextFieldListener{
protected:
	aeWPController &pPanel;
	
public:
	cBaseTextField(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeController * const controller = pPanel.GetController();
		if(!animator || !controller){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, animator, controller)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator *animator, aeController *controller) = 0;
};



class cActionCopy : public cBaseAction{
public:
	cActionCopy(aeWPController &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy controller to clipboard"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeController *controller){
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataController::Ref::NewWith(controller));
		return nullptr;
	}
};

class cActionCut : public cBaseAction{
public:
	cActionCut(aeWPController &panel) : cBaseAction(panel, "Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut controller into clipboard"){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeController *controller){
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataController::Ref::NewWith(controller));
		return new aeURemoveController(animator, controller);
	}
};

class cActionPaste : public igdeAction{
	aeWPController &pPanel;
	
public:
	cActionPaste(aeWPController &panel) : igdeAction("Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste controller from clipboard"), pPanel(panel){}
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(!animator){
			return;
		}
		
		aeClipboardDataController * const cdata = (aeClipboardDataController*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataController::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		animator->GetUndoSystem()->Add(
			aeUControllerPaste::Ref::NewWith(animator, cdata->GetControllers()));
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.GetWithTypeName(aeClipboardDataController::TYPE_NAME) && pPanel.GetAnimator());
	}
};

class cListControllers : public igdeListBoxListener{
	aeWPController &pPanel;
	
public:
	cListControllers(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		if(pPanel.GetAnimator()){
			pPanel.GetAnimator()->SetActiveController(listBox->GetSelectedItem()
				? (aeController*)listBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		const aeWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, windowMain.GetActionControllerAdd());
		helper.MenuCommand(menu, windowMain.GetActionControllerDuplicate());
		helper.MenuCommand(menu, windowMain.GetActionControllerRemove());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, windowMain.GetActionControllerUp());
		helper.MenuCommand(menu, windowMain.GetActionControllerDown());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, new cActionCopy(pPanel), true);
		helper.MenuCommand(menu, new cActionCut(pPanel), true);
		helper.MenuCommand(menu, new cActionPaste(pPanel), true);
	}
};

class cTextName : public cBaseTextField{
public:
	cTextName(aeWPController &panel) : cBaseTextField(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator *animator, aeController *controller){
		const decString value = textField->GetText();
		if(controller->GetName() == value){
			return nullptr;
		}
		if(animator->GetControllers().HasNamed(value)){
			igdeCommonDialogs::Error(&pPanel, "Set Controller Name", "Duplicate Controller Name");
			textField->SetText(controller->GetName());
			return nullptr;
		}
		return new aeUControllerSetName(controller, value);
	}
};

class cTextMinimumValue : public cBaseTextField{
public:
	cTextMinimumValue(aeWPController &panel) : cBaseTextField(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller){
		const float value = textField->GetFloat();
		return fabsf(controller->GetMinimumValue() - value) > FLOAT_SAFE_EPSILON
			? new aeUControllerSetMinimumValue(controller, value) : nullptr;
	}
};

class cTextMaximumValue : public cBaseTextField{
public:
	cTextMaximumValue(aeWPController &panel) : cBaseTextField(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller){
		const float value = textField->GetFloat();
		return fabsf(controller->GetMaximumValue() - value) > FLOAT_SAFE_EPSILON
			? new aeUControllerSetMaximumValue(controller, value) : nullptr;
	}
};

class cActionSetFromMove : public cBaseAction{
public:
	cActionSetFromMove(aeWPController &panel) : cBaseAction(panel, "Set From Move",
		nullptr, "Sets the ranges from the playtime of a move"){}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeController *controller){
		const deAnimation * const animation = animator->GetEngineAnimator()
			? animator->GetEngineAnimator()->GetAnimation() : nullptr;
		decStringList names;
		int selection = 0;
		
		if(animation){
			const int moveCount = animation->GetMoveCount();
			int i;
			for(i=0; i<moveCount; i++){
				names.Add(animation->GetMove(i)->GetName());
			}
		}
		
		names.SortAscending();
		if(names.GetCount() == 0 || !igdeCommonDialogs::SelectString(&pPanel, "Set range from move playtime",
		"Range limits are set to the playtime of the selected move.", names, selection)){
			return nullptr;
		}
		
		const deAnimationMove &move = *animation->GetMove(animation->FindMove(names.GetAt(selection)));
		return new aeUControllerSetFromMove(controller, 0.0f, move.GetPlaytime());
	}
};

class cActionResetValue : public cBaseAction{
public:
	cActionResetValue(aeWPController &panel) : cBaseAction(panel, "Reset",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongLeft),
		"Reset value to controller minimum"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeController *controller){
		controller->SetCurrentValue(controller->GetMinimumValue());
		return nullptr;
	}
};

class cSlideValue : public igdeEditSliderTextListener{
	aeWPController &pPanel;
	
public:
	cSlideValue(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		aeController * const controller = pPanel.GetController();
		if(controller){
			controller->SetCurrentValue(sliderText->GetValue());
		}
	}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		OnSliderTextValueChanged(sliderText);
	}
};

class cEditVector : public igdeEditVectorListener{
	aeWPController &pPanel;
	
public:
	cEditVector(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		aeController * const controller = pPanel.GetController();
		if(controller){
			controller->SetVector(editVector->GetVector());
		}
	}
};

class cActionClamp : public cBaseAction{
public:
	cActionClamp(aeWPController &panel) : cBaseAction(panel, "Clamp value to range",
		nullptr, "Determines if the value of the controller is clamped to the given range"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeController *controller){
		return new aeUControllerToggleClamp(controller);
	}
	
	void Update(const aeAnimator &, const aeController &controller) override{
		SetEnabled(true);
		SetSelected(controller.GetClamp());
	}
};

class cActionFrozen : public cBaseAction{
public:
	cActionFrozen(aeWPController &panel) : cBaseAction(panel, "Freeze Controller value",
		nullptr, "Prevents the controller from changing the current value"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeController *controller){
		return new aeUControllerToggleFrozen(controller);
	}
	
	void Update(const aeAnimator &, const aeController &controller) override{
		SetEnabled(true);
		SetSelected(controller.GetFrozen());
	}
};

class cComboLocoAttr : public igdeComboBoxListener{
	aeWPController &pPanel;
	
public:
	cComboLocoAttr(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeController * const controller = pPanel.GetController();
		if(controller && comboBox->GetSelectedItem()){
			controller->SetLocomotionAttribute((aeAnimatorLocomotion::eLocomotionTypes)
				(intptr_t)comboBox->GetSelectedItem()->GetData());
		}
	}
};

class cTextLocoLeg : public cBaseTextField{
public:
	cTextLocoLeg(aeWPController &panel) : cBaseTextField(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator *animator, aeController *controller){
		const int value = textField->GetInteger();
		if(value >= 0 && value < animator->GetLocomotion().GetLegCount()){
			controller->SetLocomotionLeg(value);
		}
		return nullptr;
	}
};

class cComboVectorSimulation : public igdeComboBoxListener{
	aeWPController &pPanel;
	
public:
	cComboVectorSimulation(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeController * const controller = pPanel.GetController();
		if(controller && comboBox->GetSelectedItem()){
			controller->SetVectorSimulation((aeController::eVectorSimulation)
				(intptr_t)comboBox->GetSelectedItem()->GetData());
		}
	}
};

class cTextDefaultValue : public cBaseTextField{
public:
	cTextDefaultValue(aeWPController &panel) : cBaseTextField(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller){
		const float value = textField->GetFloat();
		return fabsf(controller->GetDefaultValue() - value) > FLOAT_SAFE_EPSILON
			? new aeUControllerSetDefaultValue(controller, value) : nullptr;
	}
};

class cEditDefaultVector : public igdeEditVectorListener{
	aeWPController &pPanel;
	
public:
	cEditDefaultVector(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeController * const controller = pPanel.GetController();
		if(!animator || !controller){
			return;
		}
		
		const decVector &vector = editVector->GetVector();
		if(!controller->GetDefaultVector().IsEqualTo(vector)){
			animator->GetUndoSystem()->Add(
				aeUControllerSetDefaultVector::Ref::NewWith(controller, vector));
		}
	}
};

}



// Class aeWPController
/////////////////////////

// Constructor, destructor
////////////////////////////

aeWPController::aeWPController(aeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(nullptr),
pAnimator(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener.TakeOverWith(*this);
	
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	// controllers
	helper.GroupBoxFlow(content, groupBox, "Controllers:");
	helper.ListBox(groupBox, 8, "Controllers", pListController, new cListControllers(*this));
	
	
	// controller settings
	helper.GroupBox(content, groupBox, "Controller Settings:");
	helper.EditString(groupBox, "Name:", "Name of the controller", pEditName, new cTextName(*this));
	helper.EditFloat(groupBox, "Minimum Value:", "Minimum controller value",
		pEditMin, new cTextMinimumValue(*this));
	helper.EditFloat(groupBox, "Maximum Value:", "Maximum controller value",
		pEditMax, new cTextMaximumValue(*this));
	helper.EditSliderText(groupBox, "Value:", "Current controller value",
		0.0f, 1.0f, 6, 3, 0.1f, pSldValue, new cSlideValue(*this));
	helper.EditVector(groupBox, "Vector:", "Vector value of controller",
		pEditVector, new cEditVector(*this));
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnSetFromMove, new cActionSetFromMove(*this), true);
	helper.Button(formLine, pBtnResetValue, new cActionResetValue(*this), true);
	
	helper.CheckBox(groupBox, pChkClamp, new cActionClamp(*this), true);
	helper.CheckBox(groupBox, pChkFrozen, new cActionFrozen(*this), true);
	
	helper.EditFloat(groupBox, "Default Value:", "Default controller value",
		pEditDefaultValue, new cTextDefaultValue(*this));
	helper.EditVector(groupBox, "Default Vector:", "Default vector value of controller",
		pEditDefaultVector, new cEditDefaultVector(*this));
	
	
	// locomotion testing
	helper.GroupBox(content, groupBox, "Locomotion Testing:");
	
	helper.ComboBox(groupBox, "Attribute:", "Selects the locomotion attribute affecting this controller",
		pCBLocoAttr, new cComboLocoAttr(*this));
	pCBLocoAttr->AddItem("None", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaNone);
	pCBLocoAttr->AddItem("Elapsed Time", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaElapsedTime);
	pCBLocoAttr->AddItem("Look Up-Down", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLookUpDown);
	pCBLocoAttr->AddItem("Look Left-Right", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLookLeftRight);
	pCBLocoAttr->AddItem("Moving Speed", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaMovingSpeed);
	pCBLocoAttr->AddItem("Moving Direction", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaMovingDirection);
	pCBLocoAttr->AddItem("Relative Moving Speed", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaRelativeMovingSpeed);
	pCBLocoAttr->AddItem("Turning Speed", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaTurningSpeed);
	pCBLocoAttr->AddItem("Stance", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaStance);
	pCBLocoAttr->AddItem("Displacement", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaDisplacement);
	pCBLocoAttr->AddItem("Relative Displacement", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaRelativeDisplacement);
	pCBLocoAttr->AddItem("Body Tilt Offset", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaBodyTiltOffset);
	pCBLocoAttr->AddItem("Body Tilt Up-Down", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaBodyTiltUpDown);
	pCBLocoAttr->AddItem("Body Tilt Left-Right", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaBodyTiltRightLeft);
	pCBLocoAttr->AddItem("Turn In-Place", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaTimeTurnIP);
	pCBLocoAttr->AddItem("Leg Ground Position", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegGroundPosition);
	pCBLocoAttr->AddItem("Leg Ground Normal", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegGroundNormal);
	pCBLocoAttr->AddItem("Leg Influence", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegInfluence);
	pCBLocoAttr->AddItem("Leg Position", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegPosition);
	pCBLocoAttr->AddItem("Leg Orientation", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegOrientation);
	
	helper.EditInteger(groupBox, "Leg:", "Number of the leg to track starting with 0 (max 3)",
		pEditLocoLeg, new cTextLocoLeg(*this));
	
	helper.ComboBox(groupBox, "Vector Simulation:", "Select how to simulate vector value",
		pCBVectorSimulation, new cComboVectorSimulation(*this));
	pCBVectorSimulation->AddItem("None", nullptr, (void*)(intptr_t)aeController::evsNone);
	pCBVectorSimulation->AddItem("Position", nullptr, (void*)(intptr_t)aeController::evsPosition);
	pCBVectorSimulation->AddItem("Rotation", nullptr, (void*)(intptr_t)aeController::evsRotation);
}

aeWPController::~aeWPController(){
	SetAnimator(nullptr);
	
}



// Management
///////////////

void aeWPController::SetAnimator(aeAnimator *animator){
	if(pAnimator == animator){
		return;
	}
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddNotifier(pListener);
		animator->AddReference();
	}
	
	UpdateControllerList();
}

aeController *aeWPController::GetController() const{
	return pAnimator ? pAnimator->GetActiveController() : nullptr;
}

void aeWPController::SelectActiveController(){
	pListController->SetSelectionWithData(GetController());
}

void aeWPController::UpdateControllerList(){
	aeController * const selection = GetController();
	
	pListController->RemoveAllItems();
	
	if(pAnimator){
		const aeControllerList &list = pAnimator->GetControllers();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			aeController * const controller = list.GetAt(i);
			text.Format("%d: %s", i, controller->GetName().GetString());
			pListController->AddItem(text, nullptr, controller);
		}
	}
	
	pListController->SetSelectionWithData(selection);
	if(!pListController->GetSelectedItem() && pListController->GetItemCount() > 0){
		pListController->SetSelection(0);
	}
	
	UpdateController();
}

void aeWPController::UpdateController(){
	const aeController * const controller = GetController();
	
	if(controller){
		pEditName->SetText(controller->GetName());
		pEditMin->SetFloat(controller->GetMinimumValue());
		pEditMax->SetFloat(controller->GetMaximumValue());
		
		const float minimumValue = controller->GetMinimumValue();
		const float maximumValue = controller->GetMaximumValue();
		pSldValue->SetRange(minimumValue, maximumValue);
		pSldValue->SetTickSpacing((maximumValue - minimumValue) * 0.1f);
		pSldValue->SetValue(controller->GetCurrentValue());
		
		pEditVector->SetVector(controller->GetVector());
		pCBLocoAttr->SetSelectionWithData((void*)(intptr_t)controller->GetLocomotionAttribute());
		pEditLocoLeg->SetInteger(controller->GetLocomotionLeg());
		pCBVectorSimulation->SetSelectionWithData((void*)(intptr_t)controller->GetVectorSimulation());
		
		pEditDefaultValue->SetFloat(controller->GetDefaultValue());
		pEditDefaultVector->SetVector(controller->GetDefaultVector());
		
	}else{
		pEditName->ClearText();
		pEditMin->ClearText();
		pEditMax->ClearText();
		pSldValue->SetRange(0.0f, 1.0f);
		pSldValue->SetTickSpacing(0.1f);
		pSldValue->SetValue(0.0f);
		pEditVector->SetVector(decVector());
		pCBLocoAttr->SetSelectionWithData((void*)(intptr_t)aeAnimatorLocomotion::eaNone);
		pEditLocoLeg->ClearText();
		pCBVectorSimulation->SetSelectionWithData((void*)(intptr_t)aeController::evsNone);
		pEditDefaultValue->ClearText();
		pEditDefaultVector->SetVector(decVector());
	}
	
	const bool enabled = controller;
	pEditName->SetEnabled(enabled);
	pCBLocoAttr->SetEnabled(enabled);
	pEditLocoLeg->SetEnabled(enabled);
	pCBVectorSimulation->SetEnabled(enabled);
	
	pChkClamp->GetAction()->Update();
	pChkFrozen->GetAction()->Update();
	
	pEditMin->SetEnabled(enabled);
	pEditMax->SetEnabled(enabled);
	pSldValue->SetEnabled(enabled);
	pEditVector->SetEnabled(enabled);
	
	pEditDefaultValue->SetEnabled(enabled);
	pEditDefaultVector->SetEnabled(enabled);
}

void aeWPController::UpdateControllerValue(){
	const aeController * const controller = GetController();
	if(controller){
		pSldValue->SetValue(controller->GetCurrentValue());
		pEditVector->SetVector(controller->GetVector());
	}
}
