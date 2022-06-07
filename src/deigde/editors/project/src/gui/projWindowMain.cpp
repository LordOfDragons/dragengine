/* 
 * Drag[en]gine IGDE Project Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "projWindowMain.h"
#include "projWindowMainListener.h"
#include "projPanelProfiles.h"
#include "projPanelTestRun.h"
#include "projPanelUndoHistory.h"
#include "projDialogDistribute.h"
#include "../projIGDEModule.h"
#include "../project.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../loadsave/projProjectXml.h"
#include "../loadsave/projProjectLocalXml.h"
#include "../undosys/profile/projUProfileAdd.h"
#include "../undosys/profile/projUProfileRemove.h"

#include <deigde/deigde_configuration.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionExternOpen.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

enum ePanels{
	epProfiles,
	epTestRun,
	epUndoHistory
};



// Class projWindowMain
/////////////////////////

projWindowMain::projWindowMain( igdeEditorModule &module ) :
igdeEditorWindow( module ),

pListener( NULL ),

pLoadSaveSystem( *this ),

pProject( NULL ),

pPanelProfiles( NULL ),
pPanelTestRun( NULL ),
pPanelUndoHistory( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = new projWindowMainListener( *this );
	
	// content
	pCreateToolBarDistribute();
	pCreateToolBarEdit();
	
	pTabPanels.TakeOver( new igdeTabBook( env ) );
	AddChild( pTabPanels );
	
	pPanelProfiles = new projPanelProfiles( *this );
	pTabPanels->AddChild( pPanelProfiles, "Profiles" );
	
	pPanelTestRun = new projPanelTestRun( *this );
	pTabPanels->AddChild( pPanelTestRun, "Test-Run" );
	
	pPanelUndoHistory = new projPanelUndoHistory( env );
	pTabPanels->AddChild( pPanelUndoHistory, "Undo History" );
	
	// load game project
	LoadProject();
}

projWindowMain::~projWindowMain(){
	SetProject( NULL );
	
	if( pPanelProfiles ){
		pPanelProfiles->FreeReference();
		pPanelProfiles = NULL;
	}
	if( pPanelTestRun ){
		pPanelTestRun->FreeReference();
		pPanelTestRun = NULL;
	}
	if( pPanelUndoHistory ){
		pPanelUndoHistory->FreeReference();
		pPanelUndoHistory = NULL;
	}
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

bool projWindowMain::QuitRequest(){
	// check if user wants to quit. returns true if a quit
	// is wished by the user
	
	// clean up
	return true;
}



void projWindowMain::SetProject( projProject *project ){
	if( project == pProject ){
		return;
	}
	
	pPanelProfiles->SetProject( NULL );
	pPanelTestRun->SetProject( NULL );
	pPanelUndoHistory->SetProject( NULL );
	pActionEditUndo->SetUndoSystem( NULL );
	pActionEditRedo->SetUndoSystem( NULL );
	
	if( pProject ){
		pProject->RemoveListener( pListener );
		pProject->FreeReference();
	}
	
	pProject = project;
	
	if( project ){
		project->AddReference();
		project->AddListener( pListener );
		
		pActionEditUndo->SetUndoSystem( project->GetUndoSystem() );
		pActionEditRedo->SetUndoSystem( project->GetUndoSystem() );
	}
	
	pPanelProfiles->SetProject( project );
	pPanelTestRun->SetProject( project );
	pPanelUndoHistory->SetProject( project );
	
	UpdateShowActionPath();
}

void projWindowMain::LoadProject(){
	projProject *project = NULL;
	
	try{
		project = new projProject( &GetEnvironment() );
		
		// load project
		decBaseFileReaderReference reader;
		
		try{
			reader.TakeOver( new decDiskFileReader( project->GetFilePath() ) );
			
		}catch( const deException & ){
			// file does not exist. this is fine and allowed.
		}
		
		if( reader ){
			// file exists. parse it. should this fail we do show an error otherwise not
			projProjectXml( GetLogger(), LOGSOURCE ).ReadFromFile( reader, *project );
			reader = NULL;
		}
		
		// load local project settings if present
		LoadProjectLocal( *project );
		
		// finish loading
		SetProject( project );
		
		project->SetChanged( false ); // SetProject can trigger changes
		
		project->FreeReference();
		
	}catch( const deException &e ){
		if( project ){
			project->FreeReference();
		}
		DisplayException( e );
		
		project = new projProject( &GetEnvironment() );
		SetProject( project );
		project->FreeReference();
	}
}

void projWindowMain::LoadProjectLocal( projProject &project ){
	const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
	
	deVFSContainerReference directory;
	directory.TakeOver( new deVFSDiskDirectory( 
		decPath::CreatePathNative( gameProject.GetDirectoryPath() ) ) );
	
	decBaseFileReaderReference reader;
	decPath path( decPath::CreatePathUnix( gameProject.GetPathLocal() ) );
	path.AddComponent( "project.xml" );
	
	try{
		reader.TakeOver( directory->OpenFileForReading( path ) );
		
	}catch( const deException & ){
		// file does not exist. this is fine and allowed.
	}
	
	if( reader ){
		// file exists. parse it. should this fail we do show an error otherwise not
		projProjectLocalXml( GetLogger(), LOGSOURCE ).ReadFromFile( reader, project );
	}
}

void projWindowMain::SaveProject(){
	if( ! pProject ){
		return;
	}
	
	decBaseFileWriterReference writer;
	writer.TakeOver( new decDiskFileWriter( pProject->GetFilePath(), false ) );
	projProjectXml( GetLogger(), LOGSOURCE ).WriteToFile( writer, *pProject );
	pProject->SetChanged( false );
}

void projWindowMain::SaveProjectLocal(){
	if( ! pProject ){
		return;
	}
	
	const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
	
	deVFSContainerReference directory;
	directory.TakeOver( new deVFSDiskDirectory( 
		decPath::CreatePathNative( gameProject.GetDirectoryPath() ) ) );
	
	decBaseFileWriterReference writer;
	decPath path( decPath::CreatePathUnix( gameProject.GetPathLocal() ) );
	path.AddComponent( "project.xml" );
	writer.TakeOver( directory->OpenFileForWriting( path ) );
	projProjectLocalXml( GetLogger(), LOGSOURCE ).WriteToFile( writer, *pProject );
}



void projWindowMain::OnFrameUpdate( float elapsed ){
	pPanelTestRun->Update( elapsed );
}



void projWindowMain::GetChangedDocuments( decStringList &list ){
	if( pProject && pProject->GetChanged() ){
		list.Add( pProject->GetFilePath() );
	}
}

bool projWindowMain::SaveDocument( const char *filename ){
	if( pProject && pProject->GetFilePath() == filename ){
		try{
			SaveProject();
			SaveProjectLocal();
			
		}catch( const deException &e ){
			DisplayException( e );
		}
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void projWindowMain::OnGameProjectChanged(){
	LoadProject();
}

void projWindowMain::UpdateShowActionPath(){
	const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
	
	decPath path;
	path.SetFromNative( gameProject.GetDirectoryPath() );
	path.AddUnixPath( gameProject.GetPathData() );
	pActionShowContent->SetPath( path.GetPathNative() );
	
	path.SetFromNative( gameProject.GetDirectoryPath() );
	path.AddUnixPath( "cache/testrun/config" );
	pActionShowConfig->SetPath( path.GetPathNative() );
	
	path.SetFromNative( gameProject.GetDirectoryPath() );
	path.AddUnixPath( "cache/testrun/overlay" );
	pActionShowOverlay->SetPath( path.GetPathNative() );
	
	path.SetFromNative( gameProject.GetDirectoryPath() );
	path.AddUnixPath( "cache/testrun/capture" );
	pActionShowCapture->SetPath( path.GetPathNative() );
	
	path.SetFromNative( gameProject.GetDirectoryPath() );
	path.AddComponent( "testRun.log" );
	pActionShowLogs->SetPath( path.GetPathNative() );
	
	const projProfile * const profile = pProject ? pProject->GetActiveProfile() : NULL;
	if( profile ){
		path.SetFromNative( gameProject.GetDirectoryPath() );
		path.AddUnixPath( profile->GetDelgaPath() );
		path.RemoveLastComponent();
		pActionShowDelga->SetPath( path.GetPathNative() );
		pActionShowDelga->SetEnabled( true );
		
	}else{
		pActionShowDelga->SetPath( "" );
		pActionShowDelga->SetEnabled( false );
	}
}

bool projWindowMain::ProcessCommandLine( decUnicodeStringList &arguments ){
	while( arguments.GetCount() > 0 ){
		const decString arg( arguments.GetAt( 0 ).ToUTF8() );
		
		if( arg == "--project.profile.distribute" ){
			arguments.RemoveFrom( 0 );
			return pCmdLineProfileDistribute( arguments );
			
		}else if( arg == "--project.profile.list" ){
			arguments.RemoveFrom( 0 );
			return pCmdLineProfileList( arguments );
			
		}else{
			break;
		}
	}
	return true;
}



// GUI Elements
/////////////////

void projWindowMain::ActivatePanelTestRun(){
	pTabPanels->SetActivePanel( epTestRun );
}

/*
void projWindowMain::SetProgressVisible( bool visible ){
	GetEnvironment()->SetProgressVisible( visible );
}

void projWindowMain::SetProgress( float progress ){
	GetEnvironment()->SetProgress( progress );
}

void projWindowMain::SetProgressText( const char *text ){
	GetEnvironment()->SetProgressText( text );
}
*/

