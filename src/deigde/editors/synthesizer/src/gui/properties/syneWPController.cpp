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

#include "syneWPController.h"
#include "syneWPControllerListener.h"
#include "../syneWindowMain.h"
#include "../syneViewSynthesizer.h"
#include "../../synthesizer/syneSynthesizer.h"
#include "../../synthesizer/controller/syneController.h"
#include "../../undosys/controller/syneUAddController.h"
#include "../../undosys/controller/syneURemoveController.h"
#include "../../undosys/controller/syneUMoveControllerUp.h"
#include "../../undosys/controller/syneUMoveControllerDown.h"
#include "../../undosys/controller/syneUControllerSetName.h"
#include "../../undosys/controller/syneUControllerSetMinimumValue.h"
#include "../../undosys/controller/syneUControllerSetMaximumValue.h"
#include "../../undosys/controller/syneUControllerToggleClamp.h"
#include "../../undosys/controller/syneUControllerSetCurve.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	syneWPController &pPanel;
	
public:
	cBaseTextFieldListener(syneWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		syneController * const controller = pPanel.GetController();
		if(!controller){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, controller));
		if(undo){
			controller->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, syneController *controller) = 0;
};

class cBaseAction : public igdeAction{
protected:
	syneWPController &pPanel;
	
public:
	cBaseAction(syneWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		syneController * const controller = pPanel.GetController();
		if(!controller){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(controller));
		if(undo){
			controller->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(syneController *controller) = 0;
};



class cListControllers : public igdeListBoxListener{
	syneWPController &pPanel;
	
public:
	typedef deTObjectReference<cListControllers> Ref;
	cListControllers(syneWPController &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		synthesizer->SetActiveController(listBox->GetSelectedItem()
			? (syneController*)listBox->GetSelectedItem()->GetData() : nullptr);
		pPanel.UpdateController();
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		syneWindowMain &windowMain = pPanel.GetViewSynthesizer().GetWindowMain();
		
		helper.MenuCommand(menu, windowMain.GetActionControllerAdd());
		helper.MenuCommand(menu, windowMain.GetActionControllerRemove());
		helper.MenuCommand(menu, windowMain.GetActionControllerUp());
		helper.MenuCommand(menu, windowMain.GetActionControllerDown());
	}
};

class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(syneWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, syneController *controller) override{
		const decString &text = textField->GetText();
		if(text == controller->GetName()){
			return {};
		}
		
		if(pPanel.GetSynthesizer()->GetControllers().HasMatching([&](const syneController &c){
			return c.GetName() == text;
		})){
			igdeCommonDialogs::Error(&pPanel, "Invalid Value", "Duplicate controller name");
			textField->SetText(controller->GetName());
			return {};
		}
		
		return syneUControllerSetName::Ref::New(controller, text);
	}
};

class cTextMinimumValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextMinimumValue> Ref;
	cTextMinimumValue(syneWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, syneController *controller) override{
		const float value = textField->GetFloat();
		return fabsf(value - controller->GetMinimumValue()) > FLOAT_SAFE_EPSILON
			? syneUControllerSetMinimumValue::Ref::New(controller, value) : igdeUndo::Ref();
	}
};

class cTextMaximumValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextMaximumValue> Ref;
	cTextMaximumValue(syneWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, syneController *controller) override{
		const float value = textField->GetFloat();
		return fabsf(value - controller->GetMaximumValue()) > FLOAT_SAFE_EPSILON
			? syneUControllerSetMaximumValue::Ref::New(controller, value) : igdeUndo::Ref();
	}
};

class cActionClamp : public cBaseAction{
public:
	typedef deTObjectReference<cActionClamp> Ref;
	cActionClamp(syneWPController &panel) : cBaseAction(panel, "Clamp value to range",
		nullptr, "Determines if the value of the controller is clamped to the given range"){ }
	
