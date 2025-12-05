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

#include "feWindowMain.h"
#include "feWindowMainListener.h"
#include "fontimage/feViewFontImage.h"
#include "properties/feWindowProperties.h"
#include "../configuration/feConfiguration.h"
#include "../feIGDEModule.h"
#include "../font/feFont.h"
#include "../font/feGenerateFont.h"
#include "../loadsave/feLoadSaveSystem.h"
#include "../clipboard/feClipboard.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
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
#include <dragengine/resources/font/deFont.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class feWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

feWindowMain::feWindowMain( igdeEditorModule &module ) :
igdeEditorWindow( module )
{
	igdeEnvironment &env = GetEnvironment();
	
	pListener = NULL;
	
	pClipboard = NULL;
	pLoadSaveSystem = NULL;

	pViewFontImage = NULL;
	pWndProps = NULL;
	
	pFont = NULL;
	
	// init the default font
	env.GetApplicationFont( pGenFontConfig );
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = new feWindowMainListener( *this );
	pLoadSaveSystem = new feLoadSaveSystem( this );
	pConfiguration = new feConfiguration( *this );
	pClipboard = new feClipboard;
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted;
	splitted.TakeOver(new igdeContainerSplitted(env, igdeContainerSplitted::espLeft,
		igdeApplication::app().DisplayScaled(260)));
	AddChild( splitted );
	
	pWndProps = new feWindowProperties( *this );
	splitted->AddChild( pWndProps, igdeContainerSplitted::eaSide );
	
	pViewFontImage = new feViewFontImage( *this );
	splitted->AddChild( pViewFontImage, igdeContainerSplitted::eaCenter );
	
	CreateNewFont();
	ResetViews();
}