/*
void projWindowMain::ChangePanel( int panel ){
	if( panel < epProfiles || panel > epUndoHistory ){
		DETHROW( deeInvalidParam );
	}
	
	if( panel == pSwitcher->getCurrent() ){
		return;
	}
	
	pSwitcher->setCurrent( panel );
	
	pBtnProfiles->setState( panel == epProfiles );
	pBtnTestRun->setState( panel == epTestRun );
	pBtnUndoHistory->setState( panel == epUndoHistory );
}
*/



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	projWindowMain &pWindow;
	
public:
	cActionBase( projWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( projWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
};


class cActionDistSave : public cActionBase{
public:
	cActionDistSave( projWindowMain &window ) : cActionBase( window,
		"Save", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ),
		"Save project", deInputEvent::esmControl,
		deInputEvent::ekcS, deInputEvent::ekcS ){}
	
	virtual void OnAction(){
		projProject * const project = pWindow.GetProject();
		if( ! project ){
			return;
		}
		if( project->GetChanged() ){
			pWindow.SaveProject();
		}
		pWindow.SaveProjectLocal();
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetProject() && pWindow.GetProject()->GetChanged() );
	}
};


class cActionEditCut : public cActionBase{
public:
	cActionEditCut( projWindowMain &window ) : cActionBase( window,
		"Cut", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( false );
	}
};