	igdeUndo::Ref OnAction(syneController *controller) override{
		return syneUControllerToggleClamp::Ref::New(controller);
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	syneWPController &pPanel;
	igdeUndo::Ref pUndo;
	
public:
	typedef deTObjectReference<cEditCurve> Ref;
	cEditCurve(syneWPController &panel) : pPanel(panel){}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((syneUControllerSetCurve&)(igdeUndo&)pUndo).SetNewValue(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetController() || pPanel.GetController()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo = syneUControllerSetCurve::Ref::New(pPanel.GetController(), viewCurveBezier->GetCurve());
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(pUndo);
		pUndo = nullptr;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((syneUControllerSetCurve&)(igdeUndo&)pUndo).SetNewValue(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetController() && pPanel.GetController()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo = syneUControllerSetCurve::Ref::New(pPanel.GetController(), viewCurveBezier->GetCurve());
		}
	}
};

class cTextCurveConstValue : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCurveConstValue> Ref;
	cTextCurveConstValue(syneWPController &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, syneController *controller) override{
		controller->SetEditConstantValue(textField->GetFloat());
		return {};
	}
};

class cActionSetCurve : public cBaseAction{
	igdeViewCurveBezier::Ref pEditCurve;
	
public:
	typedef deTObjectReference<cActionSetCurve> Ref;
	cActionSetCurve(syneWPController &panel, igdeViewCurveBezier *editCurve,
		const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pEditCurve(editCurve){}
	
	igdeUndo::Ref OnAction(syneController *controller) override{
		decCurveBezier curve;
		CreateCurve(*controller, curve);
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(
			syneUControllerSetCurve::Ref::New(controller, curve));
		pEditCurve->ResetView();
		return {};
	}
	
	virtual void CreateCurve(const syneController &controller, decCurveBezier &curve) = 0;
	
	void Update() override{
		SetEnabled(pPanel.GetController());
	}
};

class cActionCurveSetConstValue : public cActionSetCurve{
	igdeTextField::Ref pEditConstValue;
	
public:
	typedef deTObjectReference<cActionCurveSetConstValue> Ref;
	cActionCurveSetConstValue(syneWPController &panel, igdeViewCurveBezier *editCurve, igdeTextField *editConstValue) :
		cActionSetCurve(panel, editCurve, "Const", nullptr, "Set curve to constant value"),
	pEditConstValue(editConstValue){}
	
