/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include "lpeWindowMain.h"
#include "lpeWindowMainListener.h"
#include "lpeViewLangPack.h"
#include "properties/lpeWindowProperties.h"
#include "../lpeIGDEModule.h"
#include "../configuration/lpeConfiguration.h"
#include "../loadsave/lpeLoadSaveSystem.h"
#include "../langpack/lpeLangPack.h"
#include "../langpack/entry/lpeLangPackEntry.h"
#include "../undosys/entry/lpeULangPackEntryRemove.h"
#include "../undosys/entry/lpeULangPackEntryAdd.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
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
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>



// Class lpeWindowMain
////////////////////////

// Constructor, destructor
////////////////////////////

lpeWindowMain::lpeWindowMain( igdeEditorModule &module ) :
igdeEditorWindow( module ),
pListener( new lpeWindowMainListener( *this ) ),
pLoadSaveSystem( NULL ),
pWindowProperties( NULL ),
pViewLangPack( NULL ),
pLangPack( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pLoadSaveSystem = new lpeLoadSaveSystem( this );
	pConfiguration = new lpeConfiguration( *this );
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplittedReference splitted;
	splitted.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espLeft, 320 ) );
	AddChild( splitted );
	
	pWindowProperties = new lpeWindowProperties( *this );
	splitted->AddChild( pWindowProperties, igdeContainerSplitted::eaSide );
	
	pViewLangPack = new lpeViewLangPack( *this );
	splitted->AddChild( pViewLangPack, igdeContainerSplitted::eaCenter );
	
	CreateNewLangPack();
}

