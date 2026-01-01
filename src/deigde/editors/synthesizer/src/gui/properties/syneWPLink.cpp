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

#include "syneWPLink.h"
#include "syneWPLinkListener.h"
#include "../syneWindowMain.h"
#include "../syneViewSynthesizer.h"
#include "../../synthesizer/syneSynthesizer.h"
#include "../../synthesizer/controller/syneController.h"
#include "../../synthesizer/link/syneLink.h"
#include "../../undosys/link/syneULinkSetName.h"
#include "../../undosys/link/syneULinkSetController.h"
#include "../../undosys/link/syneULinkSetCurve.h"
#include "../../undosys/link/syneULinkAdd.h"
#include "../../undosys/link/syneULinkRemove.h"
#include "../../undosys/link/syneULinkSetRepeat.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
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
	syneWPLink &pPanel;
	
public:
	cBaseTextFieldListener(syneWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		syneLink * const link = pPanel.GetLink();
		if(!link){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, link));
		if(undo){
			link->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, syneLink *link) = 0;
};

class cBaseAction : public igdeAction{
protected:
	syneWPLink &pPanel;
	
public:
	cBaseAction(syneWPLink &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		syneLink * const link = pPanel.GetLink();
		if(!link){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(link));
		if(undo){
			link->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(syneLink *link) = 0;
};



class cListLinks : public igdeListBoxListener{
	syneWPLink &pPanel;
	
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(syneWPLink &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		synthesizer->SetActiveLink(listBox->GetSelectedItem()
			? (syneLink*)listBox->GetSelectedItem()->GetData() : nullptr);
		pPanel.UpdateLink();
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionLinkAdd());
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};


class cActionLinkAdd : public igdeAction{
	syneWPLink &pPanel;
	
public:
	typedef deTObjectReference<cActionLinkAdd> Ref;
	cActionLinkAdd(syneWPLink &panel) : igdeAction("Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add link"),
	pPanel(panel){}
	
	void OnAction() override{
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		synthesizer->GetUndoSystem()->Add(syneULinkAdd::Ref::New(synthesizer, syneLink::Ref::New()));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSynthesizer());
	}
};

class cActionLinkRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionLinkRemove> Ref;
	cActionLinkRemove(syneWPLink &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove link"){}
	
	igdeUndo::Ref OnAction(syneLink *link) override{
		return syneULinkRemove::Ref::New(link);
	}
};

class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(syneWPLink &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, syneLink *link) override{
		return textField->GetText() != link->GetName() ? syneULinkSetName::Ref::New(link, textField->GetText()) : syneULinkSetName::Ref();
	}
};

