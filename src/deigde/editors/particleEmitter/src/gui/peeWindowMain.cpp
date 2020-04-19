/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeWindowMain.h"
#include "peeWindowMainListener.h"
#include "peeViewEmitter.h"
#include "peeTaskSyncGameDefinition.h"
#include "curves/peeWindowCurves.h"
#include "properties/peeWindowProperties.h"
#include "../peeIGDEModule.h"
#include "../configuration/peeConfiguration.h"
#include "../loadsave/peeLoadSaveSystem.h"
#include "../emitter/peeEmitter.h"

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
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>



// Class peeWindowMain
////////////////////////

// Constructor, destructor
////////////////////////////

peeWindowMain::peeWindowMain( igdeEditorModule &module ) :
igdeEditorWindow( module ),
pListener( NULL ),
pConfiguration( NULL ),
pLoadSaveSystem( NULL ),
pViewEmitter( NULL ),
pWindowProperties( NULL ),
pWindowCurves( NULL ),
pEmitter( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = new peeWindowMainListener( *this );
	pLoadSaveSystem = new peeLoadSaveSystem( *this );
	pConfiguration = new peeConfiguration( *this );
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplittedReference splitted;
	splitted.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espLeft, 360 ) );
	AddChild( splitted );
	
	pWindowProperties = new peeWindowProperties( *this );
	splitted->AddChild( pWindowProperties, igdeContainerSplitted::eaSide );
	
	igdeContainerSplittedReference splitted2;
	splitted2.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espBottom, 260 ) );
	splitted->AddChild( splitted2, igdeContainerSplitted::eaCenter );
	
	pWindowCurves = new peeWindowCurves( *this );
	splitted2->AddChild( pWindowCurves, igdeContainerSplitted::eaSide );
	
	pViewEmitter = new peeViewEmitter( *this );
	splitted2->AddChild( pViewEmitter, igdeContainerSplitted::eaCenter );
	
	CreateNewEmitter();
	ResetViews();
}