lpeWindowMain::~lpeWindowMain(){
	if( pConfiguration ){
		pConfiguration->SaveConfiguration();
	}
	
	SetLangPack( NULL );
	
	if( pViewLangPack ){
		pViewLangPack->FreeReference();
		pViewLangPack = NULL;
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

bool lpeWindowMain::QuitRequest(){
	return true;
}



void lpeWindowMain::SetLangPack( lpeLangPack *langpack ){
	if( langpack == pLangPack ){
		return;
	}
	
	pWindowProperties->SetLangPack( NULL );
	pViewLangPack->SetLangPack( NULL );
	pActionEditUndo->SetUndoSystem( NULL );
	pActionEditRedo->SetUndoSystem( NULL );
	
	if( pLangPack ){
		pLangPack->RemoveListener( pListener );
		pLangPack->FreeReference();
	}
	
	pLangPack = langpack;
	
	if( langpack ){
		langpack->AddReference();
		langpack->AddListener( pListener );
		
		pActionEditUndo->SetUndoSystem( langpack->GetUndoSystem() );
		pActionEditRedo->SetUndoSystem( langpack->GetUndoSystem() );
	}
	
	pViewLangPack->SetLangPack( langpack );
	pWindowProperties->SetLangPack( langpack );
}

void lpeWindowMain::CreateNewLangPack(){
	lpeLangPack *langpack = NULL;
	
	try{
		langpack = new lpeLangPack( &GetEnvironment() );
		
		SetLangPack( langpack );
		langpack->FreeReference();
		
	}catch( const deException & ){
		if( langpack ){
			langpack->FreeReference();
		}
		throw;
	}
}

void lpeWindowMain::SaveLangPack( const char *filename ){
	if( ! pLangPack ){
		return;
	}
	
	GetEditorModule().LogInfoFormat( "Saving Language Pack %s", filename );
	pLoadSaveSystem->SaveLangPack( pLangPack, filename );
	
	pLangPack->SetFilePath( filename );
	pLangPack->SetChanged( false );
	pLangPack->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}

lpeLangPack *lpeWindowMain::GetReferenceLangPack() const{
	return pViewLangPack->GetReferenceLangPack();
}

void lpeWindowMain::SetReferenceLangPack( lpeLangPack *langpack ){
	pViewLangPack->SetReferenceLangPack( langpack );
}

void lpeWindowMain::SelectNextMissingEntry(){
	pViewLangPack->SelectNextMissingEntry();
}



void lpeWindowMain::GetChangedDocuments( decStringList &list ){
	if( pLangPack && pLangPack->GetChanged() ){
		list.Add( pLangPack->GetFilePath() );
	}
}

void lpeWindowMain::LoadDocument( const char *filename ){
	lpeLangPack * const langpack = pLoadSaveSystem->LoadLangPack( filename );
	
	SetLangPack( langpack );
	langpack->FreeReference();
	
	langpack->SetFilePath( filename );
	langpack->SetChanged( false );
	langpack->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}

bool lpeWindowMain::SaveDocument( const char *filename ){
	if( pLangPack && pLangPack->GetFilePath() == filename ){
		pActionLangPackSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void lpeWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void lpeWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewLangPack();
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	lpeWindowMain &pWindow;
	
public:
	cActionBase( lpeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( lpeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
};


class cActionLangPackNew : public cActionBase{
public:
	cActionLangPackNew( lpeWindowMain &window ) : cActionBase( window, "New",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ), "Creates new language pack",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN ){}
	
	virtual void OnAction(){
		if( igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "New Language Pack",
		"Creating new language pack discarding current one. Is that ok?" ) == igdeCommonDialogs::ebYes ){
			pWindow.CreateNewLangPack();
		}
	}
};


class cActionLangPackOpen : public cActionBase{
public:
	cActionLangPackOpen( lpeWindowMain &window ) : cActionBase( window, "Open...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Opens language pack from file",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetLangPack()->GetFilePath() );
		if( ! igdeCommonDialogs::GetFileOpen( &pWindow, "Open Language Pack",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetLangPackFPList(), filename ) ){
			return;
		}
		
		// load language pack
		pWindow.GetEditorModule().LogInfoFormat( "Loading language pack %s", filename.GetString() );
		lpeLangPack * const langpack = pWindow.GetLoadSaveSystem().LoadLangPack( filename );
		
		// replace language pack
		pWindow.SetLangPack( langpack );
		langpack->FreeReference();
		
		// store information
		langpack->SetFilePath( filename );
		langpack->SetChanged( false );
		langpack->SetSaved( true );
		pWindow.GetRecentFiles().AddFile( filename );
	}
};


class cActionLangPackSaveAs : public cActionBase{
public:
	cActionLangPackSaveAs( lpeWindowMain &window ) : cActionBase( window, "Save As...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSaveAs ),
		"Saves the font under a differen file", deInputEvent::ekcA ){}
	
	virtual void OnAction(){
		if( ! pWindow.GetLangPack() ){
			return;
		}
		
		decString filename( pWindow.GetLangPack()->GetFilePath() );
		if( igdeCommonDialogs::GetFileSave( &pWindow, "Save Language Pack",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetLangPackFPList(), filename ) ){
			pWindow.SaveLangPack( filename );
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetLangPack() );
	}
};


class cActionLangPackSave : public cActionLangPackSaveAs{
public:
	cActionLangPackSave( lpeWindowMain &window ) : cActionLangPackSaveAs( window ){
		SetText( "Save" );
		SetDescription( "Saves the font to file" );
		SetHotKey( igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcS ) );
		SetMnemonic( deInputEvent::ekcS );
		SetIcon( window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ) );
	}
	
	virtual void OnAction(){
		if( ! pWindow.GetLangPack() ){
			return;
		}
		
		lpeLangPack &langpack = *pWindow.GetLangPack();
		
		if( langpack.GetSaved() ){
			if( langpack.GetChanged() ){
				pWindow.SaveLangPack( langpack.GetFilePath() );
			}
			
		}else{
			cActionLangPackSaveAs::OnAction();
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetLangPack() && pWindow.GetLangPack()->GetChanged() );
	}
};


class cActionLangPackOpenRef : public cActionBase{
public:
	cActionLangPackOpenRef( lpeWindowMain &window ) : cActionBase( window, "Open Reference...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ),
		"Opens reference language pack from file", deInputEvent::ekcR ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetLangPack()->GetFilePath() );
		if( ! igdeCommonDialogs::GetFileOpen( &pWindow, "Open Reference Language Pack",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetLangPackFPList(), filename ) ){
			return;
		}
		