class cActionEditCopy : public cActionBase{
public:
	cActionEditCopy( projWindowMain &window ) : cActionBase( window,
		"Copy", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( false );
	}
};


class cActionEditPaste : public cActionBase{
public:
	cActionEditPaste( projWindowMain &window ) : cActionBase( window,
		"Paste", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( false );//pWindow.GetClipboard().HasClip() );
	}
};


class cActionProfileAdd : public cActionBase{
public:
	cActionProfileAdd( projWindowMain &window ) : cActionBase( window,
		"Add...", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
		"Add profile", deInputEvent::ekcA ){}
	
	virtual void OnAction(){
		projProject * const project = pWindow.GetProject();
		if( ! project ){
			return;
		}
		
		const projProfileList &list = project->GetProfiles();
		decString name( "Profile" );
		
		while( true ){
			if( ! igdeCommonDialogs::GetString( &pWindow, "Add Profile", "Name:", name ) ){
				return;
			}
			
			if( list.HasNamed( name ) ){
				igdeCommonDialogs::Error( &pWindow, "Add Profile",
					"A profile with this name exists already." );
				
			}else{
				break;
			}
		}
		
		const projProfile * const selectedProfile = project->GetActiveProfile();
		projProfile *safeProfile = NULL;
		projProfile *profile = NULL;
		igdeUndoReference undo;
		
		try{
			profile = new projProfile;
			profile->SetName( name );
			if( selectedProfile ){
				profile->SetIdentifier( selectedProfile->GetIdentifier() );
				profile->SetAliasIdentifier( selectedProfile->GetAliasIdentifier() );
				profile->SetGameObject( selectedProfile->GetGameObject() );
				profile->SetPathCapture( selectedProfile->GetPathCapture() );
				profile->SetPathConfig( selectedProfile->GetPathConfig() );
				profile->SetRequiredExtensions( selectedProfile->GetRequiredExtensions() );
				profile->SetScriptDirectory( selectedProfile->GetScriptDirectory() );
				profile->SetCreator( selectedProfile->GetCreator() );
				profile->SetWebsite( selectedProfile->GetWebsite() );
				
			}else{
				profile->SetIdentifier( decUuid::Random() );
			}
			undo.TakeOver( new projUProfileAdd( project, profile ) );
			safeProfile = profile;
			profile->FreeReference();
			profile = NULL;
			
			project->GetUndoSystem()->Add( undo );
			
		}catch( const deException &e ){
			if( profile ){
				profile->FreeReference();
			}
			pWindow.DisplayException( e );
			return;
		}
		
		project->SetActiveProfile( safeProfile );
	}
};