peeWindowMain::~peeWindowMain(){
	if( pConfiguration ){
		pConfiguration->SaveConfiguration();
	}
	
	SetEmitter( NULL );
	
	if( pWindowProperties ){
		pWindowProperties->FreeReference();
	}
	if( pWindowCurves ){
		pWindowCurves->FreeReference();
	}
	if( pViewEmitter ){
		pViewEmitter->FreeReference();
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

bool peeWindowMain::QuitRequest(){
	return true;
}

void peeWindowMain::ResetViews(){
	pViewEmitter->ResetView();
}



void peeWindowMain::SetEmitter( peeEmitter *emitter ){
	if( emitter == pEmitter ){
		return;
	}
	
	pViewEmitter->SetEmitter( NULL );
	pWindowProperties->SetEmitter( NULL );
	pWindowCurves->SetEmitter( NULL );
	pActionEditUndo->SetUndoSystem( NULL );
	pActionEditRedo->SetUndoSystem( NULL );
	
	if( pEmitter ){
		pEmitter->RemoveListener( pListener );
		
		pEmitter->Dispose();
		pEmitter->FreeReference();
	}
	
	pEmitter = emitter;
	
	if( emitter ){
		emitter->AddReference();
		emitter->AddListener( pListener );
		
		pActionEditUndo->SetUndoSystem( emitter->GetUndoSystem() );
		pActionEditRedo->SetUndoSystem( emitter->GetUndoSystem() );
	}
	
	pViewEmitter->SetEmitter( emitter );
	pWindowProperties->SetEmitter( emitter );
	pWindowCurves->SetEmitter( emitter );
}

void peeWindowMain::CreateNewEmitter(){
	peeEmitter *emitter = NULL;
	
	try{
		emitter = new peeEmitter( &GetEnvironment(), *pLoadSaveSystem );
		
		SetEmitter( emitter );
		emitter->FreeReference();
		
	}catch( const deException & ){
		if( emitter ){
			emitter->FreeReference();
		}
		throw;
	}
}

void peeWindowMain::SaveEmitter( const char *filename ){
	GetEditorModule().LogInfoFormat( "Saving Emitter %s", filename );
	pLoadSaveSystem->SaveEmitter( pEmitter, filename );
	
	pEmitter->SetFilePath( filename );
	pEmitter->SetChanged( false );
	pEmitter->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}



void peeWindowMain::OnBeforeEngineStart(){
}

void peeWindowMain::OnAfterEngineStart(){
	pViewEmitter->OnAfterEngineStart();
}

void peeWindowMain::OnBeforeEngineStop(){
	pViewEmitter->OnBeforeEngineStop();
}

void peeWindowMain::OnAfterEngineStop(){
}



void peeWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pViewEmitter->SetEnableRendering( true );
}

void peeWindowMain::OnDeactivate(){
	pViewEmitter->SetEnableRendering( false );
	igdeEditorWindow::OnDeactivate();
}



void peeWindowMain::OnFrameUpdate( float elapsed ){
	if( ! GetActiveModule() ){
		return;
	}
	
	pViewEmitter->OnFrameUpdate( elapsed );
}



void peeWindowMain::GetChangedDocuments( decStringList &list ){
	if( pEmitter && pEmitter->GetChanged() ){
		list.Add( pEmitter->GetFilePath() );
	}
}

void peeWindowMain::LoadDocument( const char *filename ){
	peeEmitter *emitter = pLoadSaveSystem->LoadEmitter( filename );
	
	SetEmitter( emitter );
	emitter->FreeReference();
	
	emitter->SetChanged( false );
	emitter->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}

bool peeWindowMain::SaveDocument( const char *filename ){
	if( pEmitter && pEmitter->GetFilePath().Equals( filename ) ){
		pActionEmitterSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void peeWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void peeWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewEmitter();
}

igdeStepableTask *peeWindowMain::OnGameDefinitionChanged(){
	return new peeTaskSyncGameDefinition( *this );
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	peeWindowMain &pWindow;
	
public:
	cActionBase( peeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( peeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
};


class cActionEmitterNew : public cActionBase{
public:
	cActionEmitterNew( peeWindowMain &window ) : cActionBase( window, "New",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ), "Creates a new emitter",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN ){}
	
	virtual void OnAction(){
		if( igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "New Emitter",
		"Creating a new emitter discarding the current one is that ok?" ) == igdeCommonDialogs::ebYes ){
			pWindow.CreateNewEmitter();
		}
	}
};


class cActionEmitterOpen : public cActionBase{
public:
	cActionEmitterOpen( peeWindowMain &window ) : cActionBase( window, "Open...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Opens a emitter from file",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetEmitter()->GetFilePath() );
		if( ! igdeCommonDialogs::GetFileOpen( &pWindow, "Open Emitter",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetEmitterFilePatterns(), filename ) ){
			return;
		}
		
		// load emitter
		pWindow.GetEditorModule().LogInfoFormat( "Loading emitter %s", filename.GetString() );
		peeEmitter *emitter = pWindow.GetLoadSaveSystem().LoadEmitter( filename );
		
		// replace emitter
		pWindow.SetEmitter( emitter );
		emitter->FreeReference();
		
		// store informations
		emitter->SetChanged( false );
		emitter->SetSaved( true );
		pWindow.GetRecentFiles().AddFile( filename );
	}
};


class cActionEmitterSaveAs : public cActionBase{
public:
	cActionEmitterSaveAs( peeWindowMain &window ) : cActionBase( window, "Save As...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSaveAs ),
		"Saves the emitter under a differen file", deInputEvent::ekcA ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetEmitter()->GetFilePath() );
		if( igdeCommonDialogs::GetFileSave( &pWindow, "Save Emitter",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetEmitterFilePatterns(), filename ) ){
			pWindow.SaveEmitter( filename );
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetEmitter() );
	}
};


