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
	using Ref = deTObjectReference<cBaseAction>;
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
	using Ref = deTObjectReference<cBaseTextField>;
	cBaseTextField(aeWPController &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
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
	using Ref = deTObjectReference<cActionCopy>;
	
public:
	cActionCopy(aeWPController &panel) : cBaseAction(panel, "@Animator.WPController.Action.Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Animator.WPController.Action.Copy.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeController *controller) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataController::Ref::New(controller));
		return {};
	}
};

class cActionCut : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionCut>;
	
public:
	cActionCut(aeWPController &panel) : cBaseAction(panel, "@Animator.WPController.Action.Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"@Animator.WPController.Action.Cut.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeController *controller) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataController::Ref::New(controller));
		return aeURemoveController::Ref::New(animator, controller);
	}
};

class cActionPaste : public igdeAction{
public:
	using Ref = deTObjectReference<cActionPaste>;
	
private:
	aeWPController &pPanel;
	
public:
	cActionPaste(aeWPController &panel) : igdeAction("@Animator.WPController.Action.Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Animator.WPController.Action.Paste.ToolTip"), pPanel(panel){}
	
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
	using Ref = deTObjectReference<cListControllers>;
	cListControllers(aeWPController &panel) : pPanel(panel){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pPanel.GetAnimator()){
			pPanel.GetAnimator()->SetActiveController(listBox->GetSelectedItem()
				? (aeController*)listBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
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
	using Ref = deTObjectReference<cTextName>;
	
	cTextName(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeController *controller) override{
		const decString value = textField->GetText();
		if(controller->GetName() == value){
			return {};
		}
		if(animator->GetControllers().HasNamed(value)){
			igdeCommonDialogs::Error(pPanel, "@Animator.WPController.Dialog.SetControllerName.Title",
				"@Animator.WPController.Dialog.SetControllerName.DuplicateName");
			textField->SetText(controller->GetName());
			return {};
		}
		return aeUControllerSetName::Ref::New(controller, value);
	}
};

class cTextMinimumValue : public cBaseTextField{
public:
	using Ref = deTObjectReference<cTextMinimumValue>;
	cTextMinimumValue(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller) override{
		const float value = textField->GetFloat();
		return fabsf(controller->GetMinimumValue() - value) > FLOAT_SAFE_EPSILON
			? aeUControllerSetMinimumValue::Ref::New(controller, value) : igdeUndo::Ref();
	}
};

class cTextMaximumValue : public cBaseTextField{
public:
	using Ref = deTObjectReference<cTextMaximumValue>;
	cTextMaximumValue(aeWPController &panel) : cBaseTextField(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeController *controller) override{
		const float value = textField->GetFloat();
		return fabsf(controller->GetMaximumValue() - value) > FLOAT_SAFE_EPSILON
			? aeUControllerSetMaximumValue::Ref::New(controller, value) : igdeUndo::Ref();
	}
};

class cActionSetFromMove : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionSetFromMove>;
	
public:
	cActionSetFromMove(aeWPController &panel) : cBaseAction(panel, "@Animator.WPController.SetFromMove",
		nullptr, "@Animator.WPController.SetFromMove.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeController *controller) override{
		const deAnimation * const animation = animator->GetEngineAnimator()
			? animator->GetEngineAnimator()->GetAnimation().Pointer() : nullptr;
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
		if(names.GetCount() == 0 || !igdeCommonDialogs::SelectString(pPanel,
		"@Animator.WPController.Dialog.SetRangeFromMove.Title",
		"@Animator.WPController.Dialog.SetRangeFromMove.Message", names, selection)){
			return {};
		}
		
		const deAnimationMove &move = animation->GetMove(animation->FindMove(names.GetAt(selection)));
		return aeUControllerSetFromMove::Ref::New(controller, 0.0f, move.GetPlaytime());
	}
};

class cActionResetValue : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionResetValue>;
	
public:
	cActionResetValue(aeWPController &panel) : cBaseAction(panel, "@Animator.WPController.Reset",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongLeft),
		"@Animator.WPController.Reset.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeController *controller) override{
		controller->SetCurrentValue(controller->GetMinimumValue());
		return {};
	}
};

