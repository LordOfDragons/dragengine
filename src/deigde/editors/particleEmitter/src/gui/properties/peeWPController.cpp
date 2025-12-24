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

#include "peeWPController.h"
#include "peeWPControllerListener.h"
#include "peeWindowProperties.h"
#include "../peeWindowMain.h"
#include "../../emitter/peeEmitter.h"
#include "../../emitter/peeController.h"
#include "../../undosys/controller/peeUControllerAdd.h"
#include "../../undosys/controller/peeUControllerMoveDown.h"
#include "../../undosys/controller/peeUControllerMoveUp.h"
#include "../../undosys/controller/peeUControllerRemove.h"
#include "../../undosys/controller/peeUControllerSetUpper.h"
#include "../../undosys/controller/peeUControllerSetLower.h"
#include "../../undosys/controller/peeUControllerSetName.h"
#include "../../undosys/controller/peeUControllerToggleClamp.h"
#include "../../undosys/controller/peeUControllerToggleFrozen.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	peeWPController &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(peeWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeController * const controller = pPanel.GetController();
		if(!emitter || !controller){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, emitter, controller));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter *emitter, peeController *controller) = 0;
};

class cBaseEditSliderTextListener : public igdeEditSliderTextListener{
protected:
	peeWPController &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditSliderTextListener> Ref;
	cBaseEditSliderTextListener(peeWPController &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditSliderText *editSlider){
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeController * const controller = pPanel.GetController();
		if(!emitter || !controller){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editSlider->GetValue(), emitter, controller));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(float value, peeEmitter *emitter, peeController *controller) = 0;
};

class cBaseAction : public igdeAction{
protected:
	peeWPController &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(peeWPController &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	cBaseAction(peeWPController &panel, igdeIcon *icon, const char *description) :
	igdeAction("", icon, description),
	pPanel(panel){}
	
	cBaseAction(peeWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(emitter));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(peeEmitter *emitter) = 0;
};

class cBaseActionController : public cBaseAction{
public:
	cBaseActionController(peeWPController &panel, const char *text, const char *description) :
	cBaseAction(panel, text, description){}
	
	cBaseActionController(peeWPController &panel, igdeIcon *icon, const char *description) :
	cBaseAction(panel, icon, description){}
	
	cBaseActionController(peeWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(peeEmitter *emitter) override{
		peeController * const controller = pPanel.GetController();
		if(controller){
			return OnActionController(emitter, controller);
			
		}else{
			return {};
		}
	}
	
	virtual igdeUndo::Ref OnActionController(peeEmitter *emitter, peeController *controller) = 0;
};



class cListControllers : public igdeListBoxListener{
	peeWPController &pPanel;
public:
	typedef deTObjectReference<cListControllers> Ref;
	cListControllers(peeWPController &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		
		if(selection){
			emitter->SetActiveController((peeController*)selection->GetData());
			
		}else{
			emitter->SetActiveController(nullptr);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand(menu, pPanel.GetActionControllerAdd());
		helper.MenuCommand(menu, pPanel.GetActionControllerRemove());
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionControllerUp());
		helper.MenuCommand(menu, pPanel.GetActionControllerDown());
	}
};

class cActionControllerAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionControllerAdd> Ref;
	cActionControllerAdd(peeWPController &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a controller to the end of the list."){}
	
	igdeUndo::Ref OnAction(peeEmitter *emitter) override{
		return peeUControllerAdd::Ref::New(emitter, peeController::Ref::New());
	}
};

class cActionControllerRemove : public cBaseActionController{
public:
	typedef deTObjectReference<cActionControllerRemove> Ref;
	cActionControllerRemove(peeWPController &panel) : cBaseActionController(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove the selected controller."){}
	
	virtual igdeUndo::Ref OnActionController(peeEmitter*, peeController *controller){
		return peeUControllerRemove::Ref::New(controller);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetController() != nullptr);
	}
};

class cActionControllerUp : public cBaseActionController{
	igdeListBox &pListBox;
public:
	typedef deTObjectReference<cActionControllerUp> Ref;
	cActionControllerUp(peeWPController &panel, igdeListBox &listBox) : cBaseActionController(
		panel, "Move Up", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move controller up in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo::Ref OnActionController(peeEmitter*, peeController *controller){
		return peeUControllerMoveUp::Ref::New(controller);
	}
	
	void Update() override{
		SetEnabled(pListBox.GetSelection() > 0);
	}
};

class cActionControllerDown : public cBaseActionController{
	igdeListBox &pListBox;
public:
	typedef deTObjectReference<cActionControllerDown> Ref;
	cActionControllerDown(peeWPController &panel, igdeListBox &listBox) : cBaseActionController(
		panel, "Move Down", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move controller down in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo::Ref OnActionController(peeEmitter*, peeController *controller){
		return peeUControllerMoveDown::Ref::New(controller);
	}
	
	void Update() override{
		SetEnabled(pListBox.GetSelection() >= 0 && pListBox.GetSelection() < pListBox.GetItems().GetCount() - 1);
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(peeWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeController *controller) override{
		const decString &name = textField->GetText();
		if(name == controller->GetName()){
			return {};
		}
		return peeUControllerSetName::Ref::New(controller, name);
	}
};

class cTextMinimumValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextMinimumValue> Ref;
	cTextMinimumValue(peeWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeController *controller) override{
		const float value = textField->GetFloat();
		if(fabsf(value - controller->GetLower()) <= FLOAT_SAFE_EPSILON){
			return {};
		}
		return peeUControllerSetLower::Ref::New(controller, value);
	}
};

class cTextMaximumValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextMaximumValue> Ref;
	cTextMaximumValue(peeWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeController *controller) override{
		const float value = textField->GetFloat();
		if(fabsf(value - controller->GetUpper()) <= FLOAT_SAFE_EPSILON){
			return {};
		}
		return peeUControllerSetUpper::Ref::New(controller, value);
	}
};

class cSliderValue : public igdeEditSliderTextListener{
	peeWPController &pPanel;
public:
	typedef deTObjectReference<cSliderValue> Ref;
	cSliderValue(peeWPController &panel) : pPanel(panel){}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		OnSliderTextValueChanged(sliderText);
	}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		peeController * const controller = pPanel.GetController();
		if(controller){
			controller->SetValue(sliderText->GetValue());
		}
	}
};

class cActionClamp : public cBaseActionController{
public:
	typedef deTObjectReference<cActionClamp> Ref;
	cActionClamp(peeWPController &panel) : cBaseActionController(panel, "Clamp value to range",
		"Determines if the value of the controller is clamped to the given range."){ }
	
