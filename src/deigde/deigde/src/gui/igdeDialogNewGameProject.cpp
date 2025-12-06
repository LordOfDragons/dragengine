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

#include "igdeCreateProject.h"
#include "igdeDialogNewGameProject.h"
#include "igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../configuration/igdeConfiguration.h"
#include "../template/igdeTemplate.h"
#include "../template/igdeTemplateList.h"

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
#include <deigde/gui/composed/igdeEditDirectory.h>
#include <deigde/gui/composed/igdeEditDirectoryListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Actions
////////////

class igdeDialogNewGameProject_TextName : public igdeTextFieldListener {
	igdeDialogNewGameProject &pDialog;
	
public:
	igdeDialogNewGameProject_TextName( igdeDialogNewGameProject& dialog ) : pDialog( dialog ){}
	
	virtual void OnTextChanged( igdeTextField* ){
		pDialog.AutoFillPathProject();
		pDialog.AutoFillPathGameDef();
	}
	
	virtual void OnTextChanging( igdeTextField *textField ){
		OnTextChanged( textField );
	}
};

class igdeDialogNewGameProject_DirectoryProject : public igdeEditDirectoryListener {
	igdeDialogNewGameProject &pDialog;
	
public:
	igdeDialogNewGameProject_DirectoryProject( igdeDialogNewGameProject& dialog ) : pDialog( dialog ){}
	
	virtual void OnEditDirectoryChanged( igdeEditDirectory* ){
		pDialog.OnProjectDirChanged();
		pDialog.AutoFillPathGameDef();
	}
};

class igdeDialogNewGameProject_TextPathGameDefProject : public igdeTextFieldListener {
	igdeDialogNewGameProject &pDialog;
	
public:
	igdeDialogNewGameProject_TextPathGameDefProject( igdeDialogNewGameProject& dialog ) : pDialog( dialog ){}
	
	virtual void OnTextChanged( igdeTextField* ){
		pDialog.OnProjectGameDefPathChanged();
	}
	
	virtual void OnTextChanging( igdeTextField *textField ){
		OnTextChanged( textField );
	}
};



class igdeDialogNewGameProject_ListPathGameDefBase : public igdeListBoxListener {
	igdeDialogNewGameProject &pDialog;
	
public:
	igdeDialogNewGameProject_ListPathGameDefBase( igdeDialogNewGameProject& dialog ) : pDialog( dialog ){}
	
	virtual void OnSelectionChanged( igdeListBox* ){
		pDialog.UpdateBaseGameDefButtons();
	}
};

class igdeDialogNewGameProject_ComboSharedGameDef : public igdeComboBoxListener {
	igdeDialogNewGameProject &pDialog;
	
public:
	igdeDialogNewGameProject_ComboSharedGameDef( igdeDialogNewGameProject& dialog ) : pDialog( dialog ){}
	
	virtual void OnTextChanged( igdeComboBox* ){
		pDialog.UpdateSharedGameDef();
	}
};

class igdeDialogNewGameProject_ActionGameDefBaseAdd : public igdeAction {
	igdeDialogNewGameProject &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	igdeDialogNewGameProject_ActionGameDefBaseAdd(
		igdeDialogNewGameProject& dialog, igdeListBox &listPathGameDefBase ) :
	igdeAction( "", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ),
		"Add base game definition" ), pDialog( dialog ), pListPathGameDefBase( listPathGameDefBase ){}
	
	virtual void OnAction(){
		igdeGameDefinition * const sharedGameDef = pDialog.GetSelectedSharedGameDef();
		if( ! sharedGameDef ){
			return;
		}
		
		pListPathGameDefBase.AddItem( sharedGameDef->GetID() );
		pListPathGameDefBase.SetSelection( pListPathGameDefBase.IndexOfItem( sharedGameDef->GetID() ) );
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	virtual void Update(){
		SetEnabled( pDialog.GetSelectedSharedGameDef()
			&& ! pListPathGameDefBase.HasItem( pDialog.GetSelectedSharedGameDef()->GetID() ) );
	}
};

