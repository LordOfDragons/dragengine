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

#include "igdeDEModuleStatus.h"
#include "igdeDialogEngine.h"
#include "../igdeUIHelper.h"
#include "../igdeComboBox.h"
#include "../igdeCheckBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeContainer.h"
#include "../event/igdeAction.h"
#include "../event/igdeComboBoxListener.h"
#include "../model/igdeListItem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decGlobalFunctions.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deInternalModule.h>
#include <dragengine/systems/modules/deLibraryModule.h>



// Actions
////////////

class igdeDEModuleStatus_ComboModule : public igdeComboBoxListener{
	igdeDEModuleStatus &pPanel;
	
public:
	using Ref = deTObjectReference<igdeDEModuleStatus_ComboModule>;
	
	igdeDEModuleStatus_ComboModule(igdeDEModuleStatus &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox*) override{
		pPanel.UpdateModuleStatus();
	}
};



// Class igdeDEModuleStatus
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDEModuleStatus::igdeDEModuleStatus(igdeDialogEngine &dialogEngine) :
igdeContainerFlow(dialogEngine.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esLast),
pDialogEngine(dialogEngine)
{
	igdeEnvironment &env = dialogEngine.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref groupBox, line;
	
	
	line = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esLast);
	AddChild(line);
	helper.ComboBox(line, "@Igde.DEModuleStatus.Module.Label", "@Igde.DEModuleStatus.Module.ToolTip",
		pCBModule, igdeDEModuleStatus_ComboModule::Ref::New(*this));
	pCBModule->SetDefaultSorter();
	
	
	// module information
	helper.GroupBoxStatic(*this, groupBox, "@Igde.DEModuleStatus.ModuleInformation.Label");
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.Description.Label", "@Igde.DEModuleStatus.Description.ToolTip", pEditDescription, 3, {});
	pEditDescription->SetEditable(false);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.Type.Label", "@Igde.DEModuleStatus.Type.ToolTip", pEditType, {});
	pEditType->SetEditable(false);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.Author.Label", "@Igde.DEModuleStatus.Author.ToolTip", pEditAuthor, {});
	pEditAuthor->SetEditable(false);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.Version.Label", "@Igde.DEModuleStatus.Version.ToolTip", pEditVersion, {});
	pEditVersion->SetEditable(false);
	
	helper.CheckBox(groupBox, "@Igde.DEModuleStatus.FallbackModule",
		"@Igde.DEModuleStatus.FallbackModule.ToolTip", pChkFallback);
	pChkFallback->SetEnabled(false);
	
	
	// file handling information
	helper.GroupBoxStatic(*this, groupBox, "@Igde.DEModuleStatus.FileHandlingInformation.Label");
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.RecognizedFilePattern.Label", "@Igde.DEModuleStatus.RecognizedFilePattern.ToolTip", pEditPattern, {});
	pEditPattern->SetEditable(false);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.DefaultExtension.Label", "@Igde.DEModuleStatus.DefaultExtension.ToolTip", pEditDefaultExtension, {});
	pEditDefaultExtension->SetEditable(false);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.ModuleDirectoryName.Label", "@Igde.DEModuleStatus.ModuleDirectoryName.ToolTip", pEditDirName, {});
	pEditDirName->SetEditable(false);
	
	
	// library information
	helper.GroupBoxStatic(*this, groupBox, "@Igde.DEModuleStatus.LibraryModuleInformation.Label");
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.LibraryFilename.Label", "@Igde.DEModuleStatus.LibraryFilename.ToolTip", pEditLibName, {});
	pEditLibName->SetEditable(false);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.LibraryFileSize.Label", "@Igde.DEModuleStatus.LibraryFileSize.ToolTip", pEditLibSize, {});
	pEditLibSize->SetEditable(false);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.LibraryCheckSum.Label", "@Igde.DEModuleStatus.LibraryCheckSum.ToolTip", pEditLibHash, {});
	pEditLibHash->SetEditable(false);
	
	
	// library information
	helper.GroupBoxStaticFlow(*this, groupBox, "@Igde.DEModuleStatus.ModuleStatus.Label", true);
	
	helper.EditString(groupBox, "@Igde.DEModuleStatus.ModuleStatus.ToolTip", pEditStatus, 3, {});
	pEditStatus->SetEditable(false);
	
	
	// load the modules list
	UpdateModulesList();
	UpdateModuleStatus();
}

igdeDEModuleStatus::~igdeDEModuleStatus(){
}



// Management
///////////////