	virtual void CreateCurve(const syneController &controller, decCurveBezier &curve){
		const float xmax = pPanel.GetSynthesizer()->GetPlayTime();
		const float y = decMath::clamp(pEditConstValue->GetFloat(),
			controller.GetMinimumValue(), controller.GetMaximumValue());
		
		curve.AddPoint(decCurveBezierPoint(decVector2(xmax * 0.5f, y),
			decVector2(xmax * 0.25f, y), decVector2(xmax * 1.5f, y)));
		curve.SetInterpolationMode(decCurveBezier::eimConstant);
	}
};

class cActionCurveSetLinear : public cActionSetCurve{
public:
	typedef deTObjectReference<cActionCurveSetLinear> Ref;
	cActionCurveSetLinear(syneWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Linear", nullptr, "Set curve to linear curve from 0 to 1"){}
	
	virtual void CreateCurve(const syneController &controller, decCurveBezier &curve){
		const float xmax = pPanel.GetSynthesizer()->GetPlayTime();
		const float ymin = controller.GetMinimumValue();
		const float ymax = controller.GetMaximumValue();
		const float xstep = xmax / 3.0f;
		const float ystep = (ymax - ymin) / 3.0f;
		
		curve.AddPoint(decCurveBezierPoint(decVector2(0.0f, ymin),
			decVector2(-xstep, ymin - ystep), decVector2(xstep, ymin + ystep)));
		curve.AddPoint(decCurveBezierPoint(decVector2(xmax, ymax),
			decVector2(xmax - xstep, ymax - ystep), decVector2(xmax + xstep, ymax + ystep)));
		curve.SetInterpolationMode(decCurveBezier::eimLinear);
	}
};

class cActionCurveSetLinearInverse : public cActionSetCurve{
public:
	typedef deTObjectReference<cActionCurveSetLinearInverse> Ref;
	cActionCurveSetLinearInverse(syneWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Inverse", nullptr, "Set curve to linear curve from 1 to 0"){}
	
	virtual void CreateCurve(const syneController &controller, decCurveBezier &curve){
		const float xmax = pPanel.GetSynthesizer()->GetPlayTime();
		const float ymin = controller.GetMinimumValue();
		const float ymax = controller.GetMaximumValue();
		const float xstep = xmax / 3.0f;
		const float ystep = (ymax - ymin) / 3.0f;
		
		curve.AddPoint(decCurveBezierPoint(decVector2(0.0f, ymax),
			decVector2(-xstep, ymax + ystep), decVector2(xstep, ymax - ystep)));
		curve.AddPoint(decCurveBezierPoint(decVector2(xmax, ymin),
			decVector2(xmax - xstep, ymin + ystep), decVector2(xmax + xstep, ymin - ystep)));
		curve.SetInterpolationMode(decCurveBezier::eimLinear);
	}
};

class cActionCurveSetBezier : public cActionSetCurve{
public:
	typedef deTObjectReference<cActionCurveSetBezier> Ref;
	cActionCurveSetBezier(syneWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Bezier", nullptr, "Set curve to bezier curve from 0 to 1"){}
	
	virtual void CreateCurve(const syneController &controller, decCurveBezier &curve){
		const float xmax = pPanel.GetSynthesizer()->GetPlayTime();
		const float ymin = controller.GetMinimumValue();
		const float ymax = controller.GetMaximumValue();
		const float xstep = xmax / 3.0f;
		
		curve.AddPoint(decCurveBezierPoint(decVector2(0.0f, ymin),
			decVector2(-xstep, ymin), decVector2(xstep, ymin)));
		curve.AddPoint(decCurveBezierPoint(decVector2(xmax, ymax),
			decVector2(xmax - xstep, ymax), decVector2(xmax + xstep, ymax)));
	}
};

class cActionCurveSetBezierInverse : public cActionSetCurve{
public:
	typedef deTObjectReference<cActionCurveSetBezierInverse> Ref;
	cActionCurveSetBezierInverse(syneWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Inverse", nullptr, "Set curve to bezier curve from 1 to 0"){}
	
	virtual void CreateCurve(const syneController &controller, decCurveBezier &curve){
		const float xmax = pPanel.GetSynthesizer()->GetPlayTime();
		const float ymin = controller.GetMinimumValue();
		const float ymax = controller.GetMaximumValue();
		const float xstep = xmax / 3.0f;
		
		curve.AddPoint(decCurveBezierPoint(decVector2(0.0f, ymax),
			decVector2(-xstep, ymax), decVector2(xstep, ymax)));
		curve.AddPoint(decCurveBezierPoint(decVector2(xmax, ymin),
			decVector2(xmax - xstep, ymin), decVector2(xmax + xstep, ymin)));
	}
};

}



// Class syneWPController
/////////////////////////

// Constructor, destructor
////////////////////////////

syneWPController::syneWPController(syneViewSynthesizer &viewSynthesizer) :
igdeContainerScroll(viewSynthesizer.GetEnvironment(), false, true),
pViewSynthesizer(viewSynthesizer)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = syneWPControllerListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Controllers:");
	helper.ListBox(groupBox, 10, "Controllers", pListController, cListControllers::Ref::New(*this));
	pListController->SetDefaultSorter();
	
	
	helper.GroupBoxStatic(content, groupBox, "Controller Settings:");
	helper.EditString(groupBox, "Name:", "Name of controller", pEditName, cTextName::Ref::New(*this));
	helper.EditFloat(groupBox, "Minimum Value:", "Minimum controller value",
		pEditMin, cTextMinimumValue::Ref::New(*this));
	helper.EditFloat(groupBox, "Maximum Value:", "Maximum controller value",
		pEditMax, cTextMaximumValue::Ref::New(*this));
	helper.CheckBox(groupBox, pChkClamp, cActionClamp::Ref::New(*this));
	
	
	helper.GroupBoxFlow(content, groupBox, "Controller Curve:");
	
	helper.ViewCurveBezier(groupBox, pEditCurve, cEditCurve::Ref::New(*this));
	pEditCurve->SetDefaultSize(decPoint(200, 250));
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp(true);
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX);
	groupBox->AddChild(formLine);
	helper.EditFloat(formLine, "Constant curve value", 6, 3,
		pEditCurveSetConstValue, cTextCurveConstValue::Ref::New(*this));
	helper.Button(formLine, pBtnCurveSetConstant, cActionCurveSetConstValue::Ref::New(*this, pEditCurve, pEditCurveSetConstValue));
	
