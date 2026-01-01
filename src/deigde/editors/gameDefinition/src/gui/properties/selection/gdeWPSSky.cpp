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

#include "gdeWPSSky.h"
#include "gdeWPSSkyListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"
#include "../../../gamedef/sky/gdeSky.h"
#include "../../../gamedef/sky/gdeSkyController.h"
#include "../../../undosys/sky/gdeUSkySetPath.h"
#include "../../../undosys/sky/gdeUSkySetName.h"
#include "../../../undosys/sky/gdeUSkySetDescription.h"
#include "../../../undosys/sky/gdeUSkySetCategory.h"
#include "../../../undosys/sky/controller/gdeUSkyControllerAdd.h"
#include "../../../undosys/sky/controller/gdeUSkyControllerRemove.h"
#include "../../../undosys/sky/controller/gdeUSkyControllerSetName.h"
#include "../../../undosys/sky/controller/gdeUSkyControllerSetValue.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
///////////

namespace{

class cEditPath : public igdeEditPathListener{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cEditPath> Ref;
	cEditPath(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetPath() == editPath->GetPath()){
			return;
		}
		
		if(pPanel.GetGameDefinition()->GetSkies().HasMatching([&](const gdeSky &s){
			return s.GetPath() == editPath->GetPath();
		})){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Change sky emitter path",
				"A sky emitter with this path exists already.");
			editPath->SetPath(sky->GetPath());
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetPath::Ref::New(sky, editPath->GetPath()));
	}
};

class cTextName : public igdeTextFieldListener{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetName() == textField->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetName::Ref::New(sky, textField->GetText()));
	}
};

class cTextDescription : public igdeTextAreaListener{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cTextDescription> Ref;
	cTextDescription(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetDescription() == textArea->GetDescription()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetDescription::Ref::New(sky, textArea->GetText()));
	}
};


class cActionControllerAdd : public igdeAction{
	gdeWPSSky &pPanel;
	igdeComboBox &pComboBox;
public:
	typedef deTObjectReference<cActionControllerAdd> Ref;
	cActionControllerAdd(gdeWPSSky &panel, igdeComboBox &comboBox) : 
	igdeAction("Add...", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add controller"),
	pPanel(panel), pComboBox(comboBox){}
	
	void OnAction() override{
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetTags().Has(pComboBox.GetText())){
			return;
		}
		
		const gdeSkyController::List &list = sky->GetControllers();
		decString name("Controller");
		
		while(igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Controller", "Name:", name)){
			if(list.HasMatching([&](const gdeSkyController &c){
				return c.GetName() == name;
			})){
				igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Add Controller", "Controller exists already.");
				continue;
			}
			
			const gdeSkyController::Ref controller(gdeSkyController::Ref::New(name, 0.0f));
			
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(
				gdeUSkyControllerAdd::Ref::New(sky, controller));
			
			pComboBox.SetSelection(pComboBox.IndexOfItem(name));
			pPanel.UpdateController();
			return;
		}
	}
};

class cActionControllerRemove : public igdeAction{
	gdeWPSSky &pPanel;
	igdeComboBox &pComboBox;
public:
	typedef deTObjectReference<cActionControllerRemove> Ref;
	cActionControllerRemove(gdeWPSSky &panel, igdeComboBox &comboBox) : 
	igdeAction("Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove controller"),
	pPanel(panel), pComboBox(comboBox){}
	
	void OnAction() override{
		gdeSkyController * const controller = pPanel.GetController();
		if(!controller){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkyControllerRemove::Ref::New(pPanel.GetSky(), controller));
		
		if(pComboBox.GetItems().IsNotEmpty()){
			pComboBox.SetSelection(0);
			pPanel.UpdateController();
		}
	}
};

class cActionControllerMenu : public igdeActionContextMenu{
	gdeWPSSky &pPanel;
public:
	typedef deTObjectReference<cActionControllerMenu> Ref;
	cActionControllerMenu(gdeWPSSky &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Controller menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionControllerAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionControllerRemove());
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSky() != nullptr);
	}
};

