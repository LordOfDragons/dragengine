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
#include "seWindowProperties.h"
#include "../../sky/seSky.h"
#include "../../sky/controller/seController.h"
#include "../../sky/link/seLink.h"
#include "../../sky/layer/seLayer.h"
#include "../../undosys/link/seULinkAdd.h"
#include "../../undosys/link/seULinkRemove.h"
#include "../../undosys/link/seULinkSetName.h"
#include "../../undosys/link/seULinkSetCurve.h"
#include "../../undosys/link/seULinkSetController.h"
#include "../../undosys/link/seULinkSetRepeat.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPLink &pPanel;
	
public:
	cBaseTextFieldListener(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSky * const sky = pPanel.GetSky();
		seLink * const link = pPanel.GetLink();
		if(!sky || !link){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, sky, link));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seSky *sky, seLink *link) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPLink &pPanel;
	
public:
	cBaseComboBoxListener(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seSky * const sky = pPanel.GetSky();
		seLink * const link = pPanel.GetLink();
		if(!sky || !link){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, sky, link));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seSky *sky, seLink *link) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPLink &pPanel;
	
public:
	cBaseAction(seWPLink &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	cBaseAction(seWPLink &panel, igdeIcon *icon, const char *description) :
	igdeAction("", icon, description),
	pPanel(panel){}
	
	cBaseAction(seWPLink &panel, const char *text, igdeIcon *icon, const char *description) :
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

class cBaseActionLink : public cBaseAction{
public:
	cBaseActionLink(seWPLink &panel, const char *text, const char *description) :
	cBaseAction(panel, text, description){}
	
	cBaseActionLink(seWPLink &panel, igdeIcon *icon, const char *description) :
	cBaseAction(panel, icon, description){}
	
	cBaseActionLink(seWPLink &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(seSky *sky) override{
		seLink * const link = pPanel.GetLink();
		if(link){
			return OnActionLink(sky, link);
			
		}else{
			return {};
		}
	}
	
	virtual igdeUndo::Ref OnActionLink(seSky *sky, seLink *link) = 0;
};



class cListLinks : public igdeListBoxListener{
	seWPLink &pPanel;
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		
		if(selection){
			sky->SetActiveLink((seLink*)selection->GetData());
			
		}else{
			sky->SetActiveLink(nullptr);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand(menu, pPanel.GetActionLinkAdd());
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};

class cActionLinkAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionLinkAdd> Ref;
	cActionLinkAdd(seWPLink &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a link to the end of the list."){}
	
	igdeUndo::Ref OnAction(seSky *sky) override{
		return seULinkAdd::Ref::New(sky, seLink::Ref::New());
	}
};

class cActionLinkRemove : public cBaseActionLink{
public:
	typedef deTObjectReference<cActionLinkRemove> Ref;
	cActionLinkRemove(seWPLink &panel) : cBaseActionLink(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove the selected link."){}
	
	virtual igdeUndo::Ref OnActionLink(seSky *sky, seLink *link){
		const int usageCount = sky->CountLinkUsage(link);
		
		if(usageCount > 0 && igdeCommonDialogs::QuestionFormat(
			&pPanel, igdeCommonDialogs::ebsYesNo, "Remove Link",
			"The link '%s' is used by %i targets.\n"
			"If the link is removed now it is also removed from\n"
			"all the targets using it. Do you want to remove the link?",
			link->GetName().GetString(), usageCount) != igdeCommonDialogs::ebYes){
				return {};
		}
		
		return seULinkRemove::Ref::New(link);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetLink() != nullptr);
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(seWPLink &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seLink *link) override{
		const decString &name = textField->GetText();
		if(name == link->GetName()){
			return {};
		}
		return seULinkSetName::Ref::New(link, name);
	}
};

class cComboController : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboController> Ref;
	cComboController(seWPLink &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seSky*, seLink *link) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		seController *controller = nullptr;
		if(selection){
			controller = (seController*)selection->GetData();
		}
		
		if(controller == link->GetController()){
			return {};
		}
		
		return seULinkSetController::Ref::New(link, controller);
	}
};

class cTextRepeat : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRepeat> Ref;
	cTextRepeat(seWPLink &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSky*, seLink *link) override{
		const int repeat = textField->GetInteger();
		if(repeat == link->GetRepeat()){
			return {};
		}
		return seULinkSetRepeat::Ref::New(link, repeat);
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
protected:
	seWPLink &pPanel;
	igdeUndo::Ref pUndo;
	
public:
	typedef deTObjectReference<cEditCurve> Ref;
	cEditCurve(seWPLink &panel) : pPanel(panel){}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo = seULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
		
		pPanel.GetSky()->GetUndoSystem()->Add(pUndo);
		pUndo = nullptr;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo = seULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
	}
};

}