class cActionProfileRemove : public cActionBase{
public:
	cActionProfileRemove( projWindowMain &window ) : cActionBase( window,
		"Remove", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove selected profile", deInputEvent::ekcR ){}
	
	virtual void OnAction(){
		projProject * const project = pWindow.GetProject();
		if( ! project ){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if( ! profile ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new projUProfileRemove( project, profile ) );
		project->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile() );
	}
};


class cActionProfileDuplicate : public cActionBase{
public:
	cActionProfileDuplicate( projWindowMain &window ) : cActionBase( window,
		"Duplicate", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Duplicate profile", deInputEvent::ekcD ){}
	
	virtual void OnAction(){
		projProject * const project = pWindow.GetProject();
		if( ! project ){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if( ! profile ){
			return;
		}
		
		const projProfileList &list = project->GetProfiles();
		decString name;
		name.Format( "%s Copy", profile->GetName().GetString() );
		
		while( true ){
			if( ! igdeCommonDialogs::GetString( &pWindow, "Duplicate Profile", "Name:", name ) ){
				return;
			}
			
			if( list.HasNamed( name ) ){
				igdeCommonDialogs::Error( &pWindow, "Duplicate Profile",
					"A profile with this name exists already." );
				
			}else{
				break;
			}
		}
		
		projProfile *safeProfile = NULL;
		projProfile *duplicatedProfile = NULL;
		igdeUndoReference undo;
		
		try{
			duplicatedProfile = new projProfile( *profile );
			duplicatedProfile->SetName( name );
			undo.TakeOver( new projUProfileAdd( project, duplicatedProfile ) );
			safeProfile = duplicatedProfile;
			duplicatedProfile->FreeReference();
			duplicatedProfile = NULL;
			
			project->GetUndoSystem()->Add( undo );
			
		}catch( const deException &e ){
			if( duplicatedProfile ){
				duplicatedProfile->FreeReference();
			}
			pWindow.DisplayException( e );
			return;
		}
		
		project->SetActiveProfile( safeProfile );
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile() );
	}
};


class cActionProfileDistribute : public cActionBase{
public:
	cActionProfileDistribute( projWindowMain &window ) : cActionBase( window, "Build DELGA...",
		window.GetIconDelga(), "Build DELGA file using selected profile", deInputEvent::ekcD ){}
	