		pWindow.GetEditorModule().LogInfoFormat( "Loading language pack %s", filename.GetString() );
		pWindow.SetReferenceLangPack( lpeLangPack::Ref::New(
			pWindow.GetLoadSaveSystem().LoadLangPack( filename ) ) );
	}
};



class cActionEditCut : public cActionBase{
public:
	cActionEditCut( lpeWindowMain &window ) : cActionBase( window,
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
	cActionEditCopy( lpeWindowMain &window ) : cActionBase( window,
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
	cActionEditPaste( lpeWindowMain &window ) : cActionBase( window,
		"Paste", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetClipboard().GetCount() > 0 );
		// HasWithTypeName( "..." )
	}
};



class cActionEntryAdd : public cActionBase{
public:
	cActionEntryAdd( lpeWindowMain &window ) : cActionBase( window,
		"Add entry", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
		"Add language pack entry", deInputEvent::ekcA ){}
	
	virtual void OnAction(){
		lpeLangPack * const langpack = pWindow.GetLangPack();
		if( ! langpack ){
			return;
		}
		
		decString name( langpack->GetEntrySelection().GetActive()
			? langpack->GetEntrySelection().GetActive()->GetName() : decString( "Entry" ) );
		
		while( igdeCommonDialogs::GetString( &pWindow, "Add Entry", "Identifier:", name ) ){
			if( langpack->GetEntryList().HasNamed( name ) ){
				igdeCommonDialogs::Error( &pWindow, "Add Entry",
					"There exists already an entry with this name" );
				continue;
			}
			
			igdeUndoReference undo;
			const lpeLangPackEntry::Ref entry( lpeLangPackEntry::Ref::New( new lpeLangPackEntry ) );
			entry->SetName( name );
			
			const lpeLangPack * const refLangPack = pWindow.GetReferenceLangPack();
			lpeLangPackEntry *refEntry = nullptr;
			if( refLangPack ){
				refEntry = refLangPack->GetEntryList().GetNamed( name );
			}
			
			undo.TakeOver( new lpeULangPackEntryAdd( langpack, entry, refEntry ) );
			
			langpack->GetUndoSystem()->Add( undo );
			return;
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetLangPack() != nullptr );
	}
};


class cActionEntryRemove : public cActionBase{
public:
	cActionEntryRemove( lpeWindowMain &window ) : cActionBase( window,
		"Remove entry", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove selected language pack entry", deInputEvent::ekcR ){}
	
	virtual void OnAction(){
		lpeLangPack * const langpack = pWindow.GetLangPack();
		if( ! langpack ){
			return;
		}
		
		const lpeLangPackEntryList &selection = langpack->GetEntrySelection().GetSelected();
		const int count = selection.GetCount();
		lpeLangPackEntryList list;
		int i;
		
		for( i=0; i<count; i++ ){
			lpeLangPackEntry * const entry = selection.GetAt( i );
			if( entry->GetLangPack() == langpack ){
				list.Add( entry );
			}
		}
		
		if( list.GetCount() == 0 ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new lpeULangPackEntryRemove( langpack, list, pWindow.GetReferenceLangPack() ) );
		langpack->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetLangPack() && pWindow.GetLangPack()
			->GetEntrySelection().GetSelected().GetCount() > 0 );
	}
};


class cActionEntryNextMissing : public cActionBase{
public:
	cActionEntryNextMissing( lpeWindowMain &window ) : cActionBase( window,
		"Select next missing", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiWarning ),
		"Select next missing language pack entry", deInputEvent::ekcM ){}
	
	virtual void OnAction(){
		if( pWindow.GetLangPack() ){
			pWindow.SelectNextMissingEntry();
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetLangPack() );
	}
};

};



// Private Functions
//////////////////////

void lpeWindowMain::pLoadIcons(){
	//pIconLangPackNew.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/file_new.png" ) );
}