class cSlideValue : public igdeEditSliderTextListener{
	aeWPController &pPanel;
	
public:
	using Ref = deTObjectReference<cSlideValue>;
	cSlideValue(aeWPController &panel) : pPanel(panel){}
	
	void OnSliderTextValueChanged(igdeEditSliderText *sliderText) override{
		aeController * const controller = pPanel.GetController();
		if(controller){
			controller->SetCurrentValue(sliderText->GetValue());
		}
	}
	
	void OnSliderTextValueChanging(igdeEditSliderText *sliderText) override{
		OnSliderTextValueChanged(sliderText);
	}
};

class cEditVector : public igdeEditVectorListener{
	aeWPController &pPanel;
	
public:
	using Ref = deTObjectReference<cEditVector>;
	cEditVector(aeWPController &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		aeController * const controller = pPanel.GetController();
		if(controller){
			controller->SetVector(editVector->GetVector());
		}
	}
};

class cActionClamp : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionClamp>;
	
public:
	cActionClamp(aeWPController &panel) : cBaseAction(panel, "@Animator.WPController.Clamp",
		nullptr, "@Animator.WPController.Clamp.ToolTip"){ }
	
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
	using Ref = deTObjectReference<cActionFrozen>;
	
public:
	cActionFrozen(aeWPController &panel) : cBaseAction(panel, "@Animator.WPController.Frozen",
		nullptr, "@Animator.WPController.Frozen.ToolTip"){}
	
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
	using Ref = deTObjectReference<cComboLocoAttr>;
	cComboLocoAttr(aeWPController &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeController * const controller = pPanel.GetController();
		if(controller && comboBox->GetSelectedItem()){
			controller->SetLocomotionAttribute((aeAnimatorLocomotion::eLocomotionTypes)
				(intptr_t)comboBox->GetSelectedItem()->GetData());
		}
	}
};

class cTextLocoLeg : public cBaseTextField{
public:
	using Ref = deTObjectReference<cTextLocoLeg>;
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
	using Ref = deTObjectReference<cComboVectorSimulation>;
	cComboVectorSimulation(aeWPController &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeController * const controller = pPanel.GetController();
		if(controller && comboBox->GetSelectedItem()){
			controller->SetVectorSimulation((aeController::eVectorSimulation)
				(intptr_t)comboBox->GetSelectedItem()->GetData());
		}
	}
};

class cTextDefaultValue : public cBaseTextField{
public:
	using Ref = deTObjectReference<cTextDefaultValue>;
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
	using Ref = deTObjectReference<cEditDefaultVector>;
	cEditDefaultVector(aeWPController &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
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
	helper.GroupBoxFlow(content, groupBox, "@Animator.WPController.Controllers");
	helper.ListBox(groupBox, 8, "@Animator.WPController.Controllers.ToolTip",
		pListController, cListControllers::Ref::New(*this));
	
	
	// controller settings
	helper.GroupBox(content, groupBox, "@Animator.WPController.ControllerSettings");
	helper.EditString(groupBox, "@Animator.WPController.Name",
		"@Animator.WPController.Name.ToolTip", pEditName, cTextName::Ref::New(*this));
	helper.EditFloat(groupBox, "@Animator.WPController.MinimumValue",
		"@Animator.WPController.MinimumValue.ToolTip", pEditMin, cTextMinimumValue::Ref::New(*this));
	helper.EditFloat(groupBox, "@Animator.WPController.MaximumValue",
		"@Animator.WPController.MaximumValue.ToolTip", pEditMax, cTextMaximumValue::Ref::New(*this));
	helper.EditSliderText(groupBox, "@Animator.WPController.Value",
		"@Animator.WPController.Value.ToolTip", 0.0f, 1.0f, 6, 3, 0.1f, pSldValue, cSlideValue::Ref::New(*this));
	helper.EditVector(groupBox, "@Animator.WPController.Vector",
		"@Animator.WPController.Vector.ToolTip", pEditVector, cEditVector::Ref::New(*this));
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnSetFromMove, cActionSetFromMove::Ref::New(*this));
	helper.Button(formLine, pBtnResetValue, cActionResetValue::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkClamp, cActionClamp::Ref::New(*this));
	helper.CheckBox(groupBox, pChkFrozen, cActionFrozen::Ref::New(*this));
	