feWindowMain::~feWindowMain(){
	if( pConfiguration ){
		pConfiguration->SaveConfiguration();
	}
	if( pClipboard ){
		delete pClipboard;
	}
	
	SetFont( NULL );
	
	if( pViewFontImage ){
		pViewFontImage->FreeReference();
		pViewFontImage = NULL;
	}
	if( pWndProps ){
		pWndProps->FreeReference();
		pWndProps = NULL;
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

bool feWindowMain::QuitRequest(){
	// check if user wants to quit. returns true if a quit
	// is wished by the user
	
	// clean up
	return true;
}

void feWindowMain::ResetViews(){
	pViewFontImage->ResetView();
}



void feWindowMain::SetGenFontConfig( const igdeFont::sConfiguration &config ){
	pGenFontConfig = config;
}

void feWindowMain::SetFont( feFont *font ){
	if( font == pFont ){
		return;
	}
	
	pWndProps->SetFont( NULL );
	pViewFontImage->SetFont( NULL );
	pActionEditUndo->SetUndoSystem( NULL );
	pActionEditRedo->SetUndoSystem( NULL );
	
	if( pFont ){
		pFont->RemoveNotifier( pListener );
		
		pFont->Dispose();
		pFont->FreeReference();
	}
	
	pFont = font;
	
	if( font ){
		font->AddReference();
		font->AddNotifier( pListener );
		
		pActionEditUndo->SetUndoSystem( font->GetUndoSystem() );
		pActionEditRedo->SetUndoSystem( font->GetUndoSystem() );
	}
	
	pViewFontImage->SetFont( font );
	pWndProps->SetFont( font );
}

void feWindowMain::CreateNewFont(){
	feFont *font = NULL;
	
	try{
		font = new feFont( &GetEnvironment() );
		SetFont( font );
		font->FreeReference();
		
	}catch( const deException & ){
		if( font ){
			font->FreeReference();
		}
		throw;
	}
}

void feWindowMain::SaveFont( const char *filename ){
	pLoadSaveSystem->SaveFont( pFont, filename );
	
	pFont->SetChanged( false );
	pFont->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}



void feWindowMain::OnBeforeEngineStart(){
}

void feWindowMain::OnAfterEngineStart(){
	pViewFontImage->OnAfterEngineStart();
	pLoadSaveSystem->UpdateLSFonts();
}

void feWindowMain::OnBeforeEngineStop(){
	pViewFontImage->OnBeforeEngineStop();
}

void feWindowMain::OnAfterEngineStop(){
	pLoadSaveSystem->UpdateLSFonts();
}



void feWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pViewFontImage->SetEnableRendering( true );
}

void feWindowMain::OnDeactivate(){
	pViewFontImage->SetEnableRendering( false );
	igdeEditorWindow::OnDeactivate();
}



void feWindowMain::OnFrameUpdate( float elapsed ){
	if( ! GetActiveModule() ){
		return;
	}
	
	pViewFontImage->OnFrameUpdate( elapsed );
}



void feWindowMain::GetChangedDocuments( decStringList &list ){
	if( pFont && pFont->GetChanged() ){
		list.Add( pFont->GetFilePath() );
	}
}

void feWindowMain::LoadDocument( const char *filename ){
	feFont *font = pLoadSaveSystem->LoadFont( filename, GetGameDefinition() );
	
	SetFont( font );
	font->FreeReference();
	
	font->SetFilePath( filename );
	font->SetChanged( false );
	font->SetSaved( true );
	GetRecentFiles().AddFile( filename );
}

bool feWindowMain::SaveDocument( const char *filename ){
	if( pFont && pFont->GetFilePath().Equals( filename ) ){
		pActionFontSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void feWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void feWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewFont();
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	feWindowMain &pWindow;
	
public:
	cActionBase( feWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( feWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
};


class cActionFontNew : public cActionBase{
public:
	cActionFontNew( feWindowMain &window ) : cActionBase( window, "New",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ), "Creates a new font",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN ){}
	
	virtual void OnAction(){
		if( igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "New Font",
		"Creating a new font discarding the current one is that ok?" ) == igdeCommonDialogs::ebYes ){
			pWindow.CreateNewFont();
		}
	}
};


class cActionFontOpen : public cActionBase{
public:
	cActionFontOpen( feWindowMain &window ) : cActionBase( window, "Open...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Opens a font from file",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetFont()->GetFilePath() );
		if( ! igdeCommonDialogs::GetFileOpen( &pWindow, "Open Font",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltFont ), filename ) ){
			return;
		}
		
		// load font
		pWindow.GetEditorModule().LogInfoFormat( "Loading font %s", filename.GetString() );
		feFont *font = pWindow.GetLoadSaveSystem().LoadFont( filename,
			pWindow.GetGameProject()->GetGameDefinition() );
		
		// replace font
		pWindow.SetFont( font );
		font->FreeReference();
		
		// store information
		font->SetFilePath( filename );
		font->SetChanged( false );
		font->SetSaved( true );
		pWindow.GetRecentFiles().AddFile( filename );
	}
};


class cActionFontSaveAs : public cActionBase{
public:
	cActionFontSaveAs( feWindowMain &window ) : cActionBase( window, "Save As...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSaveAs ),
		"Saves the font under a differen file", deInputEvent::ekcA ){}
	
	virtual void OnAction(){
		decString filename( pWindow.GetFont()->GetFilePath() );
		if( igdeCommonDialogs::GetFileSave( &pWindow, "Save Font",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltFont ), filename ) ){
			pWindow.SaveFont( filename );
		}
	}
};


class cActionFontSave : public cActionFontSaveAs{
public:
	cActionFontSave( feWindowMain &window ) : cActionFontSaveAs( window ){
		SetText( "Save" );
		SetDescription( "Saves the font to file" );
		SetHotKey( igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcS ) );
		SetMnemonic( deInputEvent::ekcS );
		SetIcon( window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ) );
	}
	
	virtual void OnAction(){
		feFont &font = *pWindow.GetFont();
		
		if( font.GetSaved() ){
			if( font.GetChanged() ){
				pWindow.SaveFont( font.GetFilePath() );
			}
			
		}else{
			cActionFontSaveAs::OnAction();
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetFont()->GetChanged() );
	}
};


class cActionFontGenerate : public cActionBase{
public:
	cActionFontGenerate( feWindowMain &window ) : cActionBase( window, "Generate...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ),
		"Generates a new font from a system font", deInputEvent::ekcG ){}
	
