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

#include "igdeDialogProjectSettings.h"
#include "igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../configuration/igdeConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Actions
////////////

class igdeDialogProjectSettings_ListPathGameDefBase : public igdeListBoxListener {
	igdeDialogProjectSettings &pDialog;
	
public:
	typedef deTObjectReference<igdeDialogProjectSettings_ListPathGameDefBase> Ref;
	
	explicit igdeDialogProjectSettings_ListPathGameDefBase(igdeDialogProjectSettings& dialog) :
	pDialog(dialog){}
	
	void OnSelectionChanged(igdeListBox*) override{
		pDialog.UpdateBaseGameDefButtons();
	}
};

class igdeDialogProjectSettings_ComboSharedGameDef : public igdeComboBoxListener {
	igdeDialogProjectSettings &pDialog;
	
public:
	typedef deTObjectReference<igdeDialogProjectSettings_ComboSharedGameDef> Ref;
	
	explicit igdeDialogProjectSettings_ComboSharedGameDef(igdeDialogProjectSettings& dialog) :
	pDialog(dialog){}
	
	void OnTextChanged(igdeComboBox*) override{
		pDialog.UpdateSharedGameDef();
	}
};

class igdeDialogProjectSettings_ActionGameDefBaseAdd : public igdeAction {
	igdeDialogProjectSettings &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	typedef deTObjectReference<igdeDialogProjectSettings_ActionGameDefBaseAdd> Ref;
	
	igdeDialogProjectSettings_ActionGameDefBaseAdd(
		igdeDialogProjectSettings& dialog, igdeListBox &listPathGameDefBase) :
	igdeAction("", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"Add base game definition"), pDialog(dialog), pListPathGameDefBase(listPathGameDefBase){}
	
	void OnAction() override{
		igdeGameDefinition * const sharedGameDef = pDialog.GetSelectedSharedGameDef();
		if(!sharedGameDef){
			return;
		}
		
		pListPathGameDefBase.AddItem(sharedGameDef->GetID());
		pListPathGameDefBase.SetSelection(pListPathGameDefBase.IndexOfItem(sharedGameDef->GetID()));
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	void Update() override{
		SetEnabled(pDialog.GetSelectedSharedGameDef()
			&& !pListPathGameDefBase.HasItem(pDialog.GetSelectedSharedGameDef()->GetID()));
	}
};

class igdeDialogProjectSettings_ActionGameDefBaseRemove : public igdeAction {
	igdeDialogProjectSettings &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	typedef deTObjectReference<igdeDialogProjectSettings_ActionGameDefBaseRemove> Ref;
	
	igdeDialogProjectSettings_ActionGameDefBaseRemove(
		igdeDialogProjectSettings& dialog, igdeListBox &listPathGameDefBase) :
	igdeAction("", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus),
		"Remove base game definition"), pDialog(dialog), pListPathGameDefBase(listPathGameDefBase){}
	
	void OnAction() override{
		const int selection = pListPathGameDefBase.GetSelection();
		if(selection == -1){
			return;
		}
		
		pListPathGameDefBase.RemoveItem(selection);
		if(!pListPathGameDefBase.GetSelectedItem() && pListPathGameDefBase.GetItemCount() > 0){
			pListPathGameDefBase.SetSelection(0);
		}
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	void Update() override{
		SetEnabled(pListPathGameDefBase.GetSelectedItem());
	}
};

class igdeDialogProjectSettings_ActionGameDefBaseUp : public igdeAction {
	igdeDialogProjectSettings &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	typedef deTObjectReference<igdeDialogProjectSettings_ActionGameDefBaseUp> Ref;
	
	igdeDialogProjectSettings_ActionGameDefBaseUp(
		igdeDialogProjectSettings& dialog, igdeListBox &listPathGameDefBase) :
	igdeAction("", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallUp),
		"Move base game definition up"), pDialog(dialog), pListPathGameDefBase(listPathGameDefBase){}
	
	void OnAction() override{
		const int selection = pListPathGameDefBase.GetSelection();
		if(selection < 1){
			return;
		}
		
		pListPathGameDefBase.MoveItem(selection, selection - 1);
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	void Update() override{
		SetEnabled(pListPathGameDefBase.GetSelection() > 0);
	}
};

class igdeDialogProjectSettings_ActionGameDefBaseDown : public igdeAction {
	igdeDialogProjectSettings &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	typedef deTObjectReference<igdeDialogProjectSettings_ActionGameDefBaseDown> Ref;
	
