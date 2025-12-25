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

#include "seWPController.h"
#include "seWPControllerListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../sky/seSky.h"
#include "../../sky/controller/seController.h"
#include "../../undosys/controller/seUControllerAdd.h"
#include "../../undosys/controller/seUControllerMoveDown.h"
#include "../../undosys/controller/seUControllerMoveUp.h"
#include "../../undosys/controller/seUControllerRemove.h"
#include "../../undosys/controller/seUControllerSetMaximum.h"
#include "../../undosys/controller/seUControllerSetMinimum.h"
#include "../../undosys/controller/seUControllerSetName.h"
#include "../../undosys/controller/seUControllerToggleClamp.h"
#include "../../undosys/controller/seUControllerToggleFrozen.h"

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
	seWPController &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(seWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSky * const sky = pPanel.GetSky();
		seController * const controller = pPanel.GetController();
		if(!sky || !controller){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, sky, controller));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seSky *sky, seController *controller) = 0;
};

class cBaseEditSliderTextListener : public igdeEditSliderTextListener{
protected:
	seWPController &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditSliderTextListener> Ref;
	cBaseEditSliderTextListener(seWPController &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditSliderText *editSlider){
		seSky * const sky = pPanel.GetSky();
		seController * const controller = pPanel.GetController();
		if(!sky || !controller){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editSlider->GetValue(), sky, controller));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(float value, seSky *sky, seController *controller) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPController &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(seWPController &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	cBaseAction(seWPController &panel, igdeIcon *icon, const char *description) :
	igdeAction("", icon, description),
	pPanel(panel){}
	
	cBaseAction(seWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		seSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(sky));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSky *sky) = 0;
};

class cBaseActionController : public cBaseAction{
public:
	typedef deTObjectReference<cBaseActionController> Ref;
	
public:
	cBaseActionController(seWPController &panel, const char *text, const char *description) :
	cBaseAction(panel, text, description){}
	
	cBaseActionController(seWPController &panel, igdeIcon *icon, const char *description) :
	cBaseAction(panel, icon, description){}
	