	helper.EditFloat(groupBox, "@Animator.WPController.DefaultValue",
		"@Animator.WPController.DefaultValue.ToolTip", pEditDefaultValue, cTextDefaultValue::Ref::New(*this));
	helper.EditVector(groupBox, "@Animator.WPController.DefaultVector",
		"@Animator.WPController.DefaultVector.ToolTip", pEditDefaultVector, cEditDefaultVector::Ref::New(*this));
	
	
	// locomotion testing
	helper.GroupBox(content, groupBox, "@Animator.WPController.LocomotionTesting");
	
	helper.ComboBox(groupBox, "@Animator.WPController.Attribute",
		"@Animator.WPController.Attribute.ToolTip", pCBLocoAttr, cComboLocoAttr::Ref::New(*this));
	pCBLocoAttr->SetAutoTranslateItems(true);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.None", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaNone);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.ElapsedTime", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaElapsedTime);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.LookUpDown", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLookUpDown);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.LookLeftRight", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLookLeftRight);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.MovingSpeed", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaMovingSpeed);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.MovingDirection", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaMovingDirection);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.RelativeMovingSpeed", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaRelativeMovingSpeed);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.TurningSpeed", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaTurningSpeed);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.Stance", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaStance);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.Displacement", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaDisplacement);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.RelativeDisplacement", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaRelativeDisplacement);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.BodyTiltOffset", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaBodyTiltOffset);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.BodyTiltUpDown", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaBodyTiltUpDown);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.BodyTiltLeftRight", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaBodyTiltRightLeft);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.TurnInPlace", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaTimeTurnIP);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.LegGroundPosition", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegGroundPosition);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.LegGroundNormal", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegGroundNormal);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.LegInfluence", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegInfluence);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.LegPosition", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegPosition);
	pCBLocoAttr->AddItem("@Animator.WPController.Attribute.LegOrientation", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eaLegOrientation);
	
	helper.EditInteger(groupBox, "@Animator.WPController.Leg",
		"@Animator.WPController.Leg.ToolTip", pEditLocoLeg, cTextLocoLeg::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@Animator.WPController.VectorSimulation",
		"@Animator.WPController.VectorSimulation.ToolTip", pCBVectorSimulation, cComboVectorSimulation::Ref::New(*this));
	pCBVectorSimulation->SetAutoTranslateItems(true);
	pCBVectorSimulation->AddItem("@Animator.WPController.VectorSimulation.None", nullptr, (void*)(intptr_t)aeController::evsNone);
	pCBVectorSimulation->AddItem("@Animator.WPController.VectorSimulation.Position", nullptr, (void*)(intptr_t)aeController::evsPosition);
	pCBVectorSimulation->AddItem("@Animator.WPController.VectorSimulation.Rotation", nullptr, (void*)(intptr_t)aeController::evsRotation);
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
	pListController->UpdateRestoreSelection([&](){
		pListController->RemoveAllItems();
		
		if(pAnimator){
			decString text;
			pAnimator->GetControllers().VisitIndexed([&](int i, aeController *controller){
				text.Format("%d: %s", i, controller->GetName().GetString());
				pListController->AddItem(text, nullptr, controller);
			});
		}
	}, 0);
	
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