void igdeDEModuleStatus::UpdateModuleStatus(){
	deLoadableModule * const loadedModule = pCBModule->GetSelectedItem()
		? (deLoadableModule*)pCBModule->GetSelectedItem()->GetData() : nullptr;
	
	if(!loadedModule){
		pEditType->SetText("");
		pEditDescription->SetText("");
		pEditAuthor->SetText("");
		pEditVersion->SetText("");
		pEditPattern->SetText("");
		pEditDirName->SetText("");
		pEditStatus->SetText("");
		
		pEditLibName->SetText("");
		pEditLibSize->SetText("");
		pEditLibHash->SetText("");
		
		pChkFallback->SetChecked(false);
		return;
	}
	
	pEditDescription->SetText(loadedModule->GetDescription());
	pEditAuthor->SetText(loadedModule->GetAuthor());
	pEditVersion->SetText(loadedModule->GetVersion());
	
	const decStringList &patternList = loadedModule->GetPatternList();
	const int patternCount = patternList.GetCount();
	decStringList patterns;
	int i;
	for(i=0; i<patternCount; i++){
		patterns.Add(decString("*") + patternList.GetAt(i));
	}
	pEditPattern->SetText(DEJoin(patterns, ", "));
	
	pEditDefaultExtension->SetText(loadedModule->GetDefaultExtension());
	pEditDirName->SetText(loadedModule->GetDirectoryName());
	pChkFallback->SetChecked(loadedModule->GetIsFallback());
	
	switch(loadedModule->GetType()){
	case deModuleSystem::emtGraphic:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Graphic").ToUTF8());
		break;
		
	case deModuleSystem::emtSound:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Sound").ToUTF8());
		break;
		
	case deModuleSystem::emtInput:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Input").ToUTF8());
		break;
		
	case deModuleSystem::emtNetwork:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Network").ToUTF8());
		break;
		
	case deModuleSystem::emtPhysics:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Physics").ToUTF8());
		break;
		
	case deModuleSystem::emtImage:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Image").ToUTF8());
		break;
		
	case deModuleSystem::emtVideo:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Video").ToUTF8());
		break;
		
	case deModuleSystem::emtScript:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Scripting").ToUTF8());
		break;
		
	case deModuleSystem::emtModel:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Model").ToUTF8());
		break;
		
	case deModuleSystem::emtRig:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Rig").ToUTF8());
		break;
		
	case deModuleSystem::emtSkin:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Skin").ToUTF8());
		break;
		
	case deModuleSystem::emtAnimation:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Animation").ToUTF8());
		break;
		
	case deModuleSystem::emtFont:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Font").ToUTF8());
		break;
		
	case deModuleSystem::emtCrashRecovery:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.CrashRecovery").ToUTF8());
		break;
		
	case deModuleSystem::emtLanguagePack:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.LanguagePack").ToUTF8());
		break;
		
	case deModuleSystem::emtAnimator:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Animator").ToUTF8());
		break;
		
	case deModuleSystem::emtOcclusionMesh:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.OcclusionMesh").ToUTF8());
		break;
		
	case deModuleSystem::emtSynthesizer:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Synthesizer").ToUTF8());
		break;
		
	case deModuleSystem::emtArchive:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Archive").ToUTF8());
		break;
		
	case deModuleSystem::emtVR:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.VR").ToUTF8());
		break;
		
	case deModuleSystem::emtService:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Service").ToUTF8());
		break;
		
	default:
		pEditType->SetText(Translate("Igde.DEModuleStatus.Type.Unknown").ToUTF8());
	}
	
	if(loadedModule->IsInternalModule()){
		const deInternalModule * const internalModule = loadedModule->CastToInternalModule();
		
		pEditLibName->SetText("");
		pEditLibName->SetEnabled(false);
		pEditLibSize->SetText("");
		pEditLibSize->SetEnabled(false);
		pEditLibHash->SetText("");
		pEditLibHash->SetEnabled(false);
		
		switch(internalModule->GetErrorCode()){
		case deLoadableModule::eecSuccess:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.EverythingOk").ToUTF8());
			break;
			
		case deLoadableModule::eecCreateModuleFailed:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.CreateModuleFailed").ToUTF8());
			break;
			
		default:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.UnknownError").ToUTF8());
		}
		
	}else if(loadedModule->IsLibraryModule()){
		deLibraryModule * const libraryModule = loadedModule->CastToLibraryModule();
		
		pEditLibName->SetText(libraryModule->GetLibFileName());
		pEditLibName->SetEnabled(true);
		pEditLibSize->SetInteger(libraryModule->GetLibFileSize());
		pEditLibSize->SetEnabled(true);
		pEditLibHash->SetText(libraryModule->GetLibFileHash());
		pEditLibHash->SetEnabled(true);
		
		switch(libraryModule->GetErrorCode()){
		case deLoadableModule::eecSuccess:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.EverythingOk").ToUTF8());
			break;
			
		case deLoadableModule::eecCreateModuleFailed:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.CreateModuleFailed").ToUTF8());
			break;
			
		case deLibraryModule::eecLibFileNotFound:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.LibFileNotFound").ToUTF8());
			break;
			
		case deLibraryModule::eecLibFileNotRegularFile:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.LibFileNotRegularFile").ToUTF8());
			break;
			
		case deLibraryModule::eecLibFileSizeMismatch:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.LibFileSizeMismatch").ToUTF8());
			break;
			
		case deLibraryModule::eecLibFileCheckSumMismatch:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.LibFileCheckSumMismatch").ToUTF8());
			break;
			
		case deLibraryModule::eecLibFileOpenFailed:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.LibFileOpenFailed").ToUTF8());
			break;
			
		case deLibraryModule::eecLibFileEntryPointNotFound:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.LibFileEntryPointNotFound").ToUTF8());
			break;
			
		case deLibraryModule::eecLibFileCreateModuleFailed:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.LibFileCreateModuleFailed").ToUTF8());
			break;
			
		default:
			pEditStatus->SetText(Translate("Igde.DEModuleStatus.Status.UnknownError").ToUTF8());
		}
		
	}else{
		pEditStatus->SetText("");
		pEditLibName->SetText("");
		pEditLibName->SetEnabled(false);
		pEditLibSize->SetText("");
		pEditLibSize->SetEnabled(false);
		pEditLibHash->SetText("");
		pEditLibHash->SetEnabled(false);
	}
}
void igdeDEModuleStatus::UpdateModulesList(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	
	moduleSystem.GetModules().Visit([&](deLoadableModule *loadedModule){
		decString text;
		text.Format("%s %s", loadedModule->GetName().GetString(), loadedModule->GetVersion().GetString());
		pCBModule->AddItem(text.GetString(), nullptr, loadedModule);
	});
	
	pCBModule->SortItems();
}