// Class seWPLink
///////////////////

// Constructor, destructor
////////////////////////////

seWPLink::seWPLink(seWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = seWPLinkListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// link list
	helper.GroupBoxFlow(content, groupBox, "Links:");
	
	helper.ListBox(groupBox, 8, "Links", pListLinks, cListLinks::Ref::New(*this));
	
	pActionLinkAdd = cActionLinkAdd::Ref::New(*this);
	pActionLinkRemove = cActionLinkRemove::Ref::New(*this);
	
	// link settings
	helper.GroupBoxFlow(content, groupBox, "Link Settings:");
	
	frameLine = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(frameLine);
	
	helper.EditString(frameLine, "Name:", "Name of the link", pEditName, cTextName::Ref::New(*this));
	
	helper.ComboBox(frameLine, "Link:", true, "Name of the link if used", pCBController, cComboController::Ref::New(*this));
	pCBController->SetDefaultSorter();
	
	helper.EditInteger(frameLine, "Repeat:", "Repeat link value", pEditRepeat, cTextRepeat::Ref::New(*this));
	
	helper.ViewCurveBezier(groupBox, pEditCurve, cEditCurve::Ref::New(*this));
}

seWPLink::~seWPLink(){
	SetSky(nullptr);
}



// Management
///////////////

void seWPLink::SetSky(seSky *sky){
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
	UpdateLinkList();
}

seLink *seWPLink::GetLink() const{
	return pSky ? pSky->GetActiveLink() : nullptr;
}



void seWPLink::UpdateLinkList(){
	const seLink::Ref selection(GetLink());
	
	pListLinks->RemoveAllItems();
	
	if(pSky){
		pSky->GetLinks().Visit([this](seLink *link){
			pListLinks->AddItem(link->GetName().GetString(), nullptr, link);
		});
		pListLinks->SortItems();
	}
	
	if(selection){
		pListLinks->SetSelectionWithData(selection);
	}
}

void seWPLink::SelectActiveLink(){
	const int selection = pListLinks->IndexOfItemWithData(GetLink());
	
	pListLinks->SetSelection(selection);
	if(selection != -1){
		pListLinks->MakeItemVisible(selection);
	}
	
	UpdateLink();
}

void seWPLink::UpdateLink(){
	seLink * const link = GetLink();
	
	if(link){
		pEditName->SetText(link->GetName());
		pCBController->SetSelectionWithData(link->GetController());
		pEditRepeat->SetInteger(link->GetRepeat());
		pEditCurve->SetCurve(link->GetCurve());
		
	}else{
		pEditName->ClearText();
		pCBController->SetSelectionWithData(nullptr);
		pEditRepeat->ClearText();
		pEditCurve->SetDefaultBezier();
	}
	
	const bool enabled = link != nullptr;
	pEditName->SetEnabled(enabled);
	pCBController->SetEnabled(enabled);
	pEditRepeat->SetEnabled(enabled);
	pEditCurve->SetEnabled(enabled);
}

void seWPLink::UpdateControllerList(){
	pCBController->RemoveAllItems();
	pCBController->AddItem("< No Controller >", nullptr);
	
	if(pSky){
		pSky->GetControllers().Visit([&](seController *controller){
			pCBController->AddItem(controller->GetName(), nullptr, controller);
		});
		pCBController->SortItems();
	}
	
	seLink * const link = GetLink();
	if(link){
		pCBController->SetSelectionWithData(link->GetController());
		
	}else{
		pCBController->SetSelectionWithData(nullptr);
	}
}