class igdeDialogNewGameProject_ActionGameDefBaseRemove : public igdeAction {
	igdeDialogNewGameProject &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	igdeDialogNewGameProject_ActionGameDefBaseRemove(
		igdeDialogNewGameProject& dialog, igdeListBox &listPathGameDefBase ) :
	igdeAction( "", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus ),
		"Remove base game definition" ), pDialog( dialog ), pListPathGameDefBase( listPathGameDefBase ){}
	
	virtual void OnAction(){
		const int selection = pListPathGameDefBase.GetSelection();
		if( selection == -1 ){
			return;
		}
		
		pListPathGameDefBase.RemoveItem( selection );
		if( ! pListPathGameDefBase.GetSelectedItem() && pListPathGameDefBase.GetItemCount() > 0 ){
			pListPathGameDefBase.SetSelection( 0 );
		}
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	virtual void Update(){
		SetEnabled( pListPathGameDefBase.GetSelectedItem() );
	}
};

class igdeDialogNewGameProject_ActionGameDefBaseUp : public igdeAction {
	igdeDialogNewGameProject &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	igdeDialogNewGameProject_ActionGameDefBaseUp(
		igdeDialogNewGameProject& dialog, igdeListBox &listPathGameDefBase ) :
	igdeAction( "", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallUp ),
		"Move base game definition up" ), pDialog( dialog ), pListPathGameDefBase( listPathGameDefBase ){}
	
	virtual void OnAction(){
		const int selection = pListPathGameDefBase.GetSelection();
		if( selection < 1 ){
			return;
		}
		
		pListPathGameDefBase.MoveItem( selection, selection - 1 );
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	virtual void Update(){
		SetEnabled( pListPathGameDefBase.GetSelection() > 0 );
	}
};

class igdeDialogNewGameProject_ActionGameDefBaseDown : public igdeAction {
	igdeDialogNewGameProject &pDialog;
	igdeListBox &pListPathGameDefBase;
	
public:
	igdeDialogNewGameProject_ActionGameDefBaseDown(
		igdeDialogNewGameProject& dialog, igdeListBox &listPathGameDefBase ) :
	igdeAction( "", dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Move base game definition down" ), pDialog( dialog ), pListPathGameDefBase( listPathGameDefBase ){}
	
	virtual void OnAction(){
		const int selection = pListPathGameDefBase.GetSelection();
		const int count = pListPathGameDefBase.GetItemCount();
		if( selection == -1 || selection >= count - 1 ){
			return;
		}
		
		pListPathGameDefBase.MoveItem( selection, selection + 1 );
		
		pDialog.UpdateBaseGameDefButtons();
	}
	
	virtual void Update(){
		SetEnabled( pListPathGameDefBase.GetSelection() != -1
			&& pListPathGameDefBase.GetSelection() < pListPathGameDefBase.GetItemCount() - 1 );
	}
};


class igdeDialogNewGameProject_ComboTemplate : public igdeComboBoxListener {
	igdeDialogNewGameProject &pDialog;
	
public:
	igdeDialogNewGameProject_ComboTemplate( igdeDialogNewGameProject& dialog ) : pDialog( dialog ){}
	
	virtual void OnTextChanged( igdeComboBox* ){
		pDialog.UpdateTemplate();
	}
};


class igdeDialogNewGameProject_ComboScriptModule : public igdeComboBoxListener {
	igdeDialogNewGameProject &pDialog;
	
public:
	igdeDialogNewGameProject_ComboScriptModule( igdeDialogNewGameProject& dialog ) : pDialog( dialog ){}
	
	virtual void OnTextChanged( igdeComboBox* ){
		pDialog.UpdateSharedGameDefs();
	}
};



// Class igdeDialogNewGameProject
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogNewGameProject::igdeDialogNewGameProject( igdeWindowMain &windowMain ) :
igdeDialog( windowMain.GetEnvironment(), "New Game Project", windowMain.GetIconApplication() ),
pWindowMain( windowMain ),
pNewProject( NULL ),
pProjectPathChanged( false ),
pProjectGameDefPathChanged( false )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref content, panel;
	
	content.TakeOver( new igdeContainerForm( env ) );
	