	cBaseActionController(seWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(seSky *sky) override{
		seController * const controller = pPanel.GetController();
		if(controller){
			return OnActionController(sky, controller);
			
		}else{
			return {};
		}
	}
	
	virtual igdeUndo::Ref OnActionController(seSky *sky, seController *controller) = 0;
};



class cListControllers : public igdeListBoxListener{
	seWPController &pPanel;
public:
	typedef deTObjectReference<cListControllers> Ref;
	cListControllers(seWPController &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		
		if(selection){
			sky->SetActiveController((seController*)selection->GetData());
			
		}else{
			sky->SetActiveController(nullptr);
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
	
public:
	cActionControllerAdd(seWPController &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a controller to the end of the list."){}
	
	igdeUndo::Ref OnAction(seSky *sky) override{
		return seUControllerAdd::Ref::New(sky, seController::Ref::New());
	}
};

class cActionControllerRemove : public cBaseActionController{
public:
	typedef deTObjectReference<cActionControllerRemove> Ref;
	cActionControllerRemove(seWPController &panel) : cBaseActionController(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove the selected controller."){}
	
	virtual igdeUndo::Ref OnActionController(seSky *sky, seController *controller){
		const int usageCount = sky->CountControllerUsage(controller);
		
		if(usageCount > 0 && igdeCommonDialogs::QuestionFormat(
			&pPanel, igdeCommonDialogs::ebsYesNo, "Remove Controller",
			"The controller '%s' is used by %i links.\n"
			"If the controller is removed now it is also removed from\n"
			"all the links using it. Do you want to remove the controller?",
			controller->GetName().GetString(), usageCount) != igdeCommonDialogs::ebYes){
				return {};
		}
		
		return seUControllerRemove::Ref::New(controller);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetController() != nullptr);
	}
};

class cActionControllerUp : public cBaseActionController{
	igdeListBox &pListBox;
public:
	typedef deTObjectReference<cActionControllerUp> Ref;
	cActionControllerUp(seWPController &panel, igdeListBox &listBox) : cBaseActionController(
		panel, "Move Up", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move controller up in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo::Ref OnActionController(seSky*, seController *controller){
		return seUControllerMoveUp::Ref::New(controller);
	}
	
	void Update() override{
		SetEnabled(pListBox.GetSelection() > 0);
	}
};

class cActionControllerDown : public cBaseActionController{
	igdeListBox &pListBox;
public:
	typedef deTObjectReference<cActionControllerDown> Ref;
	cActionControllerDown(seWPController &panel, igdeListBox &listBox) : cBaseActionController(
		panel, "Move Down", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move controller down in the list."),
	pListBox(listBox){}
	
	virtual igdeUndo::Ref OnActionController(seSky*, seController *controller){
		return seUControllerMoveDown::Ref::New(controller);
	}
	
	void Update() override{
		SetEnabled(pListBox.GetSelection() >= 0 && pListBox.GetSelection() < pListBox.GetItems().GetCount() - 1);
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(seWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seController *controller) override{
		const decString &name = textField->GetText();
		if(name == controller->GetName()){
			return {};
		}
		return seUControllerSetName::Ref::New(controller, name);
	}
};

class cTextMinimumValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextMinimumValue> Ref;
	cTextMinimumValue(seWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seController *controller) override{
		const float value = textField->GetFloat();
		if(fabsf(value - controller->GetMinimumValue()) <= FLOAT_SAFE_EPSILON){
			return {};
		}
		return seUControllerSetMinimum::Ref::New(controller, value);
	}
};

class cTextMaximumValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextMaximumValue> Ref;
	cTextMaximumValue(seWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seController *controller) override{
		const float value = textField->GetFloat();
		if(fabsf(value - controller->GetMaximumValue()) <= FLOAT_SAFE_EPSILON){
			return {};
		}
		return seUControllerSetMinimum::Ref::New(controller, value);
	}
};

class cSliderValue : public igdeEditSliderTextListener{
	seWPController &pPanel;
public:
	typedef deTObjectReference<cSliderValue> Ref;
	cSliderValue(seWPController &panel) : pPanel(panel){}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		OnSliderTextValueChanged(sliderText);
	}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		seController * const controller = pPanel.GetController();
		if(controller){
			controller->SetCurrentValue(sliderText->GetValue());
		}
	}
};

class cActionClamp : public cBaseActionController{
public:
	typedef deTObjectReference<cActionClamp> Ref;
	cActionClamp(seWPController &panel) : cBaseActionController(panel, "Clamp value to range",
		"Determines if the value of the controller is clamped to the given range."){ }
	
	virtual igdeUndo::Ref OnActionController(seSky*, seController *controller){
		return seUControllerToggleClamp::Ref::New(controller);
	}
};

class cActionFrozen : public cBaseActionController{
public:
	typedef deTObjectReference<cActionFrozen> Ref;
	cActionFrozen(seWPController &panel) : cBaseActionController(panel,
		"Freeze Controller value", "Prevents the controller from changing the current value."){}
	
	virtual igdeUndo::Ref OnActionController(seSky*, seController *controller){
		return seUControllerToggleFrozen::Ref::New(controller);
	}
};

}


// Class seWPController
/////////////////////////

// Constructor, destructor
////////////////////////////

seWPController::seWPController(seWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = seWPControllerListener::Ref::New(*this);
	
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
		0.0f, 0.0f, 6, 3, 0.1f, pSldValue, cSliderValue::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkClamp, cActionClamp::Ref::New(*this));
	helper.CheckBox(groupBox, pChkFrozen, cActionFrozen::Ref::New(*this));
}

seWPController::~seWPController(){
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky = nullptr;
	}
}



// Management
///////////////

void seWPController::SetSky(seSky *sky){
	if(sky == pSky){
		return;
	}
	
	if(pSky){
		pSky->RemoveListener(pListener);
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddListener(pListener);
	}
	
	UpdateControllerList();
}

seController *seWPController::GetController() const{
	return pSky ? pSky->GetActiveController() : nullptr;
}



void seWPController::UpdateControllerList(){
	pListController->RemoveAllItems();
	
	if(pSky){
		pSky->GetControllers().VisitIndexed([&](int i, seController *controller){
			decString text;
			text.Format("%i: %s", i, controller->GetName().GetString());
			pListController->AddItem(text, nullptr, controller);
		});
	}
	
	SelectActiveController();
}

void seWPController::SelectActiveController(){
	pListController->SetSelectionWithData(GetController());
	pListController->MakeSelectionVisible();
	UpdateController();
}

void seWPController::UpdateController(){
	const seController * const controller = GetController();
	
	if(controller){
		pEditName->SetText(controller->GetName());
		pEditMin->SetFloat(controller->GetMinimumValue());
		pEditMax->SetFloat(controller->GetMaximumValue());
		
		pSldValue->SetRange(controller->GetMinimumValue(), controller->GetMaximumValue());
		pSldValue->SetTickSpacing((controller->GetMaximumValue() - controller->GetMinimumValue()) * 0.1f);
		pSldValue->SetValue(controller->GetCurrentValue());
		
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

void seWPController::UpdateControllerValue(){
	seController * const controller = GetController();
	if(controller){
		pSldValue->SetValue(controller->GetCurrentValue());
	}
}