	virtual void OnAction(){
		projProject * const project = pWindow.GetProject();
		if( ! project || ! project->GetActiveProfile() ){
			return;
		}
		
		if( project->GetChanged() ){
			pWindow.SaveProject();
		}
		pWindow.SaveProjectLocal();
		
		if( ! pWindow.GetEnvironment().RequestSaveDocuments( "Distribute",
		"Unsaved changes are present. To projribute it is recommended to save them" ) ){
			return;
		}
		
		igdeDialogReference dialog;
		dialog.TakeOver( new projDialogDistribute( pWindow, project->GetActiveProfile() ) );
		dialog->Run( &pWindow );
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile() );
	}
};


class cActionProfileTestRun : public cActionBase{
public:
	cActionProfileTestRun( projWindowMain &window ) : cActionBase( window,
		"Test-Run...", window.GetIconStart(), "Test-Run selected profile", deInputEvent::ekcR ){}
	
	virtual void OnAction(){
		projProject * const project = pWindow.GetProject();
		if( ! project ){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if( ! profile ){
			return;
		}
		
		pWindow.ActivatePanelTestRun();
		pWindow.GetPanelTestRun().SelectProfile( profile );
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile() );
	}
};

}



// Private Functions
//////////////////////

void projWindowMain::pLoadIcons(){
	pIconStart.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/start.png" ) );
	pIconStop.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/stop.png" ) );
	pIconKill.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/kill.png" ) );
	pIconDelga.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/delga.png" ) );
}

void projWindowMain::pCreateActions(){
	igdeEnvironment &env = GetEnvironment();
	
	pActionDistSave.TakeOver( new cActionDistSave( *this ) );
	pActionEditUndo.TakeOver( new igdeActionUndo( env ) );
	pActionEditRedo.TakeOver( new igdeActionRedo( env ) );
	pActionEditCut.TakeOver( new cActionEditCut( *this ) );
	pActionEditCopy.TakeOver( new cActionEditCopy( *this ) );
	pActionEditPaste.TakeOver( new cActionEditPaste( *this ) );
	pActionProfileAdd.TakeOver( new cActionProfileAdd( *this ) );
	pActionProfileRemove.TakeOver( new cActionProfileRemove( *this ) );
	pActionProfileDuplicate.TakeOver( new cActionProfileDuplicate( *this ) );
	pActionProfileDistribute.TakeOver( new cActionProfileDistribute( *this ) );
	pActionProfileTestRun.TakeOver( new cActionProfileTestRun( *this ) );
	
	pActionShowDelga.TakeOver( new igdeActionExternOpen( env,
		"Browse DELGA", env.GetStockIcon( igdeEnvironment::esiOpen ),
		"Open DELGA Directory in File Manager" ) );
	
	pActionShowContent.TakeOver( new igdeActionExternOpen( env,
		"Browse Content", env.GetStockIcon( igdeEnvironment::esiOpen ),
		"Open Content directory in File Manager" ) );
	
	pActionShowConfig.TakeOver( new igdeActionExternOpen( env,
		"Browse Run Config", env.GetStockIcon( igdeEnvironment::esiOpen ),
		"Open Run-Time Configuration Directory in File Manager" ) );
	
	pActionShowOverlay.TakeOver( new igdeActionExternOpen( env,
		"Browse Run Overlay", env.GetStockIcon( igdeEnvironment::esiOpen ),
		"Open Run-Time Overlay Directory in File Manager" ) );
	
	pActionShowCapture.TakeOver( new igdeActionExternOpen( env,
		"Browse Run Capture", env.GetStockIcon( igdeEnvironment::esiOpen ),
		"Open Run-Time Capture Directory in File Manager" ) );
	
	pActionShowLogs.TakeOver( new igdeActionExternOpen( env,
		"Open Run Logs", env.GetStockIcon( igdeEnvironment::esiOpen ),
		"Open Run-Time Log File in External Application" ) );
}

void projWindowMain::pCreateToolBarDistribute(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBDistribute.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBDistribute, pActionDistSave );
	
	AddSharedToolBar( pTBDistribute );
}

void projWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBEdit, pActionEditUndo );
	helper.ToolBarButton( pTBEdit, pActionEditRedo );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarButton( pTBEdit, pActionEditCut );
	helper.ToolBarButton( pTBEdit, pActionEditCopy );
	helper.ToolBarButton( pTBEdit, pActionEditPaste );
	
	AddSharedToolBar( pTBEdit );
}

void projWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascadeReference cascade;
	
	cascade.TakeOver( new igdeMenuCascade( env, "Project", deInputEvent::ekcD ) );
	pCreateMenuDistribute( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Edit", deInputEvent::ekcE ) );
	pCreateMenuEdit( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Profile", deInputEvent::ekcP ) );
	pCreateMenuProfile( cascade );
	AddSharedMenu( cascade );
}

void projWindowMain::pCreateMenuDistribute( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionDistSave );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionShowContent );
}

void projWindowMain::pCreateMenuEdit( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEditUndo );
	helper.MenuCommand( menu, pActionEditRedo );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditCut );
	helper.MenuCommand( menu, pActionEditCopy );
	helper.MenuCommand( menu, pActionEditPaste );
}

void projWindowMain::pCreateMenuProfile( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionProfileAdd );
	helper.MenuCommand( menu, pActionProfileRemove );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionProfileDistribute );
	helper.MenuCommand( menu, pActionShowDelga );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionProfileTestRun );
	helper.MenuCommand( menu, pActionShowLogs );
	helper.MenuCommand( menu, pActionShowConfig );
	helper.MenuCommand( menu, pActionShowOverlay );
	helper.MenuCommand( menu, pActionShowCapture );
}

bool projWindowMain::pCmdLineProfileDistribute( decUnicodeStringList &arguments ){
	// --project.profile.distribute <profile>
	
	DEASSERT_NOTNULL( pProject );
	
	projProfile *profile = nullptr;
	
	while( arguments.GetCount() > 0 ){
		const decString arg( arguments.GetAt( 0 ).ToUTF8() );
		arguments.RemoveFrom( 0 );
		
		if( arg.BeginsWith( "--" ) || arg.BeginsWith( "-" ) ){
			decString message;
			message.Format( "Unknown argument '%s'", arg.GetString() );
			DETHROW_INFO( deeInvalidParam, message );
			
		}else if( ! profile ){
			profile = pProject->GetProfiles().GetNamed( arg );
			if( ! profile ){
				decString message;
				message.Format( "Unknown profile '%s'", arg.GetString() );
				DETHROW_INFO( deeInvalidParam, message );
			}
			
		}else{
			decString message;
			message.Format( "Unknown argument '%s'", arg.GetString() );
			DETHROW_INFO( deeInvalidParam, message );
		}
	}
	
	GetEnvironment().ActivateEditor( &GetEditorModule() );
	
	const projDialogDistribute::Ref dialog( projDialogDistribute::Ref::New(
		new projDialogDistribute( *this, profile ) ) );
	dialog->SetCloseDialogOnFinished( true );
	dialog->Run( this );
	return false;
}

bool projWindowMain::pCmdLineProfileList( decUnicodeStringList &arguments ){
	// --project.profile.list
	
	DEASSERT_NOTNULL( pProject );
	
	if( arguments.GetCount() > 0 ){
		decString message;
		message.Format( "Unknown argument '%s'", arguments.GetAt( 0 ).ToUTF8().GetString() );
		DETHROW_INFO( deeInvalidParam, message );
	}
	
	const int count = pProject->GetProfiles().GetCount();
	int i;
	
	#ifdef IGDE_CONSOLE_APP
		for( i=0; i<count; i++ ){
			printf( "%s\n", pProject->GetProfiles().GetAt( i )->GetName().GetString() );
		}
	#else
		decStringList names;
		for( i=0; i<count; i++ ){
			names.Add( pProject->GetProfiles().GetAt( i )->GetName() );
		}
		int selection = 0;
		igdeCommonDialogs::SelectString( this, "Profiles", "Profiles", names, selection );
	#endif
		
	return false;
}
