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

#include "seWPController.h"
#include "seWPControllerListener.h"
#include "../seWindowMain.h"
#include "../seViewSynthesizer.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/controller/seController.h"
#include "../../undosys/controller/seUAddController.h"
#include "../../undosys/controller/seURemoveController.h"
#include "../../undosys/controller/seUMoveControllerUp.h"
#include "../../undosys/controller/seUMoveControllerDown.h"
#include "../../undosys/controller/seUControllerSetName.h"
#include "../../undosys/controller/seUControllerSetMinimumValue.h"
#include "../../undosys/controller/seUControllerSetMaximumValue.h"
#include "../../undosys/controller/seUControllerToggleClamp.h"
#include "../../undosys/controller/seUControllerSetCurve.h"

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
	seWPController &pPanel;
	
public:
	cBaseTextFieldListener(seWPController &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seController * const controller = pPanel.GetController();
		if(!controller){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, controller)));
		if(undo){
			controller->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seController *controller) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPController &pPanel;
	
public:
	cBaseAction(seWPController &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		seController * const controller = pPanel.GetController();
		if(!controller){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(controller)));
		if(undo){
			controller->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(seController *controller) = 0;
};



class cListControllers : public igdeListBoxListener{
	seWPController &pPanel;
	
public:
	cListControllers(seWPController &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		synthesizer->SetActiveController(listBox->GetSelectedItem()
			? (seController*)listBox->GetSelectedItem()->GetData() : NULL);
		pPanel.UpdateController();
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		seWindowMain &windowMain = pPanel.GetViewSynthesizer().GetWindowMain();
		
		helper.MenuCommand(menu, windowMain.GetActionControllerAdd());
		helper.MenuCommand(menu, windowMain.GetActionControllerRemove());
		helper.MenuCommand(menu, windowMain.GetActionControllerUp());
		helper.MenuCommand(menu, windowMain.GetActionControllerDown());
	}
};

class cTextName : public cBaseTextFieldListener{
public:
	cTextName(seWPController &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seController *controller){
		const decString &text = textField->GetText();
		if(text == controller->GetName()){
			return NULL;
		}
		
		if(pPanel.GetSynthesizer()->GetControllers().HasNamed(text)){
			igdeCommonDialogs::Error(&pPanel, "Invalid Value", "Duplicate controller name");
			textField->SetText(controller->GetName());
			return NULL;
		}
		
		return new seUControllerSetName(controller, text);
	}
};

class cTextMinimumValue : public cBaseTextFieldListener{
public:
	cTextMinimumValue(seWPController &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seController *controller){
		const float value = textField->GetFloat();
		return fabsf(value - controller->GetMinimumValue()) > FLOAT_SAFE_EPSILON
			? new seUControllerSetMinimumValue(controller, value) : NULL;
	}
};

class cTextMaximumValue : public cBaseTextFieldListener{
public:
	cTextMaximumValue(seWPController &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seController *controller){
		const float value = textField->GetFloat();
		return fabsf(value - controller->GetMaximumValue()) > FLOAT_SAFE_EPSILON
			? new seUControllerSetMaximumValue(controller, value) : NULL;
	}
};

class cActionClamp : public cBaseAction{
public:
	cActionClamp(seWPController &panel) : cBaseAction(panel, "Clamp value to range",
		NULL, "Determines if the value of the controller is clamped to the given range"){ }
	
	virtual igdeUndo *OnAction(seController *controller){
		return new seUControllerToggleClamp(controller);
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	seWPController &pPanel;
	igdeUndo::Ref pUndo;
	
public:
	cEditCurve(seWPController &panel) : pPanel(panel){}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seUControllerSetCurve&)(igdeUndo&)pUndo).SetNewValue(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetController() || pPanel.GetController()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo.TakeOver(new seUControllerSetCurve(pPanel.GetController(), viewCurveBezier->GetCurve()));
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(pUndo);
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seUControllerSetCurve&)(igdeUndo&)pUndo).SetNewValue(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetController() && pPanel.GetController()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo.TakeOver(new seUControllerSetCurve(pPanel.GetController(), viewCurveBezier->GetCurve()));
		}
	}
};

class cTextCurveConstValue : public cBaseTextFieldListener{
public:
	cTextCurveConstValue(seWPController &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seController *controller){
		controller->SetEditConstantValue(textField->GetFloat());
		return NULL;
	}
};

class cActionSetCurve : public cBaseAction{
	igdeViewCurveBezier::Ref pEditCurve;
	
public:
	cActionSetCurve(seWPController &panel, igdeViewCurveBezier *editCurve,
		const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pEditCurve(editCurve){}
	
	virtual igdeUndo *OnAction(seController *controller){
		decCurveBezier curve;
		CreateCurve(*controller, curve);
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(
			seUControllerSetCurve::Ref::NewWith(controller, curve));
		pEditCurve->ResetView();
		return NULL;
	}
	
	virtual void CreateCurve(const seController &controller, decCurveBezier &curve) = 0;
	
	virtual void Update(){
		SetEnabled(pPanel.GetController());
	}
};

class cActionCurveSetConstValue : public cActionSetCurve{
	igdeTextField::Ref pEditConstValue;
	
public:
	cActionCurveSetConstValue(seWPController &panel, igdeViewCurveBezier *editCurve, igdeTextField *editConstValue) :
		cActionSetCurve(panel, editCurve, "Const", NULL, "Set curve to constant value"),
	pEditConstValue(editConstValue){}
	
