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
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeController * const controller = pPanel.GetController();
		if(!animator || !controller){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, controller));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeController *controller) = 0;
	
	void Update() override{
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
	typedef deTObjectReference<cBaseTextField> Ref;
	cBaseTextField(aeWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeController * const controller = pPanel.GetController();
		if(!animator || !controller){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, controller));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeController *controller) = 0;
};



class cActionCopy : public cBaseAction{
public:
	typedef deTObjectReference<cActionCopy> Ref;
	
public:
	cActionCopy(aeWPController &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy controller to clipboard"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeController *controller) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataController::Ref::New(controller));
		return {};
	}
};

class cActionCut : public cBaseAction{
public:
	typedef deTObjectReference<cActionCut> Ref;
	
public:
	cActionCut(aeWPController &panel) : cBaseAction(panel, "Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut controller into clipboard"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeController *controller) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataController::Ref::New(controller));
		return aeURemoveController::Ref::New(animator, controller);
	}
};

class cActionPaste : public igdeAction{
public:
	typedef deTObjectReference<cActionPaste> Ref;
	
private:
	aeWPController &pPanel;
	
public:
	cActionPaste(aeWPController &panel) : igdeAction("Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste controller from clipboard"), pPanel(panel){}
	
	void OnAction() override{
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
			aeUControllerPaste::Ref::New(animator, cdata->GetControllers()));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.GetWithTypeName(aeClipboardDataController::TYPE_NAME) && pPanel.GetAnimator());
	}
};

class cListControllers : public igdeListBoxListener{
	aeWPController &pPanel;
	
public:
	typedef deTObjectReference<cListControllers> Ref;
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
		helper.MenuCommand(menu, cActionCopy::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionCut::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPaste::Ref::New(pPanel));
	}
};

class cTextName : public cBaseTextField{
public:
	typedef deTObjectReference<cTextName> Ref;
	
	cTextName(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeController *controller) override{
		const decString value = textField->GetText();
		if(controller->GetName() == value){
			return {};
		}
		if(animator->GetControllers().HasMatching([&](aeController *other){
			return other->GetName() == value;
		})){
			igdeCommonDialogs::Error(&pPanel, "Set Controller Name", "Duplicate Controller Name");
			textField->SetText(controller->GetName());
			return {};
		}
		return aeUControllerSetName::Ref::New(controller, value);
	}
};

class cTextMinimumValue : public cBaseTextField{
public:
	typedef deTObjectReference<cTextMinimumValue> Ref;
	cTextMinimumValue(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller) override{
		const float value = textField->GetFloat();
		return fabsf(controller->GetMinimumValue() - value) > FLOAT_SAFE_EPSILON
			? aeUControllerSetMinimumValue::Ref::New(controller, value) : igdeUndo::Ref();
	}
};

class cTextMaximumValue : public cBaseTextField{
public:
	typedef deTObjectReference<cTextMaximumValue> Ref;
	cTextMaximumValue(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller) override{
		const float value = textField->GetFloat();
		return fabsf(controller->GetMaximumValue() - value) > FLOAT_SAFE_EPSILON
			? aeUControllerSetMaximumValue::Ref::New(controller, value) : igdeUndo::Ref();
	}
};

class cActionSetFromMove : public cBaseAction{
public:
	typedef deTObjectReference<cActionSetFromMove> Ref;
	
public:
	cActionSetFromMove(aeWPController &panel) : cBaseAction(panel, "Set From Move",
		nullptr, "Sets the ranges from the playtime of a move"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeController *controller) override{
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
			return {};
		}
		
		const deAnimationMove &move = *animation->GetMove(animation->FindMove(names.GetAt(selection)));
		return aeUControllerSetFromMove::Ref::New(controller, 0.0f, move.GetPlaytime());
	}
};

class cActionResetValue : public cBaseAction{
public:
	typedef deTObjectReference<cActionResetValue> Ref;
	
public:
	cActionResetValue(aeWPController &panel) : cBaseAction(panel, "Reset",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongLeft),
		"Reset value to controller minimum"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeController *controller) override{
		controller->SetCurrentValue(controller->GetMinimumValue());
		return {};
	}
};

class cSlideValue : public igdeEditSliderTextListener{
	aeWPController &pPanel;
	
public:
	typedef deTObjectReference<cSlideValue> Ref;
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
	typedef deTObjectReference<cEditVector> Ref;
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
	typedef deTObjectReference<cActionClamp> Ref;
	
public:
	cActionClamp(aeWPController &panel) : cBaseAction(panel, "Clamp value to range",
		nullptr, "Determines if the value of the controller is clamped to the given range"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeController *controller) override{
		return aeUControllerToggleClamp::Ref::New(controller);
	}
	