	virtual void OnAction(){
		igdeFont::sConfiguration config( pWindow.GetGenFontConfig() );
		if( ! igdeCommonDialogs::SelectSystemFont( &pWindow, "Select Font", config ) ){
			return;
		}
		
		pWindow.SetGenFontConfig( config );
		
		int enlargeGlyph = 0;
		if( ! igdeCommonDialogs::GetInteger( &pWindow, "Glyph Enlarge", "Enlarge:", enlargeGlyph ) ){
			return;
		}
		
		feGenerateFont generateFont( pWindow.GetEnvironment() );
		generateFont.SetEnlargeGlpyh( enlargeGlyph );
		generateFont.SetCodeRange( 32, 255 );
		generateFont.SetFontConfig( config );
		
		deObjectReference font;
		font.TakeOver( generateFont.GenerateFont() );
		pWindow.SetFont( ( feFont* )( deObject* )font );
	}
};


class cActionEditCut : public cActionBase{
public:
	cActionEditCut( feWindowMain &window ) : cActionBase( window,
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
	cActionEditCopy( feWindowMain &window ) : cActionBase( window,
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
	cActionEditPaste( feWindowMain &window ) : cActionBase( window,
		"Paste", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP ){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetClipboard().HasClip() );
	}
};

};



// Private Functions
//////////////////////

void feWindowMain::pLoadIcons(){
	//pIconFontNew.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/file_new.png" ) );
}

void feWindowMain::pCreateActions(){
	pActionFontNew.TakeOver( new cActionFontNew( *this ) );
	pActionFontOpen.TakeOver( new cActionFontOpen( *this ) );
	pActionFontSave.TakeOver( new cActionFontSave( *this ) );
	pActionFontSaveAs.TakeOver( new cActionFontSaveAs( *this ) );
	pActionFontGenerate.TakeOver( new cActionFontGenerate( *this ) );
	pActionEditUndo.TakeOver( new igdeActionUndo( GetEnvironment() ) );
	pActionEditRedo.TakeOver( new igdeActionRedo( GetEnvironment() ) );
	pActionEditCut.TakeOver( new cActionEditCut( *this ) );
	pActionEditCopy.TakeOver( new cActionEditCopy( *this ) );
	pActionEditPaste.TakeOver( new cActionEditPaste( *this ) );
	
	
	// register for updating
	AddUpdateAction( pActionFontNew );
	AddUpdateAction( pActionFontOpen );
	AddUpdateAction( pActionFontSave );
	AddUpdateAction( pActionFontSaveAs );
	AddUpdateAction( pActionFontGenerate );
	AddUpdateAction( pActionEditUndo );
	AddUpdateAction( pActionEditRedo );
	AddUpdateAction( pActionEditCut );
	AddUpdateAction( pActionEditCopy );
	AddUpdateAction( pActionEditPaste );
}

void feWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBFile, pActionFontNew );
	helper.ToolBarButton( pTBFile, pActionFontOpen );
	helper.ToolBarButton( pTBFile, pActionFontSave );
	
	AddSharedToolBar( pTBFile );
}

void feWindowMain::pCreateToolBarEdit(){
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

void feWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade.TakeOver( new igdeMenuCascade( env, "Font", deInputEvent::ekcF ) );
	pCreateMenuFont( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Edit", deInputEvent::ekcE ) );
	pCreateMenuEdit( cascade );
	AddSharedMenu( cascade );
}

void feWindowMain::pCreateMenuFont( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionFontNew );
	helper.MenuCommand( menu, pActionFontOpen );
	helper.MenuRecentFiles( menu, GetRecentFiles() );
	helper.MenuCommand( menu, pActionFontSave );
	helper.MenuCommand( menu, pActionFontSaveAs );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionFontGenerate );
}

void feWindowMain::pCreateMenuEdit( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEditUndo );
	helper.MenuCommand( menu, pActionEditRedo );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditCut );
	helper.MenuCommand( menu, pActionEditCopy );
	helper.MenuCommand( menu, pActionEditPaste );
}
