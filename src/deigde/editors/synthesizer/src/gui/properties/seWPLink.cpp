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

#include "seWPLink.h"
#include "seWPLinkListener.h"
#include "../seWindowMain.h"
#include "../seViewSynthesizer.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/controller/seController.h"
#include "../../synthesizer/link/seLink.h"
#include "../../undosys/link/seULinkSetName.h"
#include "../../undosys/link/seULinkSetController.h"
#include "../../undosys/link/seULinkSetCurve.h"
#include "../../undosys/link/seULinkAdd.h"
#include "../../undosys/link/seULinkRemove.h"
#include "../../undosys/link/seULinkSetRepeat.h"

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
	seWPLink &pPanel;
	
public:
	cBaseTextFieldListener(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seLink * const link = pPanel.GetLink();
		if(!link){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, link)));
		if(undo){
			link->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seLink *link) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPLink &pPanel;
	
public:
	cBaseAction(seWPLink &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		seLink * const link = pPanel.GetLink();
		if(!link){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(link)));
		if(undo){
			link->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(seLink *link) = 0;
};



class cListLinks : public igdeListBoxListener{
	seWPLink &pPanel;
	
public:
	cListLinks(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		synthesizer->SetActiveLink(listBox->GetSelectedItem()
			? (seLink*)listBox->GetSelectedItem()->GetData() : NULL);
		pPanel.UpdateLink();
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionLinkAdd());
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};


class cActionLinkAdd : public igdeAction{
	seWPLink &pPanel;
	
public:
	cActionLinkAdd(seWPLink &panel) : igdeAction("Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add link"),
	pPanel(panel){}
	
	virtual void OnAction(){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		synthesizer->GetUndoSystem()->Add(seULinkAdd::Ref::NewWith(synthesizer, seLink::Ref::NewWith()));
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetSynthesizer());
	}
};

class cActionLinkRemove : public cBaseAction{
public:
	cActionLinkRemove(seWPLink &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove link"){}
	
	virtual igdeUndo *OnAction(seLink *link){
		return new seULinkRemove(link);
	}
};

class cTextName : public cBaseTextFieldListener{
public:
	cTextName(seWPLink &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, seLink *link){
		return textField->GetText() != link->GetName() ? new seULinkSetName(link, textField->GetText()) : NULL;
	}
};

class cComboController : public igdeComboBoxListener{
	seWPLink &pPanel;
	
public:
	cComboController(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seLink * const link = pPanel.GetLink();
		seController * const controller = comboBox->GetSelectedItem()
			? (seController*)comboBox->GetSelectedItem()->GetData() : NULL;
		if(!link || controller == link->GetController() || pPanel.GetPreventUpdate()){
			return;
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(
			seULinkSetController::Ref::NewWith(link, controller));
	}
};

class cSpinRepeat : public igdeSpinTextFieldListener{
	seWPLink &pPanel;
	
public:
	cSpinRepeat(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		const int value = textField->GetValue();
		seLink * const link = pPanel.GetLink();
		if(!link || value == link->GetRepeat()){
			return;
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(seULinkSetRepeat::Ref::NewWith(link, value));
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	seWPLink &pPanel;
	igdeUndo::Ref pUndo;
	
public:
	cEditCurve(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo.TakeOverWith(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(pUndo);
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo.TakeOverWith(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
	}
};

}



// Class seWPLink
///////////////////

// Constructor, destructor
////////////////////////////

seWPLink::seWPLink(seViewSynthesizer &viewSynthesizer) :
igdeContainerScroll(viewSynthesizer.GetEnvironment(), false, true),
pViewSynthesizer(viewSynthesizer),
pListener(NULL),
pSynthesizer(NULL),
pPreventUpdate(false)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener.TakeOverWith(*this);
	
	
	pActionLinkAdd.TakeOverWith(*this);
	pActionLinkRemove.TakeOverWith(*this);
	
	
	content.TakeOverWith(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Links:");
	helper.ListBox(groupBox, 10, "Links", pListLink, new cListLinks(*this));
	pListLink->SetDefaultSorter();
	
	
	helper.GroupBoxStatic(content, groupBox, "Link Settings:");
	helper.EditString(groupBox, "Name:", "Name of link", pEditName, new cTextName(*this));
	helper.ComboBox(groupBox, "Controller:", "Controller to query values from",
		pCBController, new cComboController(*this));
	pCBController->SetDefaultSorter();
	helper.EditSpinInteger(groupBox, "Repeat:", "Repeat curve along X direction", 1, 99,
		pSpinRepeat, new cSpinRepeat(*this));
	
	
	helper.GroupBoxFlow(content, groupBox, "Link Curve:");
	
	helper.ViewCurveBezier(groupBox, pEditCurve, new cEditCurve(*this));
	pEditCurve->SetDefaultSize(decPoint(200, 250));
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp(true);
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
}

seWPLink::~seWPLink(){
	SetSynthesizer(NULL);
}



// Management
///////////////

void seWPLink::SetSynthesizer(seSynthesizer *synthesizer){
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
	UpdateLinkList();
}

seLink * seWPLink::GetLink() const{
	return pSynthesizer ? pSynthesizer->GetActiveLink() : NULL;
}

void seWPLink::SelectActiveLink(){
	pListLink->SetSelectionWithData(GetLink());
}

void seWPLink::UpdateLinkList(){
	const seLink::Ref selection(GetLink());
	
	pListLink->RemoveAllItems();
	
	if(pSynthesizer){
		const seLinkList &list = pSynthesizer->GetLinks();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			seLink * const link = pSynthesizer->GetLinks().GetAt(i);
			pListLink->AddItem(link->GetName(), NULL, link);
		}
	}
	
	pListLink->SortItems();
	
	pListLink->SetSelectionWithData(selection);
	if(!pListLink->GetSelectedItem() && pListLink->GetItemCount() > 0){
		pListLink->SetSelection(0);
	}
}

void seWPLink::UpdateLink(){
	const seLink * const link = GetLink();
	
	if(link){
		pEditName->SetText(link->GetName());
		pCBController->SetSelectionWithData(link->GetController());
		pSpinRepeat->SetValue(link->GetRepeat());
		pEditCurve->SetCurve(link->GetCurve());
		
	}else{
		pEditName->ClearText();
		pCBController->SetSelectionWithData(NULL);
		pSpinRepeat->SetValue(1);
		pEditCurve->ClearCurve();
	}
	
	const bool enabled = link;
	pEditName->SetEnabled(enabled);
	pCBController->SetEnabled(enabled);
	pSpinRepeat->SetEnabled(enabled);
	pEditCurve->SetEnabled(enabled);
}

void seWPLink::UpdateControllerList(){
	seController * const selection = pCBController->GetSelectedItem()
		? (seController*)pCBController->GetSelectedItem()->GetData() : NULL;
	
	pPreventUpdate = true; // required since changing list causes text changes
	
	try{
		pCBController->RemoveAllItems();
		pCBController->AddItem("< No Controller >", NULL);
		
		if(pSynthesizer){
			const seControllerList &list = pSynthesizer->GetControllers();
			const int count = list.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				seController * const controller = list.GetAt(i);
				pCBController->AddItem(controller->GetName(), NULL, controller);
			}
		}
		
		pCBController->SortItems();
		
		pCBController->SetSelectionWithData(selection);
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
	
	pPreventUpdate = false;
}