	void Update(const aeAnimator &, const aeController &controller) override{
		SetEnabled(true);
		SetSelected(controller.GetClamp());
	}
};

class cActionFrozen : public cBaseAction{
public:
	typedef deTObjectReference<cActionFrozen> Ref;
	
public:
	cActionFrozen(aeWPController &panel) : cBaseAction(panel, "Freeze Controller value",
		nullptr, "Prevents the controller from changing the current value"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeController *controller) override{
		return aeUControllerToggleFrozen::Ref::New(controller);
	}
	
	void Update(const aeAnimator &, const aeController &controller) override{
		SetEnabled(true);
		SetSelected(controller.GetFrozen());
	}
};

class cComboLocoAttr : public igdeComboBoxListener{
	aeWPController &pPanel;
	
public:
	typedef deTObjectReference<cComboLocoAttr> Ref;
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
	typedef deTObjectReference<cTextLocoLeg> Ref;
	cTextLocoLeg(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeController *controller) override{
		const int value = textField->GetInteger();
		if(value >= 0 && value < animator->GetLocomotion().GetLegs().GetCount()){
			controller->SetLocomotionLeg(value);
		}
		return {};
	}
};

class cComboVectorSimulation : public igdeComboBoxListener{
	aeWPController &pPanel;
	
public:
	typedef deTObjectReference<cComboVectorSimulation> Ref;
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
	typedef deTObjectReference<cTextDefaultValue> Ref;
	cTextDefaultValue(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller) override{
		const float value = textField->GetFloat();
		return fabsf(controller->GetDefaultValue() - value) > FLOAT_SAFE_EPSILON
			? aeUControllerSetDefaultValue::Ref::New(controller, value) : igdeUndo::Ref();
	}
};

class cEditDefaultVector : public igdeEditVectorListener{
	aeWPController &pPanel;
	
public:
	typedef deTObjectReference<cEditDefaultVector> Ref;
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
				aeUControllerSetDefaultVector::Ref::New(controller, vector));
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
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = aeWPControllerListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// controllers
	helper.GroupBoxFlow(content, groupBox, "Controllers:");
	helper.ListBox(groupBox, 8, "Controllers", pListController, cListControllers::Ref::New(*this));
	
	
	// controller settings
	helper.GroupBox(content, groupBox, "Controller Settings:");
	helper.EditString(groupBox, "Name:", "Name of the controller", pEditName, cTextName::Ref::New(*this));
	helper.EditFloat(groupBox, "Minimum Value:", "Minimum controller value",
		pEditMin, cTextMinimumValue::Ref::New(*this));
	helper.EditFloat(groupBox, "Maximum Value:", "Maximum controller value",
		pEditMax, cTextMaximumValue::Ref::New(*this));
	helper.EditSliderText(groupBox, "Value:", "Current controller value",
		0.0f, 1.0f, 6, 3, 0.1f, pSldValue, cSlideValue::Ref::New(*this));
	helper.EditVector(groupBox, "Vector:", "Vector value of controller",
		pEditVector, cEditVector::Ref::New(*this));
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnSetFromMove, cActionSetFromMove::Ref::New(*this));
	helper.Button(formLine, pBtnResetValue, cActionResetValue::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkClamp, cActionClamp::Ref::New(*this));
	helper.CheckBox(groupBox, pChkFrozen, cActionFrozen::Ref::New(*this));
	
	helper.EditFloat(groupBox, "Default Value:", "Default controller value",
		pEditDefaultValue, cTextDefaultValue::Ref::New(*this));
	helper.EditVector(groupBox, "Default Vector:", "Default vector value of controller",
		pEditDefaultVector, cEditDefaultVector::Ref::New(*this));
	
	
	// locomotion testing
	helper.GroupBox(content, groupBox, "Locomotion Testing:");
	
	helper.ComboBox(groupBox, "Attribute:", "Selects the locomotion attribute affecting this controller",
		pCBLocoAttr, cComboLocoAttr::Ref::New(*this));
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
		pEditLocoLeg, cTextLocoLeg::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Vector Simulation:", "Select how to simulate vector value",
		pCBVectorSimulation, cComboVectorSimulation::Ref::New(*this));
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
	if(animator == pAnimator){
		return;
	}
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddNotifier(pListener);
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
		decString text;
		pAnimator->GetControllers().VisitIndexed([&](int i, aeController *controller){
			text.Format("%d: %s", i, controller->GetName().GetString());
			pListController->AddItem(text, nullptr, controller);
		});
	}
	
	pListController->SetSelectionWithData(selection);
	if(!pListController->GetSelectedItem() && pListController->GetItems().IsNotEmpty()){
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