	helper.EditString( content, "Name:", "Name of the game project.", 80, pEditName,
		new igdeDialogNewGameProject_TextName( *this ) );
	helper.EditString( content, "Description:", "Description of the game project.", pEditDescription, 5, NULL );
	
	helper.EditDirectory( content, "Project Directory:",
		"Project directory to store the project files in.", pEditPathProject,
		new igdeDialogNewGameProject_DirectoryProject( *this ), false );
	
	helper.EditString( content, "Project Game Definition:",
		"Filename of project game definition file relative to project directory.", pEditPathGameDefProject,
		 new igdeDialogNewGameProject_TextPathGameDefProject( *this ) );
	helper.EditString( content, "Data Directory:",
		"Data directory relative to project directory.", pEditPathData, NULL );
	helper.EditString( content, "Cache Directory:",
		"Cache directory relative to project directory.", pEditPathCache, NULL );
	
	helper.ComboBox( content, "Project Template:",
		"Project template to use. Enforces script module and some base game definitions.",
		pCBTemplate, new igdeDialogNewGameProject_ComboTemplate( *this ) );
	pCBTemplate->SetDefaultSorter();
	helper.EditString( content, "", "Template description.", pEditTemplateInfo, 30, 5, NULL );
	pEditTemplateInfo->SetEditable( false );
	
	helper.ListBox( content, "Base Game Definitions:", 3,
		"Game definitions to use as base for the project.", pListPathGameDefBase, NULL );
	
	helper.FormLineStretchFirst( content, "", "Available game definitions to add to project", panel );
	
	helper.ComboBox( panel, "Available game definitions to add to project",
		pCBSharedGameDefs, new igdeDialogNewGameProject_ComboSharedGameDef( *this ) );
	pCBSharedGameDefs->SetDefaultSorter();
	
	helper.Button( panel, pBtnPathGameDefBaseAdd,
		new igdeDialogNewGameProject_ActionGameDefBaseAdd( *this, pListPathGameDefBase ), true );
	helper.Button( panel, pBtnPathGameDefBaseRemove,
		new igdeDialogNewGameProject_ActionGameDefBaseRemove( *this, pListPathGameDefBase ), true );
	helper.Button( panel, pBtnPathGameDefBaseUp,
		new igdeDialogNewGameProject_ActionGameDefBaseUp( *this, pListPathGameDefBase ), true );
	helper.Button( panel, pBtnPathGameDefBaseDown,
		new igdeDialogNewGameProject_ActionGameDefBaseDown( *this, pListPathGameDefBase ), true );
	
	helper.EditString( content, "", "Shared game definition information", pEditSharedGameDefInfo, 5, NULL );
	
	helper.ComboBox( content, "Scripting Module:", "Scripting module to use.", pCBScriptModule,
		new igdeDialogNewGameProject_ComboScriptModule( *this ) );
	pCBScriptModule->SetDefaultSorter();
	
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar( buttonBar, "Create Game Project", "Discard" );
	
	AddContent( content, buttonBar );
	
	
	// initialize
	pProjectPathChanged = true;
	pProjectGameDefPathChanged = true;
	
	pEditName->SetText( "New Project" );
	
	decPath path( decPath::CreatePathNative( pWindowMain.GetConfiguration().GetPathProjects() ) );
	path.AddComponent( "New Project" );
	pEditPathProject->SetDirectory( path.GetPathNative() );
	
	pEditPathGameDefProject->SetText( "New Project.degd" );
	pEditPathCache->SetText( "cache" );
	pEditPathData->SetText( "data" );
	
	pInitScriptModules();
	pInitTemplates();
	
	pProjectPathChanged = false;
	pProjectGameDefPathChanged = false;
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(800, 600)));
}

igdeDialogNewGameProject::~igdeDialogNewGameProject(){
	if( pNewProject ){
		pNewProject->FreeReference();
	}
}



// Management
///////////////

void igdeDialogNewGameProject::OnProjectDirChanged(){
	pProjectPathChanged = true;
}

void igdeDialogNewGameProject::OnProjectGameDefPathChanged(){
	pProjectGameDefPathChanged = true;
}

