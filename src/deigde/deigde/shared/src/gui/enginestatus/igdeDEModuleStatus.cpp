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
#include "../igdeContainerReference.h"
#include "../event/igdeAction.h"
#include "../event/igdeComboBoxListener.h"
#include "../model/igdeListItem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
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
	igdeDEModuleStatus_ComboModule( igdeDEModuleStatus &panel ) : pPanel( panel ){}
	
	virtual void OnTextChanged( igdeComboBox* ){
		pPanel.UpdateModuleStatus();
	}
};



// Class igdeDEModuleStatus
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDEModuleStatus::igdeDEModuleStatus( igdeDialogEngine &dialogEngine ) :
igdeContainerFlow( dialogEngine.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esLast ),
pDialogEngine( dialogEngine )
{
	igdeEnvironment &env = dialogEngine.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainerReference groupBox, line;
	
	
	line.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esLast ) );
	AddChild( line );
	helper.ComboBox( line, "Module:", "Module to show information for",
		pCBModule, new igdeDEModuleStatus_ComboModule( *this ) );
	pCBModule->SetDefaultSorter();
	
	
	// module information
	helper.GroupBoxStatic( *this, groupBox, "Module Information:" );
	
	helper.EditString( groupBox, "Description:", "Module description", pEditDescription, 3, NULL );
	pEditDescription->SetEditable( false );
	
	helper.EditString( groupBox, "Type:", "Module type", pEditType, NULL );
	pEditType->SetEditable( false );
	
	helper.EditString( groupBox, "Author:", "Module author", pEditAuthor, NULL );
	pEditAuthor->SetEditable( false );
	
	helper.EditString( groupBox, "Version:", "Module version", pEditVersion, NULL );
	pEditVersion->SetEditable( false );
	
	helper.CheckBox( groupBox, "Fallback Module",
		"Module is used as fallback if no better module is usable", pChkFallback );
	pChkFallback->SetEnabled( false );
	
	
	// file handling information
	helper.GroupBoxStatic( *this, groupBox, "File Handling Information:" );
	
	helper.EditString( groupBox, "Recognized File Pattern:", "Recognized File Pattern", pEditPattern, NULL );
	pEditPattern->SetEditable( false );
	
	helper.EditString( groupBox, "Default Extension:", "Default Extension", pEditDefaultExtension, NULL );
	pEditDefaultExtension->SetEditable( false );
	
	helper.EditString( groupBox, "Module Directory Name:", "Module Directory Name", pEditDirName, NULL );
	pEditDirName->SetEditable( false );
	
	
	// library information
	helper.GroupBoxStatic( *this, groupBox, "Library Module Information:" );
	
	helper.EditString( groupBox, "Library Filename:", "Library Filename", pEditLibName, NULL );
	pEditLibName->SetEditable( false );
	
	helper.EditString( groupBox, "Library File Size:", "Library File Size", pEditLibSize, NULL );
	pEditLibSize->SetEditable( false );
	
	helper.EditString( groupBox, "Library Check Sum (SHA):", "Library Check Sum (SHA)", pEditLibHash, NULL );
	pEditLibHash->SetEditable( false );
	
	
	// library information
	helper.GroupBoxStaticFlow( *this, groupBox, "Module Status:", true );
	
	helper.EditString( groupBox, "Module description", pEditStatus, 3, NULL );
	pEditStatus->SetEditable( false );
	
	
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
		? ( deLoadableModule* )pCBModule->GetSelectedItem()->GetData() : NULL;
	
	if( ! loadedModule ){
		pEditType->SetText( "?" );
		pEditDescription->SetText( "?" );
		pEditAuthor->SetText( "?" );
		pEditVersion->SetText( "?" );
		pEditPattern->SetText( "?" );
		pEditDirName->SetText( "?" );
		pEditStatus->SetText( "?" );
		
		pEditLibName->SetText( "?" );
		pEditLibSize->SetText( "?" );
		pEditLibHash->SetText( "?" );
		
		pChkFallback->SetChecked( false );
		return;
	}
	
	pEditDescription->SetText( loadedModule->GetDescription() );
	pEditAuthor->SetText( loadedModule->GetAuthor() );
	pEditVersion->SetText( loadedModule->GetVersion() );
	
	const decStringList &patternList = loadedModule->GetPatternList();
	const int patternCount = patternList.GetCount();
	decStringList patterns;
	int i;
	for( i=0; i<patternCount; i++ ){
		patterns.Add( decString( "*" ) + patternList.GetAt( i ) );
	}
	pEditPattern->SetText( patterns.Join( ", " ) );
	
	pEditDefaultExtension->SetText( loadedModule->GetDefaultExtension() );
	pEditDirName->SetText( loadedModule->GetDirectoryName() );
	pChkFallback->SetChecked( loadedModule->GetIsFallback() );
	
	switch( loadedModule->GetType() ){
	case deModuleSystem::emtGraphic:
		pEditType->SetText( "Graphic" );
		break;
		
	case deModuleSystem::emtSound:
		pEditType->SetText( "Sound" );
		break;
		
	case deModuleSystem::emtInput:
		pEditType->SetText( "Input" );
		break;
		
	case deModuleSystem::emtNetwork:
		pEditType->SetText( "Network" );
		break;
		
	case deModuleSystem::emtPhysics:
		pEditType->SetText( "Physics" );
		break;
		
	case deModuleSystem::emtImage:
		pEditType->SetText( "Image" );
		break;
		
	case deModuleSystem::emtVideo:
		pEditType->SetText( "Video" );
		break;
		
	case deModuleSystem::emtScript:
		pEditType->SetText( "Scripting" );
		break;
		
	case deModuleSystem::emtModel:
		pEditType->SetText( "Model" );
		break;
		
	case deModuleSystem::emtRig:
		pEditType->SetText( "Rig" );
		break;
		
	case deModuleSystem::emtSkin:
		pEditType->SetText( "Skin" );
		break;
		
	case deModuleSystem::emtAnimation:
		pEditType->SetText( "Animation" );
		break;
		
	case deModuleSystem::emtFont:
		pEditType->SetText( "Font" );
		break;
		
	case deModuleSystem::emtCrashRecovery:
		pEditType->SetText( "Crash Recovery" );
		break;
		
	case deModuleSystem::emtLanguagePack:
		pEditType->SetText( "Language Pack" );
		break;
		
	case deModuleSystem::emtAnimator:
		pEditType->SetText( "Animator" );
		break;
		
	case deModuleSystem::emtOcclusionMesh:
		pEditType->SetText( "Occlusion Mesh" );
		break;
		
	case deModuleSystem::emtSynthesizer:
		pEditType->SetText( "Synthesizer" );
		break;
		
	case deModuleSystem::emtArchive:
		pEditType->SetText( "Archive" );
		break;
		
	case deModuleSystem::emtVR:
		pEditType->SetText( "VR" );
		break;
		
	default:
		pEditType->SetText( "Unknown Type" );
	}
	
	if( loadedModule->IsInternalModule() ){
		const deInternalModule * const internalModule = loadedModule->CastToInternalModule();
		
		pEditLibName->SetText( "?" );
		pEditLibSize->SetText( "?" );
		pEditLibHash->SetText( "?" );
		
		switch( internalModule->GetErrorCode() ){
		case deLoadableModule::eecSuccess:
			pEditStatus->SetText( "Everything OK" );
			break;
			
		case deLoadableModule::eecCreateModuleFailed:
			pEditStatus->SetText( "Creation of module failed" );
			break;
			
		default:
			pEditStatus->SetText( "Unknown error code" );
		}
		
	}else if( loadedModule->IsLibraryModule() ){
		deLibraryModule * const libraryModule = loadedModule->CastToLibraryModule();
		
		pEditLibName->SetText( libraryModule->GetLibFileName() );
		pEditLibSize->SetInteger( libraryModule->GetLibFileSize() );
		pEditLibHash->SetText( libraryModule->GetLibFileHash() );
		
		switch( libraryModule->GetErrorCode() ){
		case deLoadableModule::eecSuccess:
			pEditStatus->SetText( "Everything OK" );
			break;
			
		case deLoadableModule::eecCreateModuleFailed:
			pEditStatus->SetText( "Creation of module failed" );
			break;
			
		case deLibraryModule::eecLibFileNotFound:
			pEditStatus->SetText( "Library file could not be found" );
			break;
			
		case deLibraryModule::eecLibFileNotRegularFile:
			pEditStatus->SetText( "Library file is not a regular file" );
			break;
			
		case deLibraryModule::eecLibFileSizeMismatch:
			pEditStatus->SetText( "Size of library file does not match expected size" );
			break;
			
		case deLibraryModule::eecLibFileCheckSumMismatch:
			pEditStatus->SetText( "Check sum of library file does not match expected value" );
			break;
			
		case deLibraryModule::eecLibFileOpenFailed:
			pEditStatus->SetText( "Library file could not be opened" );
			break;
			
		case deLibraryModule::eecLibFileEntryPointNotFound:
			pEditStatus->SetText( "Entry point function could not be found" );
			break;
			
		case deLibraryModule::eecLibFileCreateModuleFailed:
			pEditStatus->SetText( "Creating module using entry function failed" );
			break;
			
		default:
			pEditStatus->SetText( "Unknown error code" );
		}
		
	}else{
		pEditStatus->SetText( "?" );
		pEditLibName->SetText( "?" );
		pEditLibSize->SetText( "?" );
		pEditLibHash->SetText( "?" );
	}
}

void igdeDEModuleStatus::UpdateModulesList(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	const int count = moduleSystem.GetModuleCount();
	decString text;
	int i;
	
	for( i=0; i<count; i++ ){
		deLoadableModule * const loadedModule = moduleSystem.GetModuleAt( i );
		text.Format( "%s %s", loadedModule->GetName().GetString(), loadedModule->GetVersion().GetString() );
		pCBModule->AddItem( text.GetString(), NULL, loadedModule );
	}
	pCBModule->SortItems();
}
