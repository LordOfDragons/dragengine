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

class cEditPath : public igdeEditPathListener {
	gdeWPSSky &pPanel;
	
public:
	cEditPath(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetPath() == editPath->GetPath()){
			return;
		}
		
		if(pPanel.GetGameDefinition()->GetSkies().HasWithPath(editPath->GetPath())){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Change sky emitter path",
				"A sky emitter with this path exists already.");
			editPath->SetPath(sky->GetPath());
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetPath::Ref::NewWith(sky, editPath->GetPath()));
	}
};

class cTextName : public igdeTextFieldListener {
	gdeWPSSky &pPanel;
	
public:
	cTextName(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetName() == textField->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetName::Ref::NewWith(sky, textField->GetText()));
	}
};

class cTextDescription : public igdeTextAreaListener {
	gdeWPSSky &pPanel;
	
public:
	cTextDescription(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetDescription() == textArea->GetDescription()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetDescription::Ref::NewWith(sky, textArea->GetText()));
	}
};


class cActionControllerAdd : public igdeAction {
	gdeWPSSky &pPanel;
	igdeComboBox &pComboBox;
public:
	cActionControllerAdd(gdeWPSSky &panel, igdeComboBox &comboBox) : 
	igdeAction("Add...", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add controller"),
	pPanel(panel), pComboBox(comboBox){}
	
	virtual void OnAction(){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetTags().Has(pComboBox.GetText())){
			return;
		}
		
		const gdeSkyControllerList &list = sky->GetControllers();
		decString name("Controller");
		
		while(igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Add Controller", "Name:", name)){
			if(list.HasNamed(name)){
				igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Add Controller", "Controller exists already.");
				continue;
			}
			
			const gdeSkyController::Ref controller(gdeSkyController::Ref::NewWith(name, 0.0f));
			
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(
				gdeUSkyControllerAdd::Ref::NewWith(sky, controller));
			
			pComboBox.SetSelection(pComboBox.IndexOfItem(name));
			pPanel.UpdateController();
			return;
		}
	}
};

class cActionControllerRemove : public igdeAction {
	gdeWPSSky &pPanel;
	igdeComboBox &pComboBox;
public:
	cActionControllerRemove(gdeWPSSky &panel, igdeComboBox &comboBox) : 
	igdeAction("Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove controller"),
	pPanel(panel), pComboBox(comboBox){}
	
	virtual void OnAction(){
		gdeSkyController * const controller = pPanel.GetController();
		if(!controller){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkyControllerRemove::Ref::NewWith(pPanel.GetSky(), controller));
		
		if(pComboBox.GetItemCount() > 0){
			pComboBox.SetSelection(0);
			pPanel.UpdateController();
		}
	}
};

class cActionControllerMenu : public igdeActionContextMenu {
	gdeWPSSky &pPanel;
public:
	cActionControllerMenu(gdeWPSSky &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Controller menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionControllerAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionControllerRemove());
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetSky() != NULL);
	}
};

class cComboController : public igdeComboBoxListener {
	gdeWPSSky &pPanel;
	
public:
	cComboController(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		pPanel.UpdateController();
	}
};

class cTextControllerName : public igdeTextFieldListener {
	gdeWPSSky &pPanel;
	
public:
	cTextControllerName(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeSkyController * const controller = pPanel.GetController();
		if(!controller || controller->GetName() == textField->GetText()){
			return;
		}
		
		if(pPanel.GetSky()->GetControllers().HasNamed(textField->GetText())){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Rename controller",
				"A controller with this name exists already.");
			textField->SetText(controller->GetName());
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUSkyControllerSetName::Ref::NewWith(
			pPanel.GetSky(), controller, textField->GetText()));
	}
};