	igdeDialogProjectSettings_ActionGameDefBaseDown(
		igdeDialogProjectSettings& dialog, igdeListBox &listPathGameDefBase) :
	igdeAction("", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Move base game definition down"), pDialog(dialog), pListPathGameDefBase(listPathGameDefBase){}
	
	void OnAction() override{
		const int selection = pListPathGameDefBase.GetSelection();
		const int count = pListPathGameDefBase.GetItemCount();
		if(selection == -1 || selection >= count - 1){
			return;
		}
		
		pListPathGameDefBase.MoveItem(selection, selection + 1);
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	void Update() override{
		SetEnabled(pListPathGameDefBase.GetSelection() != -1
			&& pListPathGameDefBase.GetSelection() < pListPathGameDefBase.GetItemCount() - 1);
	}
};


class igdeDialogProjectSettings_ComboScriptModule : public igdeComboBoxListener {
	igdeDialogProjectSettings &pDialog;
	
public:
	typedef deTObjectReference<igdeDialogProjectSettings_ComboScriptModule> Ref;
	
	explicit igdeDialogProjectSettings_ComboScriptModule(igdeDialogProjectSettings& dialog) :
	pDialog(dialog){}
	
	void OnTextChanged(igdeComboBox*) override{
		pDialog.UpdateSharedGameDefs();
	}
};



// Class igdeDialogProjectSettings
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogProjectSettings::igdeDialogProjectSettings(igdeWindowMain &windowMain) :
igdeDialog(windowMain.GetEnvironment(), "Game Project Settings"),
pWindowMain(windowMain),
pBaseGameDefsChanged(false)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref content, panel;
	
	content = igdeContainerForm::Ref::New(env);
	
	helper.EditString(content, "Name:", "Name of the game project.", 60, pEditName, {});
	helper.EditString(content, "Description:",
		"Description of the game project.", pEditDescription, 5, {});
	helper.EditString(content, "Data Directory:",
		"Data directory relative to project directory.", pEditPathData, {});
	helper.EditString(content, "Cache Directory:",
		"Cache directory relative to project directory.", pEditPathCache, {});
	
	helper.ListBox(content, "Base Game Definitions:", 3,
		"Game definitions to use as base for the project.", pListPathGameDefBase, {});
	
	helper.FormLineStretchFirst(content, "", "Available game definitions to add to project", panel);
	
	helper.ComboBox(panel, "Available game definitions to add to project",
		pCBSharedGameDefs, igdeDialogProjectSettings_ComboSharedGameDef::Ref::New(*this));
	pCBSharedGameDefs->SetDefaultSorter();
	
	helper.Button(panel, pBtnPathGameDefBaseAdd, igdeDialogProjectSettings_ActionGameDefBaseAdd::Ref::New(*this, pListPathGameDefBase));
	helper.Button(panel, pBtnPathGameDefBaseRemove, igdeDialogProjectSettings_ActionGameDefBaseRemove::Ref::New(*this, pListPathGameDefBase));
	helper.Button(panel, pBtnPathGameDefBaseUp, igdeDialogProjectSettings_ActionGameDefBaseUp::Ref::New(*this, pListPathGameDefBase));
	helper.Button(panel, pBtnPathGameDefBaseDown, igdeDialogProjectSettings_ActionGameDefBaseDown::Ref::New(*this, pListPathGameDefBase));
	
	helper.EditString(content, "", "Shared game definition information", pEditSharedGameDefInfo, 3, {});
	
	helper.ComboBox(content, "Scripting Module:", "Scripting module to use.", pCBScriptModule, igdeDialogProjectSettings_ComboScriptModule::Ref::New(*this));
	pCBScriptModule->SetDefaultSorter();
	
	helper.EditString(content, "Version:", "Scripting module version to use.", 6, pEditScriptModuleVersion, {});
	
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Accept", "Discard");
	
	AddContent(content, buttonBar);
	
	
	// initialize
	const igdeGameProject &project = *env.GetGameProject();
	
	pEditName->SetText(project.GetName());
	pEditDescription->SetText(project.GetDescription());
	pEditPathData->SetText(project.GetPathData());
	pEditPathCache->SetText(project.GetPathCache());
	
	pInitScriptModules();
	
	const decStringList &baseGameDefs = project.GetBaseGameDefinitionIDList();
	const int baseGameDefCount = baseGameDefs.GetCount();
	int i;
	
	for(i=0; i<baseGameDefCount; i++){
		pListPathGameDefBase->AddItem(baseGameDefs.GetAt(i));
	}
	
	pCBScriptModule->SetText(project.GetScriptModule());
	pEditScriptModuleVersion->SetText(project.GetScriptModuleVersion());
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(600, 500)));
}