	helper.Separator(formLine);
	helper.Button(formLine, pBtnCurveSetLinear, cActionCurveSetLinear::Ref::New(*this, pEditCurve));
	helper.Button(formLine, pBtnCurveSetLinearInverse, cActionCurveSetLinearInverse::Ref::New(*this, pEditCurve));
	
	helper.Separator(formLine);
	helper.Button(formLine, pBtnCurveSetBezier, cActionCurveSetBezier::Ref::New(*this, pEditCurve));
	helper.Button(formLine, pBtnCurveSetBezierInverse, cActionCurveSetBezierInverse::Ref::New(*this, pEditCurve));
}

syneWPController::~syneWPController(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

void syneWPController::SetSynthesizer(syneSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		synthesizer->AddNotifier(pListener);
	}
	
	UpdateControllerList();
}

syneController *syneWPController::GetController() const{
	return pSynthesizer ? pSynthesizer->GetActiveController() : nullptr;
}

void syneWPController::SelectActiveController(){
	pListController->SetSelectionWithData(GetController());
}



void syneWPController::UpdateControllerList(){
	const syneController::Ref selection(GetController());
	
	pListController->RemoveAllItems();
	
	if(pSynthesizer){
		const syneController::List &list = pSynthesizer->GetControllers();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			syneController * const controller = list.GetAt(i);
			text.Format("%d: %s", i, controller->GetName().GetString());
			pListController->AddItem(text, nullptr, controller);
		}
		
		pListController->SortItems();
	}
	
	pListController->SetSelectionWithData(selection);
	if(!pListController->GetSelectedItem() && pListController->GetItems().IsNotEmpty()){
		pListController->SetSelection(0);
	}
	
	UpdateController();
}

void syneWPController::UpdateController(){
	const syneController * const controller = GetController();
	
	if(controller){
		pEditName->SetText(controller->GetName());
		pChkClamp->SetChecked(controller->GetClamp());
		
		pEditCurveSetConstValue->SetFloat(controller->GetEditConstantValue());
		
	}else{
		pEditName->ClearText();
		pChkClamp->SetChecked(false);
		
		pEditCurveSetConstValue->ClearText();
	}
	
	const bool enabled = controller;
	pEditName->SetEnabled(enabled);
	pChkClamp->SetEnabled(enabled);
	
	pEditCurve->SetEnabled(enabled);
	pEditCurveSetConstValue->SetEnabled(enabled);
	pEditCurveSetConstValue->SetEnabled(enabled);
	pBtnCurveSetConstant->SetEnabled(enabled);
	pBtnCurveSetLinear->SetEnabled(enabled);
	pBtnCurveSetLinearInverse->SetEnabled(enabled);
	pBtnCurveSetBezier->SetEnabled(enabled);
	pBtnCurveSetBezierInverse->SetEnabled(enabled);
	
	UpdateControllerRange();
	UpdateControllerCurve();
}

void syneWPController::UpdateControllerRange(){
	const syneController * const controller = GetController();
	
	if(controller){
		pEditMin->SetFloat(controller->GetMinimumValue());
		pEditMax->SetFloat(controller->GetMaximumValue());
		
		pEditCurve->SetClamp(true);
		pEditCurve->SetClampMin(decVector2(0.0f, controller->GetMinimumValue()));
		pEditCurve->SetClampMax(decVector2(pSynthesizer->GetPlayTime(), controller->GetMaximumValue()));
		
	}else{
		pEditMin->ClearText();
		pEditMax->ClearText();
		
		pEditCurve->SetClamp(true);
		pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
		pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
		pEditCurve->ClearCurve();
	}
	
	const bool enabled = controller;
	pEditMin->SetEnabled(enabled);
	pEditMax->SetEnabled(enabled);
	
	pEditCurve->ResetView();
}

void syneWPController::UpdateControllerCurve(){
	const syneController * const controller = GetController();
	
	if(controller){
		pEditCurve->SetCurve(controller->GetCurve());
		
	}else{
		pEditCurve->ClearCurve();
	}
}