class cTextControllerValue : public igdeTextFieldListener {
	gdeWPSSky &pPanel;
	
public:
	cTextControllerValue(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeSkyController * const controller = pPanel.GetController();
		const float value = textField->GetFloat();
		if(!controller || fabsf(value - controller->GetValue()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUSkyControllerSetValue::Ref::NewWith(
			pPanel.GetSky(), controller, value));
	}
};


class cComboCategory : public igdeComboBoxListener {
	gdeWPSSky &pPanel;
	
public:
	cComboCategory(gdeWPSSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeSky * const sky = pPanel.GetSky();
		if(!sky || sky->GetCategory() == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkySetCategory::Ref::NewWith(sky, comboBox->GetText()));
	}
};

class cActionJumpToCategory : public igdeAction {
	gdeWPSSky &pPanel;
	
public:
	cActionJumpToCategory(gdeWPSSky &panel) : 
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Category"),
	pPanel(panel){}
	
	virtual void OnAction(){
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
	
	virtual void Update(){
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
pWindowProperties(windowProperties),
pListener(NULL),
pGameDefinition(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener.TakeOver(new gdeWPSSkyListener(*this));
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	// sky
	helper.GroupBox(content, groupBox, "Sky:");
	helper.EditPath(groupBox, "Path:", "Path to sky emitter",
		igdeEnvironment::efpltSky, pEditPath, new cEditPath(*this));
	helper.EditString(groupBox, "Name:", "Sky name", pEditName, new cTextName(*this));
	helper.EditString(groupBox, "Description:", "Sky description",
		pEditDescription, 15, 5, new cTextDescription(*this));
	
	helper.FormLineStretchFirst(groupBox, "Category: ", "Category", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Category", pCBCategory, new cComboCategory(*this));
	pCBCategory->SetDefaultSorter();
	pCBCategory->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToCategory, new cActionJumpToCategory(*this), true);
	
	// controller
	helper.GroupBox(content, groupBox, "Controllers:");
	
	helper.FormLineStretchFirst(groupBox, "Controller:", "Controller to edit", frameLine);
	helper.ComboBox(frameLine, "Controller to edit", pCBController, new cComboController(*this));
	pActionControllerAdd.TakeOver(new cActionControllerAdd(*this, pCBController));
	pActionControllerRemove.TakeOver(new cActionControllerRemove(*this, pCBController));
	pActionControllerMenu.TakeOver(new cActionControllerMenu(*this));
	helper.Button(frameLine, pActionControllerMenu);
	
	helper.EditString(groupBox, "Name:", "Controller name", pEditControllerName,
		new cTextControllerName(*this));
	helper.EditFloat(groupBox, "Value:", "Controller value", pEditControllerValue,
		new cTextControllerValue(*this));
}

gdeWPSSky::~gdeWPSSky(){
	SetGameDefinition(NULL);
	
}



// Management
///////////////

void gdeWPSSky::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(pGameDefinition == gameDefinition){
		return;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		gameDefinition->AddReference();
	}
	
	UpdateSky();
	
	UpdateCategoryList();
}



gdeSky *gdeWPSSky::GetSky() const{
	return pGameDefinition ? pGameDefinition->GetActiveSky() : NULL;
}

gdeSkyController *gdeWPSSky::GetController() const{
	const gdeSky * const sky = GetSky();
	if(!sky){
		return NULL;
	}
	
	return pCBController->GetSelectedItem()
		? sky->GetControllers().GetNamed(pCBController->GetSelectedItem()->GetText()) : NULL;
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
	
	const bool enabled = sky != NULL;
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
		const gdeSkyControllerList &controllers = sky->GetControllers();
		const int count = controllers.GetCount();
		int i;
		for(i=0; i<count; i++){
			pCBController->AddItem(controllers.GetAt(i)->GetName());
		}
		pCBController->SortItems();
		
		if(!selectedController && pCBController->GetItemCount() > 0){
			selectedController = controllers.GetNamed(pCBController->GetItemAt(0)->GetText());
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
	
	const bool enabled = selectedController != NULL;
	pEditControllerName->SetEnabled(enabled);
	pEditControllerValue->SetEnabled(enabled);
}