	virtual igdeUndo::Ref OnActionController(peeEmitter*, peeController *controller){
		return peeUControllerToggleClamp::Ref::New(controller);
	}
};

class cActionFrozen : public cBaseActionController{
public:
	typedef deTObjectReference<cActionFrozen> Ref;
	cActionFrozen(peeWPController &panel) : cBaseActionController(panel,
		"Freeze Controller value", "Prevents the controller from changing the current value."){}
	
	virtual igdeUndo::Ref OnActionController(peeEmitter*, peeController *controller){
		return peeUControllerToggleFrozen::Ref::New(controller);
	}
};

}


// Class peeWPController
/////////////////////////

// Constructor, destructor
////////////////////////////

peeWPController::peeWPController(peeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = peeWPControllerListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// controller list
	helper.GroupBoxFlow(content, groupBox, "Controllers:");
	
	helper.ListBox(groupBox, 8, "Controllers", pListController, cListControllers::Ref::New(*this));
	
	pActionControllerAdd = cActionControllerAdd::Ref::New(*this);
	pActionControllerRemove = cActionControllerRemove::Ref::New(*this);
	pActionControllerUp = cActionControllerUp::Ref::New(*this, pListController);
	pActionControllerDown = cActionControllerDown::Ref::New(*this, pListController);
	
	// controller settings
	helper.GroupBox(content, groupBox, "Controller Settings:");
	helper.EditString(groupBox, "Name:", "Name of the controller", pEditName, cTextName::Ref::New(*this));
	
	helper.FormLine(groupBox, "Range:", "Range of the controller value if limited", frameLine);
	helper.EditFloat(frameLine, "Minimum value the controller can take",
		pEditMin, cTextMinimumValue::Ref::New(*this));
	helper.EditFloat(frameLine, "Maximum value the controller can take",
		pEditMax, cTextMaximumValue::Ref::New(*this));
	
	helper.EditSliderText(groupBox, "Value:", "Current value of the controller",
		0.0f, 0.0f, 4, 3, 0.1f, pSldValue, cSliderValue::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkClamp, cActionClamp::Ref::New(*this));
	helper.CheckBox(groupBox, pChkFrozen, cActionFrozen::Ref::New(*this));
}

peeWPController::~peeWPController(){
	if(pEmitter){
		pEmitter->RemoveListener(pListener);
		pEmitter = nullptr;
	}
}



// Management
///////////////

void peeWPController::SetEmitter(peeEmitter *emitter){
	if(emitter == pEmitter){
		return;
	}
	
	if(pEmitter){
		pEmitter->RemoveListener(pListener);
	}
	
	pEmitter = emitter;
	
	if(emitter){
		emitter->AddListener(pListener);
	}
	
	UpdateControllerList();
}

peeController *peeWPController::GetController() const{
	return pEmitter ? pEmitter->GetActiveController() : nullptr;
}



void peeWPController::UpdateControllerList(){
	pListController->RemoveAllItems();
	
	if(pEmitter){
		const peeController::List &controllers = pEmitter->GetControllers();
		const int controllerCount = controllers.GetCount();
		decString text;
		int i;
		
		for(i=0; i<controllerCount; i++){
			peeController * const controller = controllers.GetAt(i);
			text.Format("%i: %s", i, controller->GetName().GetString());
			pListController->AddItem(text, nullptr, controller);
		}
	}
	
	SelectActiveController();
}

void peeWPController::SelectActiveController(){
	pListController->SetSelectionWithData(GetController());
	pListController->MakeSelectionVisible();
	UpdateController();
}

void peeWPController::UpdateController(){
	const peeController * const controller = GetController();
	
	if(controller){
		pEditName->SetText(controller->GetName());
		pEditMin->SetFloat(controller->GetLower());
		pEditMax->SetFloat(controller->GetUpper());
		
		pSldValue->SetRange(controller->GetLower(), controller->GetUpper());
		pSldValue->SetTickSpacing((controller->GetUpper() - controller->GetLower()) * 0.1f);
		pSldValue->SetValue(controller->GetValue());
		
		pChkClamp->SetChecked(controller->GetClamp());
		pChkFrozen->SetChecked(controller->GetFrozen());
		
	}else{
		pEditName->ClearText();
		pEditMin->ClearText();
		pEditMax->ClearText();
		pSldValue->SetRange(0, 0);
		pChkClamp->SetChecked(false);
		pChkFrozen->SetChecked(false);
	}
	
	const bool enable = controller != nullptr;
	pEditName->SetEnabled(enable);
	pEditMin->SetEnabled(enable);
	pEditMax->SetEnabled(enable);
	pSldValue->SetEnabled(enable);
	pChkClamp->SetEnabled(enable);
	pChkFrozen->SetEnabled(enable);
}

void peeWPController::UpdateControllerValue(){
	peeController * const controller = GetController();
	if(controller){
		pSldValue->SetValue(controller->GetValue());
	}
}