void lpeWindowMain::pCreateActions(){
	pActionLangPackNew.TakeOver( new cActionLangPackNew( *this ) );
	pActionLangPackOpen.TakeOver( new cActionLangPackOpen( *this ) );
	pActionLangPackSave.TakeOver( new cActionLangPackSave( *this ) );
	pActionLangPackSaveAs.TakeOver( new cActionLangPackSaveAs( *this ) );
	pActionLangPackOpenRef.TakeOver( new cActionLangPackOpenRef( *this ) );
	pActionEditUndo.TakeOver( new igdeActionUndo( GetEnvironment() ) );
	pActionEditRedo.TakeOver( new igdeActionRedo( GetEnvironment() ) );
	pActionEditCut.TakeOver( new cActionEditCut( *this ) );
	pActionEditCopy.TakeOver( new cActionEditCopy( *this ) );
	pActionEditPaste.TakeOver( new cActionEditPaste( *this ) );
	pActionEntryAdd.TakeOver( new cActionEntryAdd( *this ) );
	pActionEntryRemove.TakeOver( new cActionEntryRemove( *this ) );
	pActionEntryNextMissing.TakeOver( new cActionEntryNextMissing( *this ) );
	
	
	// register for updating
	AddUpdateAction( pActionLangPackNew );
	AddUpdateAction( pActionLangPackOpen );
	AddUpdateAction( pActionLangPackSave );
	AddUpdateAction( pActionLangPackSaveAs );
	AddUpdateAction( pActionLangPackOpenRef );
	AddUpdateAction( pActionEditUndo );
	AddUpdateAction( pActionEditRedo );
	AddUpdateAction( pActionEditCut );
	AddUpdateAction( pActionEditCopy );
	AddUpdateAction( pActionEditPaste );
	AddUpdateAction( pActionEntryAdd );
	AddUpdateAction( pActionEntryRemove );
	AddUpdateAction( pActionEntryNextMissing );
}

void lpeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBFile, pActionLangPackNew );
	helper.ToolBarButton( pTBFile, pActionLangPackOpen );
	helper.ToolBarButton( pTBFile, pActionLangPackSave );
	
	helper.ToolBarSeparator( pTBFile );
	helper.ToolBarButton( pTBFile, pActionLangPackOpenRef );
	
	helper.ToolBarSeparator( pTBFile );
	helper.ToolBarButton( pTBFile, pActionEntryAdd );
	helper.ToolBarButton( pTBFile, pActionEntryRemove );
	helper.ToolBarButton( pTBFile, pActionEntryNextMissing );
	
	AddSharedToolBar( pTBFile );
}

void lpeWindowMain::pCreateToolBarEdit(){
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

void lpeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascadeReference cascade;
	
	cascade.TakeOver( new igdeMenuCascade( env, "Language Pack", deInputEvent::ekcL ) );
	pCreateMenuLangPack( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Edit", deInputEvent::ekcE ) );
	pCreateMenuEdit( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Entry", deInputEvent::ekcN ) );
	pCreateMenuEntry( cascade );
	AddSharedMenu( cascade );
}

void lpeWindowMain::pCreateMenuLangPack( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionLangPackNew );
	helper.MenuCommand( menu, pActionLangPackOpen );
	helper.MenuRecentFiles( menu, GetRecentFiles() );
	helper.MenuCommand( menu, pActionLangPackSave );
	helper.MenuCommand( menu, pActionLangPackSaveAs );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionLangPackOpenRef );
}

void lpeWindowMain::pCreateMenuEdit( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEditUndo );
	helper.MenuCommand( menu, pActionEditRedo );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditCut );
	helper.MenuCommand( menu, pActionEditCopy );
	helper.MenuCommand( menu, pActionEditPaste );
}

void lpeWindowMain::pCreateMenuEntry( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEntryAdd );
	helper.MenuCommand( menu, pActionEntryRemove );
	helper.MenuCommand( menu, pActionEntryNextMissing );
}