class cComboController : public igdeComboBoxListener{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cComboController> Ref;
	cComboController(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		pPanel.UpdateController();
	}
};

class cTextControllerName : public igdeTextFieldListener{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cTextControllerName> Ref;
	cTextControllerName(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeSkyController * const controller = pPanel.GetController();
		if(!controller || controller->GetName() == textField->GetText()){
			return;
		}
		
		if(pPanel.GetSky()->GetControllers().HasMatching([&](const gdeSkyController &c){
			return c.GetName() == textField->GetText();
		})){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Rename controller",
				"A controller with this name exists already.");
			textField->SetText(controller->GetName());
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUSkyControllerSetName::Ref::New(
			pPanel.GetSky(), controller, textField->GetText()));
	}
};

class cTextControllerValue : public igdeTextFieldListener{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cTextControllerValue> Ref;
	cTextControllerValue(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeSkyController * const controller = pPanel.GetController();
		const float value = textField->GetFloat();
		if(!controller || fabsf(value - controller->GetValue()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUSkyControllerSetValue::Ref::New(
			pPanel.GetSky(), controller, value));
	}
};


class cComboCategory : public igdeComboBoxListener{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cComboCategory> Ref;
	cComboCategory(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetCategory() == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetCategory::Ref::New(sky, comboBox->GetText()));
	}
};

class cActionJumpToCategory : public igdeAction{
	gdeWPSSky &pPanel;
	
public:
	typedef deTObjectReference<cActionJumpToCategory> Ref;
	cActionJumpToCategory(gdeWPSSky &panel) : 
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Category"),
	pPanel(panel){}
	
	void OnAction() override{
		gdeSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeCategory * const category = gameDefinition.GetCategoriesSky()
			.GetWithPath(sky->GetCategory());
		if(!category){
			return;
		}
		
		gameDefinition.SetActiveCategory(category);
		gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategorySky);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSky());
	}
};

}



// Class gdeWPSSky
////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSSky::gdeWPSSky(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSSkyListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// sky
	helper.GroupBox(content, groupBox, "Sky:");
	helper.EditPath(groupBox, "Path:", "Path to sky emitter",
		igdeEnvironment::efpltSky, pEditPath, cEditPath::Ref::New(*this));
	helper.EditString(groupBox, "Name:", "Sky name", pEditName, cTextName::Ref::New(*this));
	helper.EditString(groupBox, "Description:", "Sky description",
		pEditDescription, 15, 5, cTextDescription::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "Category: ", "Category", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Category", pCBCategory, cComboCategory::Ref::New(*this));
	pCBCategory->SetDefaultSorter();
	pCBCategory->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToCategory, cActionJumpToCategory::Ref::New(*this));
	
	// controller
	helper.GroupBox(content, groupBox, "Controllers:");
	
	helper.FormLineStretchFirst(groupBox, "Controller:", "Controller to edit", frameLine);
	helper.ComboBox(frameLine, "Controller to edit", pCBController, cComboController::Ref::New(*this));
	pActionControllerAdd = cActionControllerAdd::Ref::New(*this, pCBController);
	pActionControllerRemove = cActionControllerRemove::Ref::New(*this, pCBController);
	pActionControllerMenu = cActionControllerMenu::Ref::New(*this);
	helper.Button(frameLine, pActionControllerMenu);
	
	helper.EditString(groupBox, "Name:", "Controller name", pEditControllerName, cTextControllerName::Ref::New(*this));
	helper.EditFloat(groupBox, "Value:", "Controller value", pEditControllerValue, cTextControllerValue::Ref::New(*this));
}

gdeWPSSky::~gdeWPSSky(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSSky::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
	}
	
	UpdateSky();
	
	UpdateCategoryList();
}