class cActionEmitterSave : public cActionEmitterSaveAs{
public:
	cActionEmitterSave( peeWindowMain &window ) : cActionEmitterSaveAs( window ){
		SetText( "Save" );
		SetDescription( "Saves the emitter to file" );
		SetHotKey( igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcS ) );
		SetMnemonic( deInputEvent::ekcS );
		SetIcon( window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ) );
	}
	
	virtual void OnAction(){
		peeEmitter &emitter = *pWindow.GetEmitter();
		
		if( emitter.GetSaved() ){
			if( emitter.GetChanged() ){
				pWindow.SaveEmitter( emitter.GetFilePath() );
			}
			
		}else{
			cActionEmitterSaveAs::OnAction();
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetEmitter() && pWindow.GetEmitter()->GetChanged() );
	}
};



class cActionEditCut : public cActionBase{
public:
	cActionEditCut( peeWindowMain &window ) : cActionBase( window,
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
	cActionEditCopy( peeWindowMain &window ) : cActionBase( window,
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
	cActionEditPaste( peeWindowMain &window ) : cActionBase( window,
		"Paste", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( false /*pWindow.GetClipboard().HasClip()*/ );
	}
};

}



// Private Functions
//////////////////////

void peeWindowMain::pLoadIcons(){
	//pIconEmitterNew.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/file_new.png" ) );
}

void peeWindowMain::pCreateActions(){
	pActionEmitterNew.TakeOver( new cActionEmitterNew( *this ) );
	pActionEmitterOpen.TakeOver( new cActionEmitterOpen( *this ) );
	pActionEmitterSave.TakeOver( new cActionEmitterSave( *this ) );
	pActionEmitterSaveAs.TakeOver( new cActionEmitterSaveAs( *this ) );
	pActionEditUndo.TakeOver( new igdeActionUndo( GetEnvironment() ) );
	pActionEditRedo.TakeOver( new igdeActionRedo( GetEnvironment() ) );
	pActionEditCut.TakeOver( new cActionEditCut( *this ) );
	pActionEditCopy.TakeOver( new cActionEditCopy( *this ) );
	pActionEditPaste.TakeOver( new cActionEditPaste( *this ) );
	
	
	// register for updating
	AddUpdateAction( pActionEmitterNew );
	AddUpdateAction( pActionEmitterOpen );
	AddUpdateAction( pActionEmitterSave );
	AddUpdateAction( pActionEmitterSaveAs );
	AddUpdateAction( pActionEditUndo );
	AddUpdateAction( pActionEditRedo );
	AddUpdateAction( pActionEditCut );
	AddUpdateAction( pActionEditCopy );
	AddUpdateAction( pActionEditPaste );
}

void peeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBFile, pActionEmitterNew );
	helper.ToolBarButton( pTBFile, pActionEmitterOpen );
	helper.ToolBarButton( pTBFile, pActionEmitterSave );
	
	AddSharedToolBar( pTBFile );
}

void peeWindowMain::pCreateToolBarEdit(){
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

void peeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascadeReference cascade;
	
	cascade.TakeOver( new igdeMenuCascade( env, "Emitter", deInputEvent::ekcM ) );
	pCreateMenuEmitter( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Edit", deInputEvent::ekcE ) );
	pCreateMenuEdit( cascade );
	AddSharedMenu( cascade );
}

void peeWindowMain::pCreateMenuEmitter( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEmitterNew );
	helper.MenuCommand( menu, pActionEmitterOpen );
	helper.MenuRecentFiles( menu, GetRecentFiles() );
	helper.MenuCommand( menu, pActionEmitterSave );
	helper.MenuCommand( menu, pActionEmitterSaveAs );
}

void peeWindowMain::pCreateMenuEdit( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEditUndo );
	helper.MenuCommand( menu, pActionEditRedo );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditCut );
	helper.MenuCommand( menu, pActionEditCopy );
	helper.MenuCommand( menu, pActionEditPaste );
}
