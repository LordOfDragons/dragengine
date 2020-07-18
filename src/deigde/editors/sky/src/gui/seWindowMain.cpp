/* 
 * Drag[en]gine IGDE Sky Editor
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seWindowMain.h"
#include "seWindowMainListener.h"
#include "seViewSky.h"
#include "seTaskSyncGameDefinition.h"
#include "curves/seWindowCurves.h"
#include "properties/seWindowProperties.h"
#include "../seIGDEModule.h"
#include "../configuration/seConfiguration.h"
#include "../sky/seSky.h"
#include "../sky/controller/seController.h"
#include "../loadsave/seLoadSaveSystem.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/layout/igdeContainerSplittedReference.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class seWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

seWindowMain::seWindowMain( igdeEditorModule &module ) :
igdeEditorWindow( module ),
pListener( NULL ),
pLoadSaveSystem( NULL ),
pViewSky( NULL ),
pWindowProperties( NULL ),
pWindowCurves( NULL ),
pSky( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = new seWindowMainListener( *this );
	pLoadSaveSystem = new seLoadSaveSystem( *this );
	pConfiguration = new seConfiguration( *this );
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplittedReference splitted;
	splitted.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espLeft, 300 ) );
	AddChild( splitted );
	
	pWindowProperties = new seWindowProperties( *this );
	splitted->AddChild( pWindowProperties, igdeContainerSplitted::eaSide );
	
	igdeContainerSplittedReference splitted2;
	splitted2.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espBottom, 260 ) );
	splitted->AddChild( splitted2, igdeContainerSplitted::eaCenter );
	
	pWindowCurves = new seWindowCurves( *this );
	splitted2->AddChild( pWindowCurves, igdeContainerSplitted::eaSide );
	
	pViewSky = new seViewSky( *this );
	splitted2->AddChild( pViewSky, igdeContainerSplitted::eaCenter );
	
	CreateNewSky();
	ResetViews();
}

seWindowMain::~seWindowMain(){
	if( pConfiguration ){
		pConfiguration->SaveConfiguration();
	}
	
	SetSky( NULL );
	
	if( pWindowCurves ){
		pWindowCurves->FreeReference();
		pWindowCurves = NULL;
	}
	if( pViewSky ){
		pViewSky->FreeReference();
		pViewSky = NULL;
	}
	if( pWindowProperties ){
		pWindowProperties->FreeReference();
		pWindowProperties = NULL;
	}
	
	if( pConfiguration ){
		delete pConfiguration;
	}
	if( pLoadSaveSystem ){
		delete pLoadSaveSystem;
	}
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

bool seWindowMain::QuitRequest(){
	return true;
}

void seWindowMain::ResetViews(){
	pViewSky->ResetView();
}



void seWindowMain::SetSky( seSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	pViewSky->SetSky( NULL );
	pWindowProperties->SetSky( NULL );
	pWindowCurves->SetSky( NULL );
	pActionEditUndo->SetUndoSystem( NULL );
	pActionEditRedo->SetUndoSystem( NULL );
	
	if( pSky ){
		pSky->RemoveListener( pListener );
		
		pSky->Dispose();
		pSky->FreeReference();
	}
	
	pSky = sky;
	
	if( sky ){
		sky->AddReference();
		sky->AddListener( pListener );
		
		pActionEditUndo->SetUndoSystem( sky->GetUndoSystem() );
		pActionEditRedo->SetUndoSystem( sky->GetUndoSystem() );
	}
	
	pViewSky->SetSky( sky );
	pWindowProperties->SetSky( sky );
	pWindowCurves->SetSky( sky );
}

void seWindowMain::CreateNewSky(){
	seSky *sky = NULL;
	
	try{
		sky = new seSky( &GetEnvironment() );
		
		SetSky( sky );
		sky->FreeReference();
		
	}catch( const deException & ){
		if( sky ){
			sky->FreeReference();
		}
		throw;
	}
}

void seWindowMain::SaveSky( const char *filename ){
	GetEditorModule().LogInfoFormat( "Saving sky %s", filename );
	pLoadSaveSystem->SaveSky( pSky, filename );
	
	pSky->SetFilePath( filename );
	pSky->UpdateRelativeResources();
	pSky->SetChanged( false );
	pSky->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}



void seWindowMain::OnBeforeEngineStart(){
}

void seWindowMain::OnAfterEngineStart(){
	pViewSky->OnAfterEngineStart();
}

void seWindowMain::OnBeforeEngineStop(){
	pViewSky->OnBeforeEngineStop();
}

void seWindowMain::OnAfterEngineStop(){
}



void seWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pViewSky->SetEnableRendering( true );
}

void seWindowMain::OnDeactivate(){
	pViewSky->SetEnableRendering( false );
	igdeEditorWindow::OnDeactivate();
}



void seWindowMain::OnFrameUpdate( float elapsed ){
	if( ! GetActiveModule() ){
		return;
	}
	
	pViewSky->OnFrameUpdate( elapsed );
}



void seWindowMain::GetChangedDocuments( decStringList &list ){
	if( pSky && pSky->GetChanged() ){
		list.Add( pSky->GetFilePath() );
	}
}

void seWindowMain::LoadDocument( const char *filename ){
	seSky * const sky = pLoadSaveSystem->LoadSky( filename );
	SetSky( sky );
	sky->FreeReference();
	
	GetRecentFiles().AddFile( filename );
}

bool seWindowMain::SaveDocument( const char *filename ){
	if( pSky && pSky->GetFilePath().Equals( filename ) ){
		pActionSkySave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void seWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void seWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewSky();
}

igdeStepableTask *seWindowMain::OnGameDefinitionChanged(){
	return new seTaskSyncGameDefinition( *this );
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	seWindowMain &pWindow;
	
public:
	cActionBase( seWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( seWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
};


class cActionSkyNew : public cActionBase{
public:
	cActionSkyNew( seWindowMain &window ) : cActionBase( window, "New",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ), "Creates a new sky",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN ){}
	
	virtual void OnAction(){
		if( igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "New Sky",
		"Creating a new sky discarding the current one is that ok?" ) == igdeCommonDialogs::ebYes ){
			pWindow.CreateNewSky();
		}
	}
};


class cActionSkyOpen : public cActionBase{
public:
	cActionSkyOpen( seWindowMain &window ) : cActionBase( window, "Open...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Opens a sky from file",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetSky()->GetFilePath() );
		if( ! igdeCommonDialogs::GetFileOpen( &pWindow, "Open Sky",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetSkyFilePatterns(), filename ) ){
			return;
		}
		
		// load sky
		pWindow.GetEditorModule().LogInfoFormat( "Loading sky %s", filename.GetString() );
		seSky *sky = pWindow.GetLoadSaveSystem().LoadSky( filename );
		
		// replace sky
		pWindow.SetSky( sky );
		sky->FreeReference();
		
		// store information
		sky->SetFilePath( filename );
		sky->SetChanged( false );
		sky->SetSaved( true );
		pWindow.GetRecentFiles().AddFile( filename );
	}
};


class cActionSkySaveAs : public cActionBase{
public:
	cActionSkySaveAs( seWindowMain &window ) : cActionBase( window, "Save As...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSaveAs ),
		"Saves the sky under a differen file", deInputEvent::ekcA ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetSky()->GetFilePath() );
		if( igdeCommonDialogs::GetFileSave( &pWindow, "Save Sky",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetSkyFilePatterns(), filename ) ){
			pWindow.SaveSky( filename );
		}
	}
};


class cActionSkySave : public cActionSkySaveAs{
public:
	cActionSkySave( seWindowMain &window ) : cActionSkySaveAs( window ){
		SetText( "Save" );
		SetDescription( "Saves the sky to file" );
		SetHotKey( igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcS ) );
		SetMnemonic( deInputEvent::ekcS );
		SetIcon( window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ) );
	}
	
	virtual void OnAction(){
		seSky &sky = *pWindow.GetSky();
		
		if( sky.GetSaved() ){
			if( sky.GetChanged() ){
				pWindow.SaveSky( sky.GetFilePath() );
			}
			
		}else{
			cActionSkySaveAs::OnAction();
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetSky()->GetChanged() );
	}
};



class cActionEditCut : public cActionBase{
public:
	cActionEditCut( seWindowMain &window ) : cActionBase( window,
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
	cActionEditCopy( seWindowMain &window ) : cActionBase( window,
		"Copy", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( false );
	}
};


class cActionEditPaste : public cActionBase{
public:
	cActionEditPaste( seWindowMain &window ) : cActionBase( window,
		"Paste", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( false /*pWindow.GetClipboard().HasClip()*/ );
	}
};