	virtual void CreateCurve(const seController &controller, decCurveBezier &curve){
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
	cActionCurveSetLinear(seWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Linear", NULL, "Set curve to linear curve from 0 to 1"){}
	
	virtual void CreateCurve(const seController &controller, decCurveBezier &curve){
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
	cActionCurveSetLinearInverse(seWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Inverse", NULL, "Set curve to linear curve from 1 to 0"){}
	
	virtual void CreateCurve(const seController &controller, decCurveBezier &curve){
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
	cActionCurveSetBezier(seWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Bezier", NULL, "Set curve to bezier curve from 0 to 1"){}
	
	virtual void CreateCurve(const seController &controller, decCurveBezier &curve){
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
	cActionCurveSetBezierInverse(seWPController &panel, igdeViewCurveBezier *editCurve) :
		cActionSetCurve(panel, editCurve, "Inverse", NULL, "Set curve to bezier curve from 1 to 0"){}
	
	virtual void CreateCurve(const seController &controller, decCurveBezier &curve){
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



// Class seWPController
/////////////////////////

// Constructor, destructor
////////////////////////////

seWPController::seWPController(seViewSynthesizer &viewSynthesizer) :
igdeContainerScroll(viewSynthesizer.GetEnvironment(), false, true),
pViewSynthesizer(viewSynthesizer),
pListener(NULL),
pSynthesizer(NULL)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener.TakeOverWith(*this);
	
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Controllers:");
	helper.ListBox(groupBox, 10, "Controllers", pListController, new cListControllers(*this));
	pListController->SetDefaultSorter();
	
	
	helper.GroupBoxStatic(content, groupBox, "Controller Settings:");
	helper.EditString(groupBox, "Name:", "Name of controller", pEditName, new cTextName(*this));
	helper.EditFloat(groupBox, "Minimum Value:", "Minimum controller value",
		pEditMin, new cTextMinimumValue(*this));
	helper.EditFloat(groupBox, "Maximum Value:", "Maximum controller value",
		pEditMax, new cTextMaximumValue(*this));
	helper.CheckBox(groupBox, pChkClamp, new cActionClamp(*this));
	
	
	helper.GroupBoxFlow(content, groupBox, "Controller Curve:");
	
	helper.ViewCurveBezier(groupBox, pEditCurve, new cEditCurve(*this));
	pEditCurve->SetDefaultSize(decPoint(200, 250));
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp(true);
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
	
	formLine.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaX));
	groupBox->AddChild(formLine);
	helper.EditFloat(formLine, "Constant curve value", 6, 3,
		pEditCurveSetConstValue, new cTextCurveConstValue(*this));
	helper.Button(formLine, pBtnCurveSetConstant,
		new cActionCurveSetConstValue(*this, pEditCurve, pEditCurveSetConstValue), true);
	
	helper.Separator(formLine);
	helper.Button(formLine, pBtnCurveSetLinear,
		new cActionCurveSetLinear(*this, pEditCurve), true);
	helper.Button(formLine, pBtnCurveSetLinearInverse,
		new cActionCurveSetLinearInverse(*this, pEditCurve), true);
	
	helper.Separator(formLine);
	helper.Button(formLine, pBtnCurveSetBezier,
		new cActionCurveSetBezier(*this, pEditCurve), true);
	helper.Button(formLine, pBtnCurveSetBezierInverse,
		new cActionCurveSetBezierInverse(*this, pEditCurve), true);
}

seWPController::~seWPController(){
	SetSynthesizer(NULL);
}



// Management
///////////////

void seWPController::SetSynthesizer(seSynthesizer *synthesizer){
	if(pSynthesizer == synthesizer){
		return;
	}
	
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		synthesizer->AddNotifier(pListener);
		synthesizer->AddReference();
	}
	
	UpdateControllerList();
}

seController *seWPController::GetController() const{
	return pSynthesizer ? pSynthesizer->GetActiveController() : NULL;
}

void seWPController::SelectActiveController(){
	pListController->SetSelectionWithData(GetController());
}



void seWPController::UpdateControllerList(){
	const seController::Ref selection(GetController());
	
	pListController->RemoveAllItems();
	
	if(pSynthesizer){
		const seControllerList &list = pSynthesizer->GetControllers();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			seController * const controller = list.GetAt(i);
			text.Format("%d: %s", i, controller->GetName().GetString());
			pListController->AddItem(text, NULL, controller);
		}
		
		pListController->SortItems();
	}
	
	pListController->SetSelectionWithData(selection);
	if(!pListController->GetSelectedItem() && pListController->GetItemCount() > 0){
		pListController->SetSelection(0);
	}
	
	UpdateController();
}

void seWPController::UpdateController(){
	const seController * const controller = GetController();
	
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

void seWPController::UpdateControllerRange(){
	const seController * const controller = GetController();
	
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

void seWPController::UpdateControllerCurve(){
	const seController * const controller = GetController();
	
	if(controller){
		pEditCurve->SetCurve(controller->GetCurve());
		
	}else{
		pEditCurve->ClearCurve();
	}
}