igdeDialogProjectSettings::~igdeDialogProjectSettings(){
}



// Management
///////////////

bool igdeDialogProjectSettings::CheckValidInput(){
	if(pEditName->GetText().IsEmpty()){
		igdeCommonDialogs::Error(this, "Game Project Settings", "Name can not be empty");
		return false;
	}
	
	return true;
}

bool igdeDialogProjectSettings::Accept(){
	if(!CheckValidInput()){
		return false;
	}
	
	const igdeGameDefinitionList &sharedGameDefList = pWindowMain.GetSharedGameDefinitions();
	
	try{
		igdeGameProject &project = *GetEnvironment().GetGameProject();
		
		project.SetName(pEditName->GetText());
		project.SetDescription(pEditDescription->GetText());
		project.SetPathData(pEditPathData->GetText());
		project.SetPathCache(pEditPathCache->GetText());
		project.SetScriptModule(pCBScriptModule->GetText());
		project.SetScriptModuleVersion(pEditScriptModuleVersion->GetText());
		
		project.GetBaseGameDefinitionIDList().RemoveAll();
		project.GetBaseGameDefinitionList().RemoveAll();
		
		const int baseGameDefCount = pListPathGameDefBase->GetItemCount();
		int i;
		for(i=0; i<baseGameDefCount; i++){
			const decString &id = pListPathGameDefBase->GetItemAt(i)->GetText();
			project.GetBaseGameDefinitionIDList().Add(id);
			project.GetBaseGameDefinitionList().Add(sharedGameDefList.GetWithID(id));
		}
		
		pBaseGameDefsChanged = true; // TODO check first if this is required
		
		project.SetChanged(true);
		
	}catch(const deException &e){
		pWindowMain.GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(this, e);
	}
	
	return igdeDialog::Accept();
}



igdeGameDefinition *igdeDialogProjectSettings::GetSelectedSharedGameDef() const{
	return pCBSharedGameDefs->GetSelectedItem()
		? static_cast<igdeGameDefinition*>(pCBSharedGameDefs->GetSelectedItem()->GetData()) : nullptr;
}

void igdeDialogProjectSettings::UpdateBaseGameDefButtons(){
	pBtnPathGameDefBaseAdd->GetAction()->Update();
	pBtnPathGameDefBaseRemove->GetAction()->Update();
	pBtnPathGameDefBaseUp->GetAction()->Update();
	pBtnPathGameDefBaseDown->GetAction()->Update();
}

void igdeDialogProjectSettings::UpdateSharedGameDefs(){
	igdeGameDefinition * const selection = GetSelectedSharedGameDef();
	
	pCBSharedGameDefs->RemoveAllItems();
	
	const decString &scriptModule = pCBScriptModule->GetText();
	if(scriptModule.IsEmpty()){
		return;
	}
	
	const igdeGameDefinitionList &list = pWindowMain.GetSharedGameDefinitions();
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeGameDefinition * const gameDefinition = list.GetAt(i);
		if(gameDefinition->GetScriptModule() == scriptModule){
			pCBSharedGameDefs->AddItem(gameDefinition->GetID(), nullptr, gameDefinition);
		}
	}
	
	pCBSharedGameDefs->SortItems();
	pCBSharedGameDefs->SetSelectionWithData(selection);
	if(!pCBSharedGameDefs->GetSelectedItem() && pCBSharedGameDefs->GetItemCount() > 0){
		pCBSharedGameDefs->SetSelection(0);
	}
}

void igdeDialogProjectSettings::UpdateSharedGameDef(){
	igdeGameDefinition * const sharedGameDef = GetSelectedSharedGameDef();
	if(sharedGameDef){
		pEditSharedGameDefInfo->SetText(sharedGameDef->GetDescription());
		
	}else{
		pEditSharedGameDefInfo->ClearText();
	}
}



// Private Functions
//////////////////////

void igdeDialogProjectSettings::pInitScriptModules(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	const int engModuleCount = moduleSystem.GetModuleCount();
	int i;
	
	for(i=0; i<engModuleCount; i++){
		const deLoadableModule &module = *moduleSystem.GetModuleAt(i);
		
		if(module.GetType() != deModuleSystem::emtScript || module.GetName() == "IGDEScript"){
			continue;
		}
		
		pCBScriptModule->AddItem(module.GetName());
	}
	
	pCBScriptModule->SortItems();
	
	if(pCBScriptModule->GetItemCount() > 0){
		pCBScriptModule->SetSelection(0);
	}
}