class cComboController : public igdeComboBoxListener{
	syneWPLink &pPanel;
	
public:
	typedef deTObjectReference<cComboController> Ref;
	cComboController(syneWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		syneLink * const link = pPanel.GetLink();
		syneController * const controller = comboBox->GetSelectedItem()
			? (syneController*)comboBox->GetSelectedItem()->GetData() : nullptr;
		if(!link || controller == link->GetController() || pPanel.GetPreventUpdate()){
			return;
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(
			syneULinkSetController::Ref::New(link, controller));
	}
};

class cSpinRepeat : public igdeSpinTextFieldListener{
	syneWPLink &pPanel;
	
public:
	typedef deTObjectReference<cSpinRepeat> Ref;
	cSpinRepeat(syneWPLink &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		const int value = textField->GetValue();
		syneLink * const link = pPanel.GetLink();
		if(!link || value == link->GetRepeat()){
			return;
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(syneULinkSetRepeat::Ref::New(link, value));
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	syneWPLink &pPanel;
	igdeUndo::Ref pUndo;
	
public:
	typedef deTObjectReference<cEditCurve> Ref;
	cEditCurve(syneWPLink &panel) : pPanel(panel){}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((syneULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo = syneULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(pUndo);
		pUndo = nullptr;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((syneULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo = syneULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
	}
};

}



// Class syneWPLink
///////////////////

// Constructor, destructor
////////////////////////////

syneWPLink::syneWPLink(syneViewSynthesizer &viewSynthesizer) :
igdeContainerScroll(viewSynthesizer.GetEnvironment(), false, true),
pViewSynthesizer(viewSynthesizer),
pPreventUpdate(false)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = syneWPLinkListener::Ref::New(*this);
	
	
	pActionLinkAdd = cActionLinkAdd::Ref::New(*this);
	pActionLinkRemove = cActionLinkRemove::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Links:");
	helper.ListBox(groupBox, 10, "Links", pListLink, cListLinks::Ref::New(*this));
	pListLink->SetDefaultSorter();
	
	
	helper.GroupBoxStatic(content, groupBox, "Link Settings:");
	helper.EditString(groupBox, "Name:", "Name of link", pEditName, cTextName::Ref::New(*this));
	helper.ComboBox(groupBox, "Controller:", "Controller to query values from",
		pCBController, cComboController::Ref::New(*this));
	pCBController->SetDefaultSorter();
	helper.EditSpinInteger(groupBox, "Repeat:", "Repeat curve along X direction", 1, 99,
		pSpinRepeat, cSpinRepeat::Ref::New(*this));
	
	
	helper.GroupBoxFlow(content, groupBox, "Link Curve:");
	
	helper.ViewCurveBezier(groupBox, pEditCurve, cEditCurve::Ref::New(*this));
	pEditCurve->SetDefaultSize(decPoint(200, 250));
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp(true);
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
}

syneWPLink::~syneWPLink(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

void syneWPLink::SetSynthesizer(syneSynthesizer *synthesizer){
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
	UpdateLinkList();
}

syneLink * syneWPLink::GetLink() const{
	return pSynthesizer ? pSynthesizer->GetActiveLink() : nullptr;
}

void syneWPLink::SelectActiveLink(){
	pListLink->SetSelectionWithData(GetLink());
}

void syneWPLink::UpdateLinkList(){
	const syneLink::Ref selection(GetLink());
	
	pListLink->RemoveAllItems();
	
	if(pSynthesizer){
		pSynthesizer->GetLinks().Visit([&](syneLink *link){
			pListLink->AddItem(link->GetName(), nullptr, link);
		});
	}
	
	pListLink->SortItems();
	
	pListLink->SetSelectionWithData(selection);
	if(!pListLink->GetSelectedItem() && pListLink->GetItems().IsNotEmpty()){
		pListLink->SetSelection(0);
	}
}

void syneWPLink::UpdateLink(){
	const syneLink * const link = GetLink();
	
	if(link){
		pEditName->SetText(link->GetName());
		pCBController->SetSelectionWithData(link->GetController());
		pSpinRepeat->SetValue(link->GetRepeat());
		pEditCurve->SetCurve(link->GetCurve());
		
	}else{
		pEditName->ClearText();
		pCBController->SetSelectionWithData(nullptr);
		pSpinRepeat->SetValue(1);
		pEditCurve->ClearCurve();
	}
	
	const bool enabled = link;
	pEditName->SetEnabled(enabled);
	pCBController->SetEnabled(enabled);
	pSpinRepeat->SetEnabled(enabled);
	pEditCurve->SetEnabled(enabled);
}

void syneWPLink::UpdateControllerList(){
	syneController * const selection = pCBController->GetSelectedItem()
		? (syneController*)pCBController->GetSelectedItem()->GetData() : nullptr;
	
	pPreventUpdate = true; // required since changing list causes text changes
	
	try{
		pCBController->RemoveAllItems();
		pCBController->AddItem("< No Controller >", nullptr);
		
		if(pSynthesizer){
			pSynthesizer->GetControllers().Visit([&](syneController *controller){
				pCBController->AddItem(controller->GetName(), nullptr, controller);
			});
		}
		
		pCBController->SortItems();
		
		pCBController->SetSelectionWithData(selection);
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
	
	pPreventUpdate = false;
}
