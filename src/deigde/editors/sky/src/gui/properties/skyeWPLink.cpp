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

#include "skyeWPLink.h"
#include "skyeWPLinkListener.h"
#include "skyeWindowProperties.h"
#include "../../sky/skyeSky.h"
#include "../../sky/controller/skyeController.h"
#include "../../sky/link/skyeLink.h"
#include "../../sky/layer/skyeLayer.h"
#include "../../undosys/link/skyeULinkAdd.h"
#include "../../undosys/link/skyeULinkRemove.h"
#include "../../undosys/link/skyeULinkSetName.h"
#include "../../undosys/link/skyeULinkSetCurve.h"
#include "../../undosys/link/skyeULinkSetController.h"
#include "../../undosys/link/skyeULinkSetRepeat.h"

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
	skyeWPLink &pPanel;
	
public:
	cBaseTextFieldListener(skyeWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		skyeSky * const sky = pPanel.GetSky();
		skyeLink * const link = pPanel.GetLink();
		if(!sky || !link){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, sky, link));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky *sky, skyeLink *link) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	skyeWPLink &pPanel;
	
public:
	cBaseComboBoxListener(skyeWPLink &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		skyeSky * const sky = pPanel.GetSky();
		skyeLink * const link = pPanel.GetLink();
		if(!sky || !link){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, sky, link));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, skyeSky *sky, skyeLink *link) = 0;
};

class cBaseAction : public igdeAction{
protected:
	skyeWPLink &pPanel;
	
public:
	cBaseAction(skyeWPLink &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	cBaseAction(skyeWPLink &panel, igdeIcon *icon, const char *description) :
	igdeAction("", icon, description),
	pPanel(panel){}
	
	cBaseAction(skyeWPLink &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		skyeSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(sky));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(skyeSky *sky) = 0;
};

class cBaseActionLink : public cBaseAction{
public:
	cBaseActionLink(skyeWPLink &panel, const char *text, const char *description) :
	cBaseAction(panel, text, description){}
	
	cBaseActionLink(skyeWPLink &panel, igdeIcon *icon, const char *description) :
	cBaseAction(panel, icon, description){}
	
	cBaseActionLink(skyeWPLink &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(skyeSky *sky) override{
		skyeLink * const link = pPanel.GetLink();
		if(link){
			return OnActionLink(sky, link);
			
		}else{
			return {};
		}
	}
	
	virtual igdeUndo::Ref OnActionLink(skyeSky *sky, skyeLink *link) = 0;
};



class cListLinks : public igdeListBoxListener{
	skyeWPLink &pPanel;
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(skyeWPLink &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		skyeSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		
		if(selection){
			sky->SetActiveLink((skyeLink*)selection->GetData());
			
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
	cActionLinkAdd(skyeWPLink &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add a link to the end of the list."){}
	
	igdeUndo::Ref OnAction(skyeSky *sky) override{
		return skyeULinkAdd::Ref::New(sky, skyeLink::Ref::New());
	}
};

class cActionLinkRemove : public cBaseActionLink{
public:
	typedef deTObjectReference<cActionLinkRemove> Ref;
	cActionLinkRemove(skyeWPLink &panel) : cBaseActionLink(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove the selected link."){}
	
	virtual igdeUndo::Ref OnActionLink(skyeSky *sky, skyeLink *link){
		const int usageCount = sky->CountLinkUsage(link);
		
		if(usageCount > 0 && igdeCommonDialogs::QuestionFormat(
			&pPanel, igdeCommonDialogs::ebsYesNo, "Remove Link",
			"The link '%s' is used by %i targets.\n"
			"If the link is removed now it is also removed from\n"
			"all the targets using it. Do you want to remove the link?",
			link->GetName().GetString(), usageCount) != igdeCommonDialogs::ebYes){
				return {};
		}
		
		return skyeULinkRemove::Ref::New(link);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetLink() != nullptr);
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(skyeWPLink &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky*, skyeLink *link) override{
		const decString &name = textField->GetText();
		if(name == link->GetName()){
			return {};
		}
		return skyeULinkSetName::Ref::New(link, name);
	}
};

class cComboController : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboController> Ref;
	cComboController(skyeWPLink &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, skyeSky*, skyeLink *link) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		skyeController *controller = nullptr;
		if(selection){
			controller = (skyeController*)selection->GetData();
		}
		
		if(controller == link->GetController()){
			return {};
		}
		
		return skyeULinkSetController::Ref::New(link, controller);
	}
};

class cTextRepeat : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRepeat> Ref;
	cTextRepeat(skyeWPLink &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, skyeSky*, skyeLink *link) override{
		const int repeat = textField->GetInteger();
		if(repeat == link->GetRepeat()){
			return {};
		}
		return skyeULinkSetRepeat::Ref::New(link, repeat);
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
protected:
	skyeWPLink &pPanel;
	igdeUndo::Ref pUndo;
	
public:
	typedef deTObjectReference<cEditCurve> Ref;
	cEditCurve(skyeWPLink &panel) : pPanel(panel){}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((skyeULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo = skyeULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
		
		pPanel.GetSky()->GetUndoSystem()->Add(pUndo);
		pUndo = nullptr;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((skyeULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo = skyeULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
	}
};

}



// Class skyeWPLink
///////////////////

// Constructor, destructor
////////////////////////////

skyeWPLink::skyeWPLink(skyeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = skyeWPLinkListener::Ref::New(*this);
	
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

skyeWPLink::~skyeWPLink(){
	SetSky(nullptr);
}



// Management
///////////////

void skyeWPLink::SetSky(skyeSky *sky){
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

skyeLink *skyeWPLink::GetLink() const{
	return pSky ? pSky->GetActiveLink() : nullptr;
}



void skyeWPLink::UpdateLinkList(){
	const skyeLink::Ref selection(GetLink());
	
	pListLinks->RemoveAllItems();
	
	if(pSky){
		pSky->GetLinks().Visit([this](skyeLink *link){
			pListLinks->AddItem(link->GetName().GetString(), nullptr, link);
		});
		pListLinks->SortItems();
	}
	
	if(selection){
		pListLinks->SetSelectionWithData(selection);
	}
}

void skyeWPLink::SelectActiveLink(){
	const int selection = pListLinks->IndexOfItemWithData(GetLink());
	
	pListLinks->SetSelection(selection);
	if(selection != -1){
		pListLinks->MakeItemVisible(selection);
	}
	
	UpdateLink();
}

void skyeWPLink::UpdateLink(){
	skyeLink * const link = GetLink();
	
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

void skyeWPLink::UpdateControllerList(){
	pCBController->RemoveAllItems();
	pCBController->AddItem("< No Controller >", nullptr);
	
	if(pSky){
		pSky->GetControllers().Visit([&](skyeController *controller){
			pCBController->AddItem(controller->GetName(), nullptr, controller);
		});
		pCBController->SortItems();
	}
	
	skyeLink * const link = GetLink();
	if(link){
		pCBController->SetSelectionWithData(link->GetController());
		
	}else{
		pCBController->SetSelectionWithData(nullptr);
	}
}