igdeGameDefinition *igdeDialogNewGameProject::GetSelectedSharedGameDef() const{
	return pCBSharedGameDefs->GetSelectedItem()
		? ( igdeGameDefinition* )pCBSharedGameDefs->GetSelectedItem()->GetData() : NULL;
}

igdeTemplate *igdeDialogNewGameProject::GetSelectedTemplate() const{
	return pCBTemplate->GetSelectedItem()
		? ( igdeTemplate* )pCBTemplate->GetSelectedItem()->GetData() : NULL;
}

bool igdeDialogNewGameProject::CheckValidInput(){
	const char * const mbtitle = "New Game Project";
	
	if( pEditName->GetText().IsEmpty() ){
		pEditName->Focus();
		igdeCommonDialogs::Error( this, mbtitle, "Name can not be empty" );
		return false;
	}
	
	if( ! pCBTemplate->GetSelectedItem()->GetData() ){
		if( igdeCommonDialogs::Question( this, igdeCommonDialogs::ebsYesNo, mbtitle,
		"No template selected. Do you really want to create an empty project?" )
		== igdeCommonDialogs::ebNo ){
			pCBTemplate->Focus();
			return false;
		}
	}
	
	deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::NewWith(
		decPath::CreatePathNative( pEditPathProject->GetDirectory() )));
	if( container->ExistsFile( decPath() ) ){
		pEditPathProject->Focus();
		igdeCommonDialogs::Error( this, mbtitle, "Project directory exists. "
			"New project can only be created using a non-existing directory" );
		return false;
	}
	
	return true;
}

bool igdeDialogNewGameProject::Accept(){
	if( ! CheckValidInput() ){
		return false;
	}
	
	igdeCreateProject createProject( pWindowMain );
	createProject.SetName( pEditName->GetText() );
	createProject.SetDescription( pEditDescription->GetText() );
	createProject.SetPathGameDefProject( pEditPathGameDefProject->GetText() );
	createProject.SetPathProject( pEditPathProject->GetDirectory() );
	createProject.SetPathData( pEditPathData->GetText() );
	createProject.SetPathCache( pEditPathCache->GetText() );
	createProject.SetScriptModule( pCBScriptModule->GetText() );
	createProject.SetTemplate( ( const igdeTemplate* )pCBTemplate->GetSelectedItem()->GetData() );
	
	const int baseGameDefCount = pListPathGameDefBase->GetItemCount();
	int i;
	for( i=0; i<baseGameDefCount; i++ ){
		createProject.GetBaseGameDefs().Add( pListPathGameDefBase->GetItemAt( i )->GetText() );
	}
	
	try{
		createProject.CreateProject();
		
	}catch( const deException &e ){
		pWindowMain.GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( this, e );
		return false;
	}
	
	pNewProject = createProject.GetProject();
	pNewProject->AddReference();
	
	pWindowMain.AddRecentGameProject( pNewProject->GetFilePath() );
	
	return igdeDialog::Accept();
}



void igdeDialogNewGameProject::AutoFillPathProject(){
	if( pProjectPathChanged ){
		return;
	}
	
	decPath path( decPath::CreatePathNative( pWindowMain.GetConfiguration().GetPathProjects() ) );
	path.AddComponent( pEditName->GetText() );
	pEditPathProject->SetDirectory( path.GetPathNative() );
	
	pProjectPathChanged = false;
}

void igdeDialogNewGameProject::AutoFillPathGameDef(){
	if( pProjectGameDefPathChanged ){
		return;
	}
	
	pEditPathGameDefProject->SetText( pEditName->GetText() + ".degd" );
	
	pProjectGameDefPathChanged = false;
}

void igdeDialogNewGameProject::UpdateBaseGameDefButtons(){
	pBtnPathGameDefBaseAdd->GetAction()->Update();
	pBtnPathGameDefBaseRemove->GetAction()->Update();
	pBtnPathGameDefBaseUp->GetAction()->Update();
	pBtnPathGameDefBaseDown->GetAction()->Update();
}