gdeSky *gdeWPSSky::GetSky() const{
	return pGameDefinition ? pGameDefinition->GetActiveSky() : nullptr;
}

gdeSkyController *gdeWPSSky::GetController() const{
	const gdeSky * const sky = GetSky();
	if(!sky){
		return nullptr;
	}
	
	const decString &name = pCBController->GetSelectedItem()->GetText();
	return pCBController->GetSelectedItem()
		? sky->GetControllers().FindOrDefault([&](const gdeSkyController &c){
			return c.GetName() == name;
		}) : nullptr;
}



void gdeWPSSky::UpdateCategoryList(){
	const decString selection(pCBCategory->GetText());
	
	pCBCategory->RemoveAllItems();
	
	if(pGameDefinition){
		const gdeCategoryList &categories = pGameDefinition->GetCategoriesSky();
		if(categories.GetCount() > 0){
			UpdateCategoryList(categories, "");
		}
		
		pCBCategory->SortItems();
		pCBCategory->StoreFilterItems();
	}
	
	pCBCategory->SetText(selection);
	pCBCategory->SetInvalidValue(!pCBCategory->GetText().IsEmpty() && !pCBCategory->GetSelectedItem());
}

void gdeWPSSky::UpdateCategoryList(const gdeCategoryList &list, const char *prefix){
	const int count = list.GetCount();
	decString text;
	int i;
	
	for(i=0; i<count; i++){
		const gdeCategory &category = *list.GetAt(i);
		text.Format("%s%s", prefix, category.GetName().GetString());
		pCBCategory->AddItem(text);
		if(category.GetCategories().GetCount() > 0){
			text.AppendCharacter('/');
			UpdateCategoryList(category.GetCategories(), text);
		}
	}
}



void gdeWPSSky::UpdateSky(){
	const gdeSky * const sky = GetSky();
	
	if(sky){
		pEditPath->SetPath(sky->GetPath());
		pEditName->SetText(sky->GetName());
		pEditDescription->SetText(sky->GetDescription());
		pCBCategory->SetText(sky->GetCategory());
		pCBCategory->SetInvalidValue(!pCBCategory->GetText().IsEmpty()
			&& !pCBCategory->GetSelectedItem());
		
	}else{
		pEditPath->ClearPath();
		pEditName->ClearText();
		pEditDescription->ClearText();
		pCBCategory->ClearText();
		pCBCategory->SetInvalidValue(false);
	}
	
	const bool enabled = sky != nullptr;
	pEditPath->SetEnabled(enabled);
	pEditName->SetEnabled(enabled);
	pEditDescription->SetEnabled(enabled);
	pCBCategory->SetEnabled(enabled);
	
	UpdateController();
}

void gdeWPSSky::UpdateController(){
	const gdeSkyController * selectedController = GetController();
	const gdeSky * const sky = GetSky();
	
	pCBController->RemoveAllItems();
	
	if(sky){
		const gdeSkyController::List &controllers = sky->GetControllers();
		const int count = controllers.GetCount();
		int i;
		for(i=0; i<count; i++){
			pCBController->AddItem(controllers.GetAt(i)->GetName());
		}
		pCBController->SortItems();
		
		if(!selectedController && pCBController->GetItems().IsNotEmpty()){
			const decString &name = pCBController->GetItems().First()->GetText();
			selectedController = controllers.FindOrDefault([&](const gdeSkyController &c){
				return c.GetName() == name;
			});
		}
	}
	
	if(selectedController){
		pCBController->SetSelection(pCBController->IndexOfItem(selectedController->GetName()));
	}
	
	if(selectedController){
		pEditControllerName->SetText(selectedController->GetName());
		pEditControllerValue->SetFloat(selectedController->GetValue());
		
	}else{
		pEditControllerName->ClearText();
		pEditControllerValue->ClearText();
	}
	
	const bool enabled = selectedController != nullptr;
	pEditControllerName->SetEnabled(enabled);
	pEditControllerValue->SetEnabled(enabled);
}