class cActionViewShowCompass : public cActionBase{
public:
	cActionViewShowCompass( seWindowMain &window ) : cActionBase( window,
		"Show Sky-Compass", NULL, "Shows/Hides the Sky Compass", deInputEvent::ekcC ){}
	
	virtual void OnAction(){
		seSky * const sky = pWindow.GetSky();
		if( sky ){
			sky->SetDrawSkyCompass( ! sky->GetDrawSkyCompass() );
		}
	}
	
	virtual void Update(){
		const seSky * const sky = pWindow.GetSky();
		SetEnabled( sky != NULL );
		SetSelected( sky != NULL && sky->GetDrawSkyCompass() );
	}
};

}



// Private Functions
//////////////////////

void seWindowMain::pLoadIcons(){
	//pIconSkyNew.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/file_new.png" ) );
}

void seWindowMain::pCreateActions(){
	pActionSkyNew.TakeOver( new cActionSkyNew( *this ) );
	pActionSkyOpen.TakeOver( new cActionSkyOpen( *this ) );
	pActionSkySave.TakeOver( new cActionSkySave( *this ) );
	pActionSkySaveAs.TakeOver( new cActionSkySaveAs( *this ) );
	pActionEditUndo.TakeOver( new igdeActionUndo( GetEnvironment() ) );
	pActionEditRedo.TakeOver( new igdeActionRedo( GetEnvironment() ) );
	pActionEditCut.TakeOver( new cActionEditCut( *this ) );
	pActionEditCopy.TakeOver( new cActionEditCopy( *this ) );
	pActionEditPaste.TakeOver( new cActionEditPaste( *this ) );
	pActionViewShowCompass.TakeOver( new cActionViewShowCompass( *this ) );
	
	
	// register for updating
	AddUpdateAction( pActionSkyNew );
	AddUpdateAction( pActionSkyOpen );
	AddUpdateAction( pActionSkySave );
	AddUpdateAction( pActionSkySaveAs );
	AddUpdateAction( pActionEditUndo );
	AddUpdateAction( pActionEditRedo );
	AddUpdateAction( pActionEditCut );
	AddUpdateAction( pActionEditCopy );
	AddUpdateAction( pActionEditPaste );
	AddUpdateAction( pActionViewShowCompass );
}

void seWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBFile, pActionSkyNew );
	helper.ToolBarButton( pTBFile, pActionSkyOpen );
	helper.ToolBarButton( pTBFile, pActionSkySave );
	
	AddSharedToolBar( pTBFile );
}

void seWindowMain::pCreateToolBarEdit(){
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

void seWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascadeReference cascade;
	
	cascade.TakeOver( new igdeMenuCascade( env, "Sky", deInputEvent::ekcS ) );
	pCreateMenuSky( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Edit", deInputEvent::ekcE ) );
	pCreateMenuEdit( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "View", deInputEvent::ekcV ) );
	pCreateMenuView( cascade );
	AddSharedMenu( cascade );
}

void seWindowMain::pCreateMenuSky( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionSkyNew );
	helper.MenuCommand( menu, pActionSkyOpen );
	helper.MenuRecentFiles( menu, GetRecentFiles() );
	helper.MenuCommand( menu, pActionSkySave );
	helper.MenuCommand( menu, pActionSkySaveAs );
}

void seWindowMain::pCreateMenuEdit( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEditUndo );
	helper.MenuCommand( menu, pActionEditRedo );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditCut );
	helper.MenuCommand( menu, pActionEditCopy );
	helper.MenuCommand( menu, pActionEditPaste );
}

void seWindowMain::pCreateMenuView( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck( menu, pActionViewShowCompass );
}