void igdeDialogNewGameProject::UpdateSharedGameDefs(){
	igdeGameDefinition * const selection = GetSelectedSharedGameDef();
	
	pCBSharedGameDefs->RemoveAllItems();
	
	const decString &scriptModule = pCBScriptModule->GetText();
	if( scriptModule.IsEmpty() ){
		return;
	}
	
	const igdeGameDefinitionList &list = pWindowMain.GetSharedGameDefinitions();
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGameDefinition * const gameDefinition = list.GetAt( i );
		if( gameDefinition->GetScriptModule() == scriptModule ){
			pCBSharedGameDefs->AddItem( gameDefinition->GetID(), NULL, gameDefinition );
		}
	}
	
	pCBSharedGameDefs->SortItems();
	pCBSharedGameDefs->SetSelectionWithData( selection );
	if( ! pCBSharedGameDefs->GetSelectedItem() && pCBSharedGameDefs->GetItemCount() > 0 ){
		pCBSharedGameDefs->SetSelection( 0 );
	}
}

void igdeDialogNewGameProject::UpdateSharedGameDef(){
	igdeGameDefinition * const sharedGameDef = GetSelectedSharedGameDef();
	if( sharedGameDef ){
		pEditSharedGameDefInfo->SetText( sharedGameDef->GetDescription() );
		
	}else{
		pEditSharedGameDefInfo->ClearText();
	}
}

void igdeDialogNewGameProject::UpdateTemplate(){
	const igdeTemplate * const atemplate = GetSelectedTemplate();
	if( ! atemplate ){
		pEditTemplateInfo->SetText( "No template selected. Creates an empty project" );
		pCBScriptModule->SetEnabled( true );
		return;
	}
	
	pEditTemplateInfo->SetText( atemplate->GetDescription() );
	pCBScriptModule->SetText( atemplate->GetScriptModule() );
	pCBScriptModule->SetEnabled( false );
	
	// enforce base game definitions. for this to work properly with the ordering remove
	// first all enforced game definitions from the list. then insert the enforced game
	// definitions at the start of the list. this should be good enough for the time being
	const decStringList &baseGameDefs = atemplate->GetBaseGameDefinitions();
	const int baseGameDefCount = baseGameDefs.GetCount();
	int i;
	
	for( i=0; i<baseGameDefCount; i++ ){
		const int index = pListPathGameDefBase->IndexOfItem( baseGameDefs.GetAt( i ) );
		if( index != -1 ){
			pListPathGameDefBase->RemoveItem( index );
		}
	}
	
	for( i=0; i<baseGameDefCount; i++ ){
		pListPathGameDefBase->InsertItem( 0, baseGameDefs.GetAt( i ) );
	}
	
	UpdateBaseGameDefButtons();
}




// Private Functions
//////////////////////

void igdeDialogNewGameProject::pInitScriptModules(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	const int engModuleCount = moduleSystem.GetModuleCount();
	int i;
	
	for( i=0; i<engModuleCount; i++ ){
		const deLoadableModule &module = *moduleSystem.GetModuleAt( i );
		
		if( module.GetType() != deModuleSystem::emtScript || module.GetName() == "IGDEScript" ){
			continue;
		}
		
		if( ! pCBScriptModule->HasItem( module.GetName() ) ){
			pCBScriptModule->AddItem( module.GetName() );
		}
	}
	
	pCBScriptModule->SortItems();
	
	if( pCBScriptModule->GetItemCount() > 0 ){
		pCBScriptModule->SetSelection( 0 );
	}
}

void igdeDialogNewGameProject::pInitTemplates(){
	const igdeTemplateList &templates = pWindowMain.GetTemplates();
	const int count = templates.GetCount();
	int i;
	
	pCBTemplate->RemoveAllItems();
	pCBTemplate->AddItem( "< Use No Template >" );
	
	for( i=0; i<count; i++ ){
		igdeTemplate * const atemplate = templates.GetAt( i );
		if( ! pCBScriptModule->HasItemWithData( atemplate ) ){
			pCBTemplate->AddItem( atemplate->GetName(), NULL, atemplate );
		}
	}
	
	pCBTemplate->SortItems();
	
	pCBTemplate->SetSelectionWithData( NULL );
	
	UpdateTemplate();
}
