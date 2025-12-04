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

#include "igdeWindowMain.h"
#include "igdeDialogNewGameProject.h"
#include "igdeDialogProjectSettings.h"
#include "igdeDialogStartUp.h"
#include "igdeTaskSyncGameDefinition.h"
#include "igdeSharedFontList.h"
#include "../engine/igdeScriptModule.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../loadsave/igdeLoadSaveSystem.h"
#include "../loadsave/igdeLoadTemplate.h"
#include "../module/igdeEditorModuleManager.h"
#include "../module/igdeEditorModuleDefinition.h"
#include "../template/igdeTemplate.h"
#include "../template/igdeTemplateList.h"

#include <deigde/deigde_configuration.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/engine/textureProperties/igdeTextureProperty.h>
#include <deigde/engine/textureProperties/igdeTexturePropertyList.h>
#include <deigde/engine/textureProperties/igdeXMLLoadTexturePropertyList.h>
#include <deigde/logger/igdeLoggerHistory.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeStepableTask.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTimer.h>
#include <deigde/gui/igdeToggleButton.h>
#include <deigde/gui/igdeToggleButtonReference.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeStatusBar.h>
#include <deigde/gui/igdeProgressBar.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/gui/dialog/igdeDialogTexturePropertyList.h>
#include <deigde/gui/enginestatus/igdeDialogEngine.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/layout/igdeContainerBorderReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/logger/igdeWindowLogger.h>
#include <deigde/gui/menu/igdeMenuBar.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuOption.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gui/theme/igdeGuiTheme.h>
#include <deigde/gui/theme/igdeGuiThemeReference.h>
#include <deigde/gui/theme/propertyNames.h>
#include <deigde/gui/theme/themeNames.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gamedefinition/igdeGDCategory.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/igdeGameDefinitionList.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/skin/igdeGDSkin.h>
#include <deigde/gamedefinition/skin/igdeGDSkinManager.h>
#include <deigde/gamedefinition/sky/igdeGDSky.h>
#include <deigde/gamedefinition/sky/igdeGDSkyManager.h>
#include <deigde/gamedefinition/preview/igdeGDPreviewManager.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gamedefinition/igdeXMLGameDefinition.h>
#include <deigde/gamedefinition/igdeXMLElementClass.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSRedirect.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerReference.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBuilder.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>


// Definitions
////////////////

#define LOGSOURCE "IGDE"



// Actions
////////////

class cActionBase : public igdeAction{
protected:
	igdeWindowMain &pWindow;
	
public:
	cActionBase( igdeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( igdeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
};


class cActionGameNew : public cActionBase{
public:
	cActionGameNew( igdeWindowMain &window ) : cActionBase( window,
		"New...", window.GetIconGameNew(), "Creates a new game", deInputEvent::ekcN ){}
	
	virtual void OnAction(){
		// RequestSaveDocuments returns true if user wants to continue creating a new project.
		// this can mean to save the documents first or to not save them first.
		if( pWindow.RequestSaveDocuments( "New Game Project", NULL ) ){
			pWindow.CreateNewGameProject();
		}
	}
};


class cActionGameOpen : public cActionBase{
public:
	cActionGameOpen( igdeWindowMain &window ) : cActionBase( window,
		"Open...", window.GetIconGameOpen(), "Opens a game", deInputEvent::ekcO ){}
	
	virtual void OnAction(){
		// RequestSaveDocuments returns true if user wants to continue creating open a project.
		// this can mean to save the documents first or to not save them first.
		if( ! pWindow.RequestSaveDocuments( "Open Game Project", NULL ) ){
			return;
		}
		
		decString filename( pWindow.GetGameProject() ? pWindow.GetGameProject()->GetFilePath() : decString() );
		if( igdeCommonDialogs::GetFileOpen( &pWindow, "Open Game Project",
		pWindow.GetLoadSaveSystem()->GetOpenFilePatternList( igdeLoadSaveSystem::efplGameProject ), filename ) ){
			pWindow.LoadGameProject( filename );
		}
	}
};


class cActionGameOpenRecent : public cActionBase{
private:
	const decString pFilename;
	
	static decString BuildText( const char *filename ){
		decPath path( decPath::CreatePathNative( filename ) );
		const decString filetitle( path.GetLastComponent() );
		
		path.RemoveLastComponent();
		const decString directory( path.GetPathNative() );
		
		decString text;
		text.Format( "%s [%s]", filetitle.GetString(), directory.GetString() );
		return text;
	}
	
public:
	cActionGameOpenRecent( igdeWindowMain &window, const char *filename ) :
	cActionBase( window, BuildText( filename ), NULL, "Open game project" ),
	pFilename( filename ){ }
	
	virtual void OnAction(){
		// RequestSaveDocuments returns true if user wants to continue creating open a project.
		// this can mean to save the documents first or to not save them first.
		if( pWindow.RequestSaveDocuments( "Open Game Project", NULL ) ){
			// WARNING loading a game does change the recent games menu. to avoid troubles
			//         the filename is safe-guarded locally to not cause trashed memory
			const decString filename( pFilename );
			pWindow.LoadGameProject( filename );
		}
	}
};


class cActionGameSaveAs : public cActionBase{
public:
	cActionGameSaveAs( igdeWindowMain &window ) : cActionBase( window,
		"Save As...", window.GetIconGameSaveAs(), "Saves game under a differen file",
		deInputEvent::ekcA ){}
	
	virtual void OnAction(){
		if( ! pWindow.GetGameProject() ){
			return;
		}
		
		decString filename( pWindow.GetGameProject()->GetFilePath() );
		if( igdeCommonDialogs::GetFileOpen( &pWindow, "Save Game Project",
		pWindow.GetLoadSaveSystem()->GetSaveFilePatternList( igdeLoadSaveSystem::efplGameProject ), filename ) ){
			pWindow.SaveGameProject( filename );
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetGameProject() != NULL );
	}
};


class cActionGameSave : public cActionGameSaveAs{
public:
	cActionGameSave( igdeWindowMain &window ) : cActionGameSaveAs( window ){
		SetText( "Save" );
		SetDescription( "Saves game" );
		//SetHotKey( igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcS ) );
		SetMnemonic( deInputEvent::ekcS );
		SetIcon( window.GetIconGameSave() );
	}
	
	virtual void OnAction(){
		igdeGameProject * const project = pWindow.GetGameProject();
		if( project->GetChanged() ){
			pWindow.SaveGameProject( project->GetFilePath() );
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetGameProject() && pWindow.GetGameProject()->GetChanged() );
	}
};


class cActionGameSettings : public cActionBase{
public:
	cActionGameSettings( igdeWindowMain &window ) : cActionBase( window,
		"Settings...", NULL, "Game project settings", deInputEvent::ekcT ){}
	
	virtual void OnAction(){
		igdeDialogReference dialog;
		dialog.TakeOver( new igdeDialogProjectSettings( pWindow ) );
		if( dialog->Run( &pWindow )
		&& ( ( igdeDialogProjectSettings& )( igdeDialog& )dialog ).GetBaseGameDefsChanged() ){
			pWindow.OnProjectGameDefinitionChanged();
		}
	}
	
	virtual void Update(){
		SetEnabled( pWindow.GetGameProject() != NULL );
	}
};


class cActionGameReloadXMLElementClasses : public cActionBase{
public:
	cActionGameReloadXMLElementClasses( igdeWindowMain &window ) : cActionBase( window,
		"Reload XML Element Classes", window.GetIconGameReloadXMLElementClasses(),
		"Reload XML Element Classes" ){}
	
	virtual void OnAction(){
		if( ! pWindow.IsSyncGameDefTaskRunning() ){
			pWindow.ReloadXMLElementClasses();
		}
	}
	
	virtual void Update(){
		SetEnabled( ! pWindow.IsSyncGameDefTaskRunning() );
	}
};


class cActionGameQuit : public cActionBase{
public:
	cActionGameQuit( igdeWindowMain &window ) : cActionBase( window,
		"Quit", window.GetIconGameExit(), "Quit IGDE", deInputEvent::ekcQ ){}
	
	virtual void OnAction(){
		if( pWindow.QuitRequest() ){
			pWindow.Close();
		}
	}
};


class cActionSettingsEngine : public cActionBase{
public:
	cActionSettingsEngine( igdeWindowMain &window ) : cActionBase( window,
		"Engine Control Center...", window.GetIconSettingsEngine(),
		"Shows the engine control system window", deInputEvent::ekcE ){}
	
	virtual void OnAction(){
		igdeDialogReference dialog;
		dialog.TakeOver( new igdeDialogEngine( pWindow ) );
		dialog->Run( &pWindow );
	}
};


class cActionSettingsIgde: public cActionBase{
public:
	cActionSettingsIgde( igdeWindowMain &window ) : cActionBase( window,
		"IGDE Settings...", window.GetIconSettingsTexPropList(),
		"Change IGDE Settings", deInputEvent::ekcI ){}
	
	virtual void OnAction(){
		// TODO
	}
};


class cActionSettingsTexPropList : public cActionBase{
public:
	cActionSettingsTexPropList( igdeWindowMain &window ) : cActionBase( window,
		"Texture Property List...", window.GetIconSettingsTexPropList(),
		"Shows the list of texture properties", deInputEvent::ekcT ){}
	
	virtual void OnAction(){
		igdeDialogReference dialog;
		dialog.TakeOver( new igdeDialogTexturePropertyList( pWindow.GetEnvironment() ) );
		dialog->Run( &pWindow );
	}
};


class cActionSettingsLogging : public cActionBase{
public:
	cActionSettingsLogging( igdeWindowMain &window ) : cActionBase( window,
		"Logging Window...", window.GetIconSettingsEngine(),
		"Displays the logging window", deInputEvent::ekcL ){}
	
	virtual void OnAction(){
		pWindow.ShowWindowLogger();
	}
};


class cActionWindowEditor : public cActionBase{
private:
	igdeEditorModuleDefinition &pModule;
	
	static decString BuildDescription( const igdeEditorModuleDefinition &module ){
		decString text;
		text.Format( "Switch to %s", module.GetName().GetString() );
		return text;
	}
	
public:
	cActionWindowEditor( igdeWindowMain &window, igdeEditorModuleDefinition &module, bool smallIcon = true ) :
	cActionBase( window, module.GetName(), NULL, BuildDescription( module ) ),
	pModule( module ){
		igdeIconReference icon;
		const decString &path = smallIcon || module.GetIconLarge().IsEmpty() ? module.GetIconSmall() : module.GetIconLarge();
		if( ! path.IsEmpty() ){
			try{
				icon.TakeOver( igdeIcon::LoadPNG( *module.GetModule(), path ) );
			}catch( const deException &e ){
				icon = NULL;
				window.GetEnvironment().GetLogger()->LogException( LOGSOURCE, e );
			}
		}
		if( ! icon ){
			icon = window.GetStockIcon( igdeEnvironment::esiWarning );
		}
		SetIcon( icon );
	}
	
	virtual void OnAction(){
		pWindow.GetModuleManager().SetActiveModule( &pModule );
	}
	
	virtual void Update(){
		SetSelected( &pModule == pWindow.GetModuleManager().GetActiveModule() );
	}
};


class cTimerFrameUpdate : public igdeTimer{
	igdeWindowMain &pWindow;
	
public:
	cTimerFrameUpdate( igdeWindowMain &window ) : igdeTimer( window.GetEnvironment() ), pWindow( window ){}
	
	virtual void OnTimeout(){
		pWindow.OnFrameUpdate();
	}
};

class cTimerSyncProject : public igdeTimer{
	igdeWindowMain &pWindow;
	
public:
	cTimerSyncProject( igdeWindowMain &window ) : igdeTimer( window.GetEnvironment() ), pWindow( window ){}
	
	virtual void OnTimeout(){
		pWindow.UpdateSyncProject();
	}
};



// Class igdeWindowMain
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeWindowMain::igdeWindowMain( igdeEnvironmentIGDE &environment ) :
igdeMainWindow( environment, "Drag[en]gine IGDE" ),

pEnvironmentIGDE( environment ),

pConfiguration( *this ),
pConfigurationLocal( *this ),
pModuleManager( NULL ),
pLoadSaveSystem( NULL ),
pGDPreviewManager( NULL ),
pTexturePropertyList( NULL ),
pTemplates( NULL ),
pSharedGameDefinitions( NULL ),
pIGDEGameDefinition( NULL ),
pGameProject( NULL ),
pSharedFontList( NULL ),
pResourceLoader( NULL ),
pUIHelper( NULL ),
pUIHelperProperties( NULL ),

pStatusBar( NULL ),
pSBProgress( NULL ),

pElapsedTime( 0.0f ),
pMinUpdateTime( 0.01f ), //0.0f;//0.00001f;
pTimer( NULL ),

pDisplayFPSInfo( false ),
pFPSAccum( 0 ),
pFPSFrames( 0 ),
pFPSRate( 1 ),

pPauseUpdating( false ),
pFirstEngineRun( true ),

pTaskSyncGameDefinition( NULL )
{
	pEnvironmentIGDE.SetWindowMain( this );
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(1600, 900)));
	
	try{
		pConfiguration.LocatePath();
		
		pVFS.TakeOver( new deVirtualFileSystem );
		pConfiguration.InitVirtualFileSystem();
		
		pInitLogger(); // log file depends on VFS to be ready
		
		pConfiguration.LogImportantValues();
		
		pUIHelper = new igdeUIHelper( pEnvironmentIGDE );
		pUIHelperProperties = new igdeUIHelper( *pUIHelper );
		
		pSharedFontList = new igdeSharedFontList( pEnvironmentIGDE );
		pTexturePropertyList = new igdeTexturePropertyList;
		pTimer = new decTimer;
		
		pResourceLoader = new igdeResourceLoader( pEnvironmentIGDE );
		
		CreateEngineController(); // uses methods provided by base class
		GetEngineController().UpdateEngine( nullptr, pConfiguration.GetPathIGDEData(),
			pConfiguration.GetPathIGDEEditorData() );
		
		pCreateSharedModelCollisionRig();
		pLoadStockSkins();
		pLoadStockModels();
		pLoadStockRigs();
		
		pAddIGDEEngineModules();
		GetEngineController().ActivateModule( igdeEngineController::esScripting, "IGDEScript" );
		
		pLoadSaveSystem = new igdeLoadSaveSystem( this );
		pLoadSaveSystem->UpdatePatternLists();
		
		pGDPreviewManager = new igdeGDPreviewManager( pEnvironmentIGDE );
		pGDPreviewManager->SetDirectoryObjectClass( "objectclass" );
		pGDPreviewManager->SetDirectorySkin( "skin" );
		pGDPreviewManager->SetDirectorySky( "sky" );
		
		pLoadTexturePropertyList();
		
		pConfiguration.LoadConfiguration();
		/*
		if( pConfiguration.GetGameDefinitionManager()->GetDefinitionCount() == 0 ){
			igdeGameDefinition *gamedef = CreateNewGameDefinition();
			pConfiguration.GetGameDefinitionManager()->AddDefinition( gamedef );
			gamedef = NULL;
			
// 			igdeCommonDialogs::Error( this, "Error", "No Game Definitions found!" );
// 			DETHROW( deeInvalidParam );
		}
		*/
// 		GetEngineController().UpdateEngine( pConfiguration.GetGameDefinitionManager()->GetDefinitionAt( 0 ) );
		
		// load the igde game definition
		
		pLoadIGDEGameDefinition();
		
		pSharedGameDefinitions = new igdeGameDefinitionList;
		pLoadSharedGameDefinitions();
		
		pTemplates = new igdeTemplateList;
		pLoadTemplates();
		
		pIconApplication.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/application.png" ) );
		SetIcon( pIconApplication );
		
		pIconGameNew.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/game_new.png" ) );
		pIconGameOpen.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/game_open.png" ) );
		pIconGameSave.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/game_save.png" ) );
		pIconGameSaveAs.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/game_saveas.png" ) );
		pIconGameExit.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/game_exit.png" ) );
		pIconGameReloadXMLElementClasses.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/settings_engine.png" ) );
		pIconSettingsIGDE.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/settings_igde.png" ) );
		pIconSettingsEngine.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/settings_engine.png" ) );
		pIconSettingsTexPropList.TakeOver( igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/settings_engine.png" ) );
		
		pLoadStockIcons();
		pCreateGuiThemes();
		
		pModuleManager = new igdeEditorModuleManager( *this );
		
		pCreateActions();
		pCreateMenu();
		
		
		// create window content
		igdeContainerBorderReference content;
		content.TakeOver( new igdeContainerBorder( pEnvironmentIGDE ) );
		AddChild( content );
		
		
		// top side
		igdeContainerReference panel;
		panel.TakeOver( new igdeContainerFlow( pEnvironmentIGDE, igdeContainerFlow::eaY ) );
		content->AddChild( panel, igdeContainerBorder::eaTop );
		
		pMenuBar.TakeOver( new igdeMenuBar( pEnvironmentIGDE ) );
		panel->AddChild( pMenuBar );
		
		pToolBarDockTop.TakeOver( new igdeToolBarDock( pEnvironmentIGDE, igdeToolBarDock::esTop ) );
		panel->AddChild( pToolBarDockTop );
		
		
		// bottom side
		panel.TakeOver( new igdeContainerFlow( pEnvironmentIGDE, igdeContainerFlow::eaY ) );
		content->AddChild( panel, igdeContainerBorder::eaBottom );
		
		pToolBarDockBottom.TakeOver( new igdeToolBarDock( pEnvironmentIGDE, igdeToolBarDock::esBottom ) );
		panel->AddChild( pToolBarDockBottom );
		
		pStatusBar.TakeOver( new igdeStatusBar( environment ) );
		pStatusBar->SetText( "Ready" );
		panel->AddChild( pStatusBar );
		
		pUIHelper->ProgressBar( pStatusBar, "", 0, 100, true, pSBProgress );
		pSBProgress->SetVisible( false );
		
		
		// left side
		panel.TakeOver( new igdeContainerFlow( pEnvironmentIGDE, igdeContainerFlow::eaX ) );
		content->AddChild( panel, igdeContainerBorder::eaLeft );
		
		pToolBarDockLeft.TakeOver( new igdeToolBarDock( pEnvironmentIGDE, igdeToolBarDock::esLeft ) );
		panel->AddChild( pToolBarDockLeft );
		
		
		// right side
		panel.TakeOver( new igdeContainerFlow( pEnvironmentIGDE, igdeContainerFlow::eaX ) );
		content->AddChild( panel, igdeContainerBorder::eaRight );
		
		pFraEditors.TakeOver( new igdeContainerFlow( pEnvironmentIGDE, igdeContainerFlow::eaY ) );
		panel->AddChild( pFraEditors );
		
		pToolBarDockRight.TakeOver( new igdeToolBarDock( pEnvironmentIGDE, igdeToolBarDock::esRight ) );
		panel->AddChild( pToolBarDockRight );
		
		
		// center
		pSwiContent.TakeOver( new igdeSwitcher( pEnvironmentIGDE ) );
		
		igdeContainerBorderReference emptyContainer;
		emptyContainer.TakeOver( new igdeContainerBorder( pEnvironmentIGDE ) );
		pSwiContent->AddChild( emptyContainer );
		
		content->AddChild( pSwiContent, igdeContainerBorder::eaCenter );
		
		
		// fill it
		pCreateToolBarGame();
// 		pTBEditors.TakeOver( new igdeToolBar( pEnvironmentIGDE ) );
		
		CreatePlaceholderGameProject();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeWindowMain::~igdeWindowMain(){
}



// Management
///////////////

igdeGuiTheme *igdeWindowMain::GetGuiThemeNamed( const char *name ){
	return ( igdeGuiTheme* )pGuiThemes.GetAt( name );
}

void igdeWindowMain::ShowWindowLogger(){
	if( ! pWindowLogger ){
		pWindowLogger.TakeOver( new igdeWindowLogger( pEnvironmentIGDE ) );
		pWindowLogger->SetLogger( GetLoggerHistory() );
	}
	
	pWindowLogger->SetVisible( true );
	pWindowLogger->RaiseAndActivate();
}



bool igdeWindowMain::ProcessCommandLine( const decUnicodeStringList &arguments ){
	decUnicodeStringList args( arguments );
	decString loadFile;
	
	while( args.GetCount() > 0 ){
		const decString arg( args.GetAt( 0 ).ToUTF8() );
		args.RemoveFrom( 0 );
		
		if( arg == "--showFPS" ){
			pDisplayFPSInfo = true;
			
		}else if( arg.BeginsWith( "--" ) || arg.BeginsWith( "-" ) ){
			decString message;
			message.Format( "Unknown argument '%s'", arg.GetString() );
			DETHROW_INFO( deeInvalidParam, message );
			
		}else if( loadFile.IsEmpty() ){
			loadFile = arg;
			break;
		}
	}
	
	if( loadFile.IsEmpty() ){
		#ifdef IGDE_NULL_TOOLKIT
		printf( "deigde <project-file.degp> {--help | ...}\n" );
		DETHROW_INFO( deeInvalidParam, "Missing arguments" );
		#endif
		
		igdeDialogReference dialog;
		dialog.TakeOver( new igdeDialogStartUp( *this ) );
		return dialog->Run( this );
	}
	
	GetLogger()->LogInfoFormat( LOGSOURCE, "Loading game project %s", loadFile.GetString() );
	
	if( ! LoadGameProject( loadFile ) ){
		return false;
	}
	
	pAfterLoadArguments = args;
	
	#ifdef IGDE_NULL_TOOLKIT
	if( pAfterLoadArguments.GetCount() == 0 ){
		printf( "deigde <project-file.degp> {--help | ...}\n" );
		DETHROW_INFO( deeInvalidParam, "Missing arguments" );
	}
	#endif
	return true;
}



void igdeWindowMain::SetGameProject( igdeGameProject *project ){
	if( project == pGameProject ){
		return;
	}
	
	igdeGameProject * const oldProject = pGameProject;
	
	pGameProject = project;
	
	if( project ){
		project->AddReference();
	}
	
	GetEngineController().UpdateEngine( project, pConfiguration.GetPathIGDEData(),
		pConfiguration.GetPathIGDEEditorData() );
	
	if( project ){
		project->GetGameDefinition()->UpdateEngineObjects();
	}
	
	const int moduleCount = pModuleManager->GetModuleCount();
	int i;
	
	for( i=0; i<moduleCount; i++ ){
		const igdeEditorModuleDefinition &moduleDefinition = *pModuleManager->GetModuleAt( i );
		
		if( moduleDefinition.IsModuleRunning() ){
			moduleDefinition.GetModule()->OnGameProjectChanged();
		}
	}
	
	if( oldProject ){
		oldProject->FreeReference();
	}
	
	// update window title
	const decPath path( decPath::CreatePathNative( project->GetFilePath() ) );
	decString title;
	
	if( path.GetComponentCount() == 0 ){
		title = "Drag[en]gine IGDE";
		
	}else{
		title.Format( "Drag[en]gine IGDE - %s", path.GetLastComponent().GetString() );
	}
	
	SetTitle( title );
	
	// load configuration
	pConfigurationLocal.LoadConfiguration();
	pModuleManager->SetActiveModule( pModuleManager->GetRecentModuleAt( 0 ) );
}

bool igdeWindowMain::CreateNewGameProject(){
	igdeDialogReference dialog;
	dialog.TakeOver( new igdeDialogNewGameProject( *this ) );
	if( dialog->Run( this ) ){
		SetGameProject( ( ( igdeDialogNewGameProject& )( igdeDialog& )dialog ).GetNewProject() );
		pModuleManager->ActivateProjectManager();
		return true;
	}
	return false;
}

void igdeWindowMain::CreatePlaceholderGameProject(){
	deObjectReference refProject;
	refProject.TakeOver( new igdeGameProject( pEnvironmentIGDE ) );
	igdeGameProject * const project = ( igdeGameProject* )( deObject* )refProject;
	
	deObjectReference refGameDef;
	refGameDef.TakeOver( CreateNewGameDefinition() );
	project->SetProjectGameDefinition( ( igdeGameDefinition* )( deObject* )refGameDef );
	
	project->MergeGameDefinitions();
	SetGameProject( project );
	
	pModuleManager->ActivateProjectManager();
}

bool igdeWindowMain::LoadGameProject( const char *filename ){
	GetLogger()->LogInfoFormat( LOGSOURCE, "Open game project %s", filename );
	
	try{
		deObjectReference refProject;
		refProject.TakeOver( pLoadSaveSystem->LoadGameProject( filename ) );
		igdeGameProject * const project = ( igdeGameProject* )( deObject* )refProject;
		
		pLoadXMLElementClasses( *project );
		pFindAndAddSkins( *project );
		pFindAndAddSkies( *project );
		project->MergeGameDefinitions();
		
		SetGameProject( project );
		
		AddRecentGameProject( filename );
		return true;
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
		igdeCommonDialogs::ErrorFormat( this, "Open Game Project",
			"Failed loading game project: %s", e.GetDescription().GetString() );
		
		const int index = pConfiguration.GetRecentProjectList().IndexOf( filename );
		if( index != -1 ){
			if( igdeCommonDialogs::Question( this, igdeCommonDialogs::ebsYesNo, "Open Game Project",
			"Remove game project from recent file list?" ) == igdeCommonDialogs::ebYes ){
				pConfiguration.GetRecentProjectList().RemoveFrom( index );
				pConfiguration.SaveConfiguration();
				UpdateRecentProjectMenu();
			}
		}
		
		return false;
	}
}

void igdeWindowMain::SaveGameProject( const char *filename ){
	GetLogger()->LogInfoFormat( LOGSOURCE, "Saving game project %s", filename );
	pLoadSaveSystem->SaveGameProject( pGameProject, filename );
	
	pGameProject->SetFilePath( filename );
	pGameProject->SetChanged( false );
	
	AddRecentGameProject( filename );
}

void igdeWindowMain::AddRecentGameProject( const char *filename ){
	// add to recent project list
	decStringList &recentProjectList = pConfiguration.GetRecentProjectList();
	const int recentProjectIndex = recentProjectList.IndexOf( filename );
	
	if( recentProjectIndex == -1 ){
		recentProjectList.InsertAt( filename, 0 );
		
		while( recentProjectList.GetCount() > pConfiguration.GetMaxRecentProjectEntries() ){
			recentProjectList.RemoveFrom( recentProjectList.GetCount() - 1 );
		}
		
	}else{
		recentProjectList.Move( recentProjectIndex, 0 );
	}
	pConfiguration.SaveConfiguration();
	
	// update menu
	UpdateRecentProjectMenu();
}



igdeGameDefinition *igdeWindowMain::CreateNewGameDefinition(){
	igdeGameDefinition *gamedef = NULL;
	decPath path;
	
	path.SetFromNative( pConfiguration.GetPathShares() );
	path.AddComponent( "newproject.degd" );
	
	decBaseFileReaderReference reader;
	
	try{
		reader.TakeOver( new decDiskFileReader( path.GetPathNative() ) );
		
		gamedef = new igdeGameDefinition( pEnvironmentIGDE );
		gamedef->SetFilename( path.GetPathNative() );
		
		path.SetFromNative( pConfiguration.GetPathProjects() );
		path.AddComponent( "newproject" );
		gamedef->SetBasePath( path.GetPathNative() );
		
		igdeXMLGameDefinition( pEnvironmentIGDE, GetLogger() ).Load( reader, *gamedef );
		
	}catch( const deException & ){
		if( gamedef ){
			gamedef->FreeReference();
		}
		throw;
	}
	
	return gamedef;
}



void igdeWindowMain::DisplayException( const deException &exception ){
	GetLogger()->LogException( LOGSOURCE, exception );
	igdeCommonDialogs::Exception( this, exception );
}

void igdeWindowMain::ActiveModuleSharedMenusChanged(){
	RebuildMenu();
}

void igdeWindowMain::ActiveModuleSharedToolBarsChanged(){
	RebuildToolBars();
}

void igdeWindowMain::ActivateEditor( igdeEditorModule *editor ){
	DEASSERT_NOTNULL( editor )
	
	const int count = pModuleManager->GetModuleCount();
	int i;
	for( i=0; i<count; i++ ){
		if( pModuleManager->GetModuleAt( i )->GetModule() == editor ){
			pModuleManager->SetActiveModule( pModuleManager->GetModuleAt( i ) );
			break;
		}
	}
}



void igdeWindowMain::RebuildMenu(){
	// clear the menu bar from all entries
	pMenuBar->RemoveAllChildren();
	
	// add menus
	pMenuBar->AddChild( pMenuGame );
	
	igdeEditorWindow * editorWindow = NULL;
	igdeEditorModuleDefinition * const editorModuleDef = pModuleManager->GetActiveModule();
	if( editorModuleDef ){
		igdeEditorModule * const editorModule = editorModuleDef->GetModule();
		if( editorModule ){
			editorWindow = editorModule->GetEditorWindow();
		}
	}
	
	if( editorWindow ){
		const int count = editorWindow->GetSharedMenuCount();
		int i;
		for( i=0; i<count; i++ ){
			pMenuBar->AddChild( editorWindow->GetSharedMenuAt( i ) );
		}
	}
	
	pMenuBar->AddChild( pMenuSettings );
	pMenuBar->AddChild( pMenuWindow );
	
	// update
	RebuildWindowMenu();
	UpdateRecentProjectMenu();
}

void igdeWindowMain::RebuildToolBars(){
	// remove all toolbars
	pToolBarDockTop->RemoveAllChildren();
	pToolBarDockLeft->RemoveAllChildren();
	pToolBarDockRight->RemoveAllChildren();
	pToolBarDockBottom->RemoveAllChildren();
	
	// add our toolbars
	pToolBarDockTop->AddChild( pTBGame );
// 	pToolBarDockRight->AddChild( pTBEditors );
	
	// add active editor module toolbars
	igdeEditorWindow * editorWindow = NULL;
	igdeEditorModuleDefinition * const editorModuleDef = pModuleManager->GetActiveModule();
	if( editorModuleDef ){
		igdeEditorModule * const editorModule = editorModuleDef->GetModule();
		if( editorModule ){
			editorWindow = editorModule->GetEditorWindow();
		}
	}
	
	if( editorWindow ){
		const int count = editorWindow->GetSharedToolBarCount();
		int i;
		for( i=0; i<count; i++ ){
			igdeToolBar * const toolbar = editorWindow->GetSharedToolBarAt( i );
			// TODO add it to the desired dock
			pToolBarDockTop->AddChild( toolbar );
		}
	}
}

void igdeWindowMain::SwitchToModuleWindow(){
	if( ! pModuleManager->GetActiveModule()
	|| ! pModuleManager->GetActiveModule()->GetModule() 
	|| ! pModuleManager->GetActiveModule()->GetModule()->GetEditorWindow() ){
		pSwiContent->SetCurrent( 0 ); // empty container
		return;
	}
	
	const int index = pSwiContent->IndexOfChild( pModuleManager->GetActiveModule()->GetModule()->GetEditorWindow() );
	if( index != -1 ){
		pSwiContent->SetCurrent( index );
	}
}

void igdeWindowMain::RebuildWindowMenu(){
	if( pMenuWindow ){
		pUpdateMenuWindow( pMenuWindow );
	}
}

void igdeWindowMain::UpdateRecentProjectMenu(){
	if( pMenuRecentProjects ){
		pUpdateMenuRecentProjects( pMenuRecentProjects );
	}
}

bool igdeWindowMain::IsSyncGameDefTaskRunning() const{
	return pTaskSyncGameDefinition != NULL;
}



void igdeWindowMain::ReloadXMLElementClasses(){
	if( ! pGameProject ){
		return;
	}
	
	try{
		pLoadXMLElementClasses( *pGameProject );
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
		DisplayException( e );
	}
	
	if( pTaskSyncGameDefinition ){
		delete pTaskSyncGameDefinition;
		pTaskSyncGameDefinition = NULL;
	}
	
	pTaskSyncGameDefinition = new igdeTaskSyncGameDefinition( *this );
	pTaskSyncGameDefinition->SetReloadXMLElementClasses( true );
	
	if( ! pTimerSyncProject ){
		pTimerSyncProject.TakeOver( new cTimerSyncProject( *this ) );
		pTimerSyncProject->Start( 10, true );
	}
	
	pActionGameReloadXMLElementClasses->Update();
}

void igdeWindowMain::ReFindAndAddSkins(){
	if( ! pGameProject ){
		return;
	}
	
	try{
		pFindAndAddSkins( *pGameProject );
		pGameProject->MergeGameDefinitions();
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
		DisplayException( e );
	}
}

void igdeWindowMain::ReFindAndAddSkies(){
	if( ! pGameProject ){
		return;
	}
	
	try{
		pFindAndAddSkies( *pGameProject );
		pGameProject->MergeGameDefinitions();
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
		DisplayException( e );
	}
}



void igdeWindowMain::SetProgressVisible( bool visible ){
	pSBProgress->SetVisible( visible );
}

void igdeWindowMain::SetProgress( float progress ){
	pSBProgress->SetValue( ( int )( progress * 100.0f + 0.5f ) );
}

void igdeWindowMain::SetProgressText( const char *text ){
	pStatusBar->SetText( text );
}



void igdeWindowMain::SetMinUpdateTime( float seconds ){
	pMinUpdateTime = decMath::max( seconds, 0.0f );
}



void igdeWindowMain::OnBeforeEngineStart(){
	const int moduleCount = pModuleManager->GetModuleCount();
	int i;
	
	for( i=0; i<moduleCount; i++ ){
		igdeEditorModuleDefinition &module = *pModuleManager->GetModuleAt( i );
		if( module.IsModuleRunning() ){
			module.GetModule()->OnBeforeEngineStart();
		}
	}
}

void igdeWindowMain::OnAfterEngineStart(){
	pLoadSaveSystem->UpdatePatternLists();
	pGDPreviewManager->OnAfterEngineStart();
	
	const int moduleCount = pModuleManager->GetModuleCount();
	int i;
	
	for( i=0; i<moduleCount; i++ ){
		igdeEditorModuleDefinition &module = *pModuleManager->GetModuleAt( i );
		if( module.IsModuleRunning() ){
			module.GetModule()->OnAfterEngineStart();
		}
	}
	
	// first time init. we have to move this here since this is the only safe time to
	// start anything potentially requiring the game engine object to be present.
	if( ! pFirstEngineRun ){
		return;
	}
	pFirstEngineRun = false;
	
	// start modules now the engine is running and we are constructed
	pModuleManager->StartModules();
	
	if( ! pModuleManager->GetActiveModule() ){
		RebuildMenu();
	}
	
	pRebuildToolBarEditors();
	RebuildWindowMenu();
	RebuildToolBars();
	
	// reset timer
	pTimer->Reset();
}

void igdeWindowMain::OnBeforeEngineStop(){
	pGDPreviewManager->OnBeforeEngineStop();
	
	const int moduleCount = pModuleManager->GetModuleCount();
	int i;
	
	for( i=0; i<moduleCount; i++ ){
		igdeEditorModuleDefinition &module = *pModuleManager->GetModuleAt( i );
		if( module.IsModuleRunning() ){
			module.GetModule()->OnBeforeEngineStop();
		}
	}
}

void igdeWindowMain::OnAfterEngineStop(){
	const int moduleCount = pModuleManager->GetModuleCount();
	int i;
	
	for( i=0; i<moduleCount; i++ ){
		igdeEditorModuleDefinition &module = *pModuleManager->GetModuleAt( i );
		if( module.IsModuleRunning() ){
			module.GetModule()->OnAfterEngineStop();
		}
	}
}



bool igdeWindowMain::QuitRequest(){
	// RequestSaveDocuments returns true if user wants to continue closing the application.
	// this can mean to save the documents first or to not save them first.
	return RequestSaveDocuments( "Quit", NULL );
}



void igdeWindowMain::OnFrameUpdate(){
	if( pPauseUpdating ){
		// stop the busy event loop until an event arrives. we just want to stop consuming
		// CPU time if the window is minimized or otherwise put out of sight in a way the
		// user obviously does not want to work with it right now.
// 		getApp()->runOneEvent();
		return;
	}
	if( ! pTimer ){
		return;  // cleaning up can still call timer
	}
	
	pElapsedTime += pTimer->GetElapsedTime();
	
	// to avoid spamming FOX the update speed is limited. this limit can be
	// modified using an option. Useful to be increased only for speed testing.
	bool doUpdate = false;
	
	if( GetEngineController().GetRunning() ){
		if( pElapsedTime >= pMinUpdateTime ){
			doUpdate = true;
		}
		
	}else{
		doUpdate = false; // no updates if the engine is stopped
		pElapsedTime = 0.0f;
		pFPSAccum = 0.0f;
		pFPSFrames = 1;
		pFPSRate = 0;
	}
	
	if( ! doUpdate ){
		return;
	}
	
	// clamp update time to a maximum of 0.2 seconds. this avoids jumps in case the event processing
	// gets a bit chocked up in FOX for a short time. this equals to a lower limit of 5Hz. if
	// somebody really ends up with 5Hz or less permanently he should adjust the module parameters
	// anyways to get a reasonable working environment
	pElapsedTime = decMath::min( pElapsedTime, 0.2f );
	
	// update fps tracker
	//printf( "update elapsed %i\n", (int)(pElapsedTime*1e6f) );
	pFPSAccum += pElapsedTime;
	pFPSFrames++;
	if( pFPSAccum >= 1.0f ){
		pFPSRate = pFPSFrames;
		pFPSFrames = 0;
		pFPSAccum -= ( int )pFPSAccum;
		
		if( pDisplayFPSInfo ){
			//pLogger->LogInfoFormat( LOGGING_NAME, "Average FPS over the last second: %i frames, %ims frame time.", pFPSRate, 1000 / pFPSRate );
			printf( "Average FPS: %i frames, %ims frame time.\n", pFPSRate, 1000 / pFPSRate );
		}
	}
	
	// update engine
	deEngine &engine = *GetEngine();
	
	// input is not processed as this would interfere with FOX event handling
	
	// set the elapsed time. we use our own elapsed time to make sure engine
	// and igde module processing is synchronous
	engine.SetElapsedTime( pElapsedTime );
	pElapsedTime = 0.0f;
	
	// run a single frame update. through our own igde script module we get UpdateFrame
	// called which does the actual frame update jobs
	try{
		engine.RunSingleFrame();
		
	}catch( const deException &exception ){
		DisplayException( exception );
		engine.SignalSystemFailed();
	}
	
	// check for errors. if something went wrong shut down the engine
	if( engine.GetScriptFailed() || engine.GetSystemFailed() ){
		engine.GetErrorTrace()->AddPoint( NULL, "igdeWindowMain::OnFrameUpdate", __LINE__ );
		StopEngine();
	}
	
	// process after start command line if present
	if( pModuleManager->GetModuleCount() > 0 && pAfterLoadArguments.GetCount() > 0 ){
		try{
			const int moduleCount = pModuleManager->GetModuleCount();
			int i;
			
			while( pAfterLoadArguments.GetCount() > 0 ){
				const int argCount = pAfterLoadArguments.GetCount();
				
				for( i=0; i<moduleCount; i++ ){
					igdeEditorModule * const module = pModuleManager->GetModuleAt( i )->GetModule();
					if( module && ! module->ProcessCommandLine( pAfterLoadArguments ) ){
						pAfterLoadArguments.RemoveAll();
						pEnvironmentIGDE.CloseApplication();
						return;
					}
				}
				
				if( pAfterLoadArguments.GetCount() == argCount ){
					if( pAfterLoadArguments.GetAt( 0 ).ToUTF8() == "--help" ){
						pAfterLoadArguments.RemoveAll();
						pEnvironmentIGDE.CloseApplication();
						return;
					}
					
					decString message;
					message.Format( "Unknown argument '%s'", pAfterLoadArguments.GetAt( 0 ).ToUTF8().GetString() );
					DETHROW_INFO( deeInvalidParam, message );
				}
			}
			
		}catch( const deException &e ){
			pAfterLoadArguments.RemoveAll();
			#ifdef IGDE_NULL_TOOLKIT
			throw e;
			#else
			DisplayException( e );
			pEnvironmentIGDE.CloseApplication();
			return;
			#endif
		}
		
		#ifdef IGDE_NULL_TOOLKIT
		printf( "deigde <project-file.degp> {--help | ...}\n" );
		DETHROW_INFO( deeInvalidParam, "Missing Arguments" );
		#endif
	}
}

void igdeWindowMain::OnProjectGameDefinitionChanged(){
	if( pTaskSyncGameDefinition ){
		delete pTaskSyncGameDefinition;
		pTaskSyncGameDefinition = NULL;
	}
	
	pTaskSyncGameDefinition = new igdeTaskSyncGameDefinition( *this );
	
	if( ! pTimerSyncProject ){
		pTimerSyncProject.TakeOver( new cTimerSyncProject( *this ) );
		pTimerSyncProject->Start( 10, true );
	}
	
	pActionGameReloadXMLElementClasses->Update();
}

void igdeWindowMain::ArmUpdateTimer(){
	if( ! pTimerFrameUpdate ){
		pTimerFrameUpdate.TakeOver( new cTimerFrameUpdate( *this ) );
		pTimerFrameUpdate->Start( 1, true ); // 5
	}
}

void igdeWindowMain::UpdateFrame(){
	const float elapsed = GetEngine()->GetElapsedTime();
	
	pResourceLoader->Update();
	pGDPreviewManager->Update();
	
	// update modules
	const int moduleCount = pModuleManager->GetModuleCount();
	int i;
	
	for( i=0; i<moduleCount; i++ ){
		igdeEditorModuleDefinition &moduleDef = *pModuleManager->GetModuleAt( i );
		if( ! moduleDef.IsModuleRunning() ){
			continue;
		}
		
		igdeEditorModule * const module = moduleDef.GetModule();
		if( ! module ){
			continue;
		}
		
		module->OnFrameUpdate( elapsed );
	}
	
	// update logging window
	if( pWindowLogger ){
		pWindowLogger->OnFrameUpdate();
	}
}

void igdeWindowMain::UpdateSyncProject(){
	if( ! pTaskSyncGameDefinition ){
		if( pTimerSyncProject ){
			pTimerSyncProject->Stop();
			pTimerSyncProject = NULL;
		}
		return;
	}
	
	try{
		if( pTaskSyncGameDefinition->Step() ){
			return;
		}
		
		if( pTimerSyncProject ){
			pTimerSyncProject->Stop();
			pTimerSyncProject = NULL;
		}
		
		delete pTaskSyncGameDefinition;
		pTaskSyncGameDefinition = NULL;
		pActionGameReloadXMLElementClasses->Update();
		
	}catch( const deException &e ){
		if( pTimerSyncProject ){
			pTimerSyncProject->Stop();
			pTimerSyncProject = NULL;
		}
		if( pTaskSyncGameDefinition ){
			delete pTaskSyncGameDefinition;
			pTaskSyncGameDefinition = NULL;
		}
		
		DisplayException( e );
		pActionGameReloadXMLElementClasses->Update();
	}
}



bool igdeWindowMain::RequestSaveDocuments( const char *title, const char *message ){
	const int moduleCount = pModuleManager->GetModuleCount();
	decStringList changedDocuments;
	int i;
	
	// determine the list of changed documents across all modules
	for( i=0; i<moduleCount; i++ ){
		igdeEditorModuleDefinition &moduleDefinition = *pModuleManager->GetModuleAt( i );
		
		if( moduleDefinition.IsModuleRunning() ){
			decStringList mcdList;
			moduleDefinition.GetModule()->GetChangedDocuments( mcdList );
			
			const int mcdCount = mcdList.GetCount();
			int j;
			
			for( j=0; j<mcdCount; j++ ){
				changedDocuments.Add( moduleDefinition.GetID() + ": " + mcdList.GetAt( j ) );
			}
		}
	}
	
	// ask the user which documents to save:
	// 
	// TODO create an own dialog for this containing an icon list. for each entry show the
	//      filename with the editor icon left to it. store the save information as a struct
	//      with module pointer and filename assigned as data pointer. this avoids this
	//      stupid split-string hack used below which can only break sooner or later
	
	if( changedDocuments.GetCount() > 0 ){
		decStringList choices;
		for( i=0; i<changedDocuments.GetCount(); i++ ){
			choices += changedDocuments.GetAt( i );
		}
		
		/*
		igdeCommonDialogs needs a ChoiceString in addition to SelectString allowing to select
		more then one string from a list of strings returning decIntList as result.
		
		const int selection = FXChoiceBox::ask( this, MBOX_OK_CANCEL,
			title, "Changed Documents", NULL, choices );
		
		if( selection == -1 ){
			return false;
		}
		*/
		
		decString text;
		
		if( message ){
			text = message;
			
		}else{
			text = "Unsaved changes in game project";
		}
		text += "\n\n";
		
		text += choices.Join( ", " );
		
		//MBOX_QUIT_SAVE_CANCEL
		switch( igdeCommonDialogs::Question( this, igdeCommonDialogs::ebsYesNoCancel, title, text ) ){
		case igdeCommonDialogs::ebNo:
			return true; // do not save and continue
			
		case igdeCommonDialogs::ebCancel:
			return false; // do not save but also do not continue
			
		case igdeCommonDialogs::ebYes:
			break; // do save and continue
			
		default: // should never happen
			return false;
		}
	}
	
	// save documents
	const int saveCount = changedDocuments.GetCount();
	
	for( i=0; i<saveCount; i++ ){
		const decString &document = changedDocuments.GetAt( i );
		const decStringList parts( document.Split( ':' ) );
		
		if( parts.GetCount() != 2 ){
			igdeCommonDialogs::Error( this, title, "Internal error" );
			return false;
		}
		
		const decString moduleID( parts.GetAt( 0 ).GetTrimmed() );
		const decString docfile( parts.GetAt( 1 ).GetTrimmed() );
		igdeEditorModuleDefinition * const moduleDefinition = pModuleManager->GetModuleWithID( moduleID );
		
		if( ! moduleDefinition || ! moduleDefinition->IsModuleRunning()
		|| ! moduleDefinition->GetModule()->SaveDocument( docfile ) ){
			igdeCommonDialogs::ErrorFormat( this, title, "Failed to save '%s'", docfile.GetString() );
			return false;
		}
	}
	
	// finished
	return true;
}

igdeIcon *igdeWindowMain::GetStockIcon( igdeEnvironment::eStockIcons icon ) const{
	DEASSERT_TRUE( icon >= 0 )
	DEASSERT_TRUE( icon < pStockImageCount )
	return pStockIcons[ icon ];
}

const deSkin::Ref &igdeWindowMain::GetStockSkin( igdeEnvironment::eStockSkins skin ) const{
	DEASSERT_TRUE( skin >= 0 )
	DEASSERT_TRUE( skin < pStockSkinCount )
	return pStockSkins[ skin ];
}

const deRig::Ref &igdeWindowMain::GetStockRig( igdeEnvironment::eStockRigs rig ) const{
	DEASSERT_TRUE( rig >= 0 )
	DEASSERT_TRUE( rig < pStockRigCount )
	return pStockRigs[ rig ];
}

const deModel::Ref &igdeWindowMain::GetStockModel( igdeEnvironment::eStockModels model ) const{
	DEASSERT_TRUE( model >= 0 )
	DEASSERT_TRUE( model < pStockModelCount )
	return pStockModels[ model ];
}



bool igdeWindowMain::CloseWindow(){
	if( ! QuitRequest() ){
		return false; // do not close window
	}
	
	return true; // close window
}

void igdeWindowMain::OnWindowState(){
	// NOTE listening to minimize/maximize event is not enough since FOX has a bug when
	//      uses on multi-desktop. switching the desktop causes the minimzed window to
	//      get the maximize event send twice although it is still minimized. by checking
	//      the window state directly this problem can be solved
	/*
	if( isMaximized() && ! isMinimized() ){
		pPauseUpdating = false;
		printf("onMaximized\n");
	}
	*/
	pUpdatePauseUpdating();
}



// IGDE Internal Use Only
///////////////////////////

void igdeWindowMain::CreateNativeWidget(){
	igdeMainWindow::CreateNativeWidget();
}

void igdeWindowMain::DestroyNativeWidget(){
	try{
		pCleanUp();
		
	}catch( const deException &e ){
		e.PrintError();
	}
	
	igdeMainWindow::DestroyNativeWidget();
}



// Private Functions
//////////////////////

void igdeWindowMain::pCleanUp(){
	if( pTimerSyncProject ){
		pTimerSyncProject->Stop();
		pTimerSyncProject = NULL;
	}
	if( pTimerFrameUpdate ){
		pTimerFrameUpdate->Stop();
		pTimerFrameUpdate = NULL;
	}
	
	if( pTaskSyncGameDefinition ){
		delete pTaskSyncGameDefinition;
		pTaskSyncGameDefinition = NULL;
	}
	
	// save configuration. we do this first in case something goes wrong
	if( pVFS ){
		pConfiguration.SaveConfiguration();
	}
	
	// remove toolbars and menus which could be shared from modules before unloading them
	if( pToolBarDockTop ){
		pToolBarDockTop->RemoveAllChildren();
	}
	if( pToolBarDockLeft ){
		pToolBarDockLeft->RemoveAllChildren();
	}
	if( pToolBarDockRight ){
		pToolBarDockRight->RemoveAllChildren();
	}
	if( pToolBarDockBottom ){
		pToolBarDockBottom->RemoveAllChildren();
	}
	if( pMenuBar ){
		pMenuBar->RemoveAllChildren();
	}
	if( pMenuWindow ){
		pMenuWindow->RemoveAllChildren();
	}
	
	// shut down all modules. this is faster than shutting down the game engine first
	// and prevents potential problems
	deLogger * const logger = GetLogger();
	if( logger ){
		logger->LogInfo( LOGSOURCE, "Shut down editor modules..." );
	}
	if( pModuleManager ){
		pModuleManager->StopModules();
		pModuleManager->RemoveAllModules();
	}
	if( logger ){
		logger->LogInfo( LOGSOURCE, "Shut down editor modules finished" );
	}
	
	// shut down game engine
	if( logger ){
		logger->LogInfo( LOGSOURCE, "Shut down game engine..." );
	}
	StopEngine();
	if( logger ){
		logger->LogInfo( LOGSOURCE, "Shut down game engine finished" );
	}
	
	// continue cleaning up. now everything can be simply cleaned up
	if( pModuleManager ){
		delete pModuleManager;
		pModuleManager = NULL;
	}
	
	if( pGameProject ){
		pGameProject->FreeReference();
		pGameProject = NULL;
	}
	if( pIGDEGameDefinition ){
		pIGDEGameDefinition->FreeReference();
		pIGDEGameDefinition = NULL;
	}
	if( pSharedGameDefinitions ){
		delete pSharedGameDefinitions;
		pSharedGameDefinitions = NULL;
	}
	if( pTemplates ){
		delete pTemplates;
		pTemplates = NULL;
	}
	
	if( pSharedFontList ){
		delete pSharedFontList;
		pSharedFontList = NULL;
	}
	if( pUIHelperProperties ){
		delete pUIHelperProperties;
		pUIHelperProperties = NULL;
	}
	if( pUIHelper ){
		delete pUIHelper;
		pUIHelper = NULL;
	}
	
	if( pTexturePropertyList ){
		delete pTexturePropertyList;
		pTexturePropertyList = NULL;
	}
	if( pGDPreviewManager ){
		delete pGDPreviewManager;
		pGDPreviewManager = NULL;
	}
	if( pLoadSaveSystem ){
		delete pLoadSaveSystem;
		pLoadSaveSystem = NULL;
	}
	if( pResourceLoader ){
		delete pResourceLoader;
		pResourceLoader = NULL;
	}
	
	if( pTimer ){
		delete pTimer;
		pTimer = NULL;
	}
	
	pWindowLogger = NULL;
	pVFS = NULL;
}

void igdeWindowMain::pInitLogger(){
	if( ! pLoggerHistory ){
		pLoggerHistory.TakeOver( new igdeLoggerHistory );
		pLoggerHistory->SetHistorySize( 250 );
	}
	
	const deLoggerChain::Ref loggerChain( deLoggerChain::Ref::New( new deLoggerChain ) );
	
	loggerChain->AddLogger( pLoggerHistory );
	
	//no console logging to support console use in scripts
// 	loggerChain->AddLogger( deLoggerConsoleColor::Ref::New( new deLoggerConsoleColor ) );
	
	loggerChain->AddLogger( deLoggerFile::Ref::New( new deLoggerFile(
		decBaseFileWriter::Ref::New( pVFS->OpenFileForWriting(
			decPath::CreatePathUnix( "/logs/deigde.log" ) ) ) ) ) );
	
	pEnvironmentIGDE.SetLogger( loggerChain );
}

void igdeWindowMain::pLoadStockIcons(){
	pStockIcons[ igdeEnvironment::esiNew ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_new.png" ) );
	pStockIcons[ igdeEnvironment::esiOpen ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_open.png" ) );
	pStockIcons[ igdeEnvironment::esiSave ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_save.png" ) );
	pStockIcons[ igdeEnvironment::esiSaveAs ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_save_as.png" ) );
	pStockIcons[ igdeEnvironment::esiDelete ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_delete.png" ) );
	pStockIcons[ igdeEnvironment::esiCopy ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_copy.png" ) );
	pStockIcons[ igdeEnvironment::esiCut ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_cut.png" ) );
	pStockIcons[ igdeEnvironment::esiUndo ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_undo.png" ) );
	pStockIcons[ igdeEnvironment::esiRedo ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_redo.png" ) );
	pStockIcons[ igdeEnvironment::esiQuit ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_quit.png" ) );
	pStockIcons[ igdeEnvironment::esiPaste ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_paste.png" ) );
	pStockIcons[ igdeEnvironment::esiPlus ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_plus.png" ) );
	pStockIcons[ igdeEnvironment::esiMinus ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_minus.png" ) );
	pStockIcons[ igdeEnvironment::esiUp ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_up.png" ) );
	pStockIcons[ igdeEnvironment::esiDown ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_down.png" ) );
	pStockIcons[ igdeEnvironment::esiLeft ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_left.png" ) );
	pStockIcons[ igdeEnvironment::esiRight ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_right.png" ) );
	pStockIcons[ igdeEnvironment::esiStrongUp ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_strong_up.png" ) );
	pStockIcons[ igdeEnvironment::esiStrongDown ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_strong_down.png" ) );
	pStockIcons[ igdeEnvironment::esiStrongLeft ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_strong_left.png" ) );
	pStockIcons[ igdeEnvironment::esiStrongRight ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_strong_right.png" ) );
	pStockIcons[ igdeEnvironment::esiDuplicate ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_duplicate.png" ) );
	pStockIcons[ igdeEnvironment::esiSearch ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_search.png" ) );
	
	pStockIcons[ igdeEnvironment::esiWarning ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_warning.png" ) );
	
	pStockIcons[ igdeEnvironment::esiSmallPlus ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_plus.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallMinus ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_minus.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallUp ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_up.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallDown ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_down.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallLeft ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_left.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallRight ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_right.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallStrongUp ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_strong_up.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallStrongDown ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_strong_down.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallStrongLeft ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_strong_left.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallStrongRight ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_strong_right.png" ) );
	pStockIcons[ igdeEnvironment::esiSmallWarning ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_small_warning.png" ) );
	
	pStockIcons[ igdeEnvironment::esiEdit ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_edit.png" ) );
	pStockIcons[ igdeEnvironment::esiConfig ].TakeOver(
		igdeIcon::LoadPNG( pEnvironmentIGDE, "data/icons/stock_config.png" ) );
}

void igdeWindowMain::pLoadStockSkins(){
	deSkinManager &manager = *GetEngine()->GetSkinManager();
	
	pStockSkins[ igdeEnvironment::essMissing ].TakeOver(
		manager.LoadSkin( "/igde/materials/missing/material.deskin", "/" ) );
	pStockSkins[ igdeEnvironment::essError ].TakeOver(
		manager.LoadSkin( "/igde/materials/error/material.deskin", "/" ) );
	pStockSkins[ igdeEnvironment::essTestMap ].TakeOver(
		manager.LoadSkin( "/igde/materials/testmap/material.deskin", "/" ) );
	pStockSkins[ igdeEnvironment::essEditOutline ].TakeOver(
		manager.LoadSkin( "/igde/materials/editing/outlined.deskin", "/" ) );
	pStockSkins[ igdeEnvironment::essEditRim ].TakeOver(
		manager.LoadSkin( "/igde/materials/editing/rim.deskin", "/" ) );
	pStockSkins[ igdeEnvironment::essEditRimOutline ].TakeOver(
		manager.LoadSkin( "/igde/materials/editing/rimOutlined.deskin", "/" ) );
}

void igdeWindowMain::pLoadStockRigs(){
	deRigManager &manager = *GetEngine()->GetRigManager();
	
	pStockRigs[ igdeEnvironment::esrModelCollision ].TakeOver(
		manager.LoadRig( "/igde/models/modelCollision.derig", "/" ) );
	pStockRigs[ igdeEnvironment::esrGhostCollision ].TakeOver(
		manager.LoadRig( "/igde/models/ghostCollision.derig", "/" ) );
	pStockRigs[ igdeEnvironment::esrGizmoMove ].TakeOver(
		manager.LoadRig( "/igde/models/gizmo/move.derig", "/" ) );
}

void igdeWindowMain::pLoadStockModels(){
	deModelManager &manager = *GetEngine()->GetModelManager();
	
	pStockModels[ igdeEnvironment::esmGizmoMove ].TakeOver(
		manager.LoadModel( "/igde/models/gizmo/move.demodel", "/" ) );
}

void igdeWindowMain::pCreateGuiThemes(){
	// default theme
	pDefaultGuiTheme.TakeOver( new igdeGuiTheme( igdeGuiThemeNames::defaultTheme ) );
	
	pDefaultGuiTheme->SetFloatProperty( igdeGuiThemePropertyNames::fontSize, 1.0f );
	
	pDefaultGuiTheme->SetIntProperty( igdeGuiThemePropertyNames::dialogPaddingLeft, 10 );
	pDefaultGuiTheme->SetIntProperty( igdeGuiThemePropertyNames::dialogPaddingRight, 10 );
	pDefaultGuiTheme->SetIntProperty( igdeGuiThemePropertyNames::dialogPaddingTop, 10 );
	pDefaultGuiTheme->SetIntProperty( igdeGuiThemePropertyNames::dialogPaddingBottom, 10 );
	pDefaultGuiTheme->SetIntProperty( igdeGuiThemePropertyNames::dialogPaddingContent, 10 );
	pDefaultGuiTheme->SetIntProperty( igdeGuiThemePropertyNames::dialogPaddingButtons, 5 );
	
	pGuiThemes.SetAt( pDefaultGuiTheme->GetName(), pDefaultGuiTheme.operator->() );
	
	// properties theme
	igdeGuiThemeReference guitheme;
	guitheme.TakeOver( new igdeGuiTheme( igdeGuiThemeNames::properties, pDefaultGuiTheme ) );
	
	// guitheme->SetFloatProperty( igdeGuiThemePropertyNames::fontSize, 0.85f );
	
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingLeft, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingRight, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingTop, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingBottom, 3 );
	
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingLeft, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingRight, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingTop, 0 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingBottom, 0 );
	
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingLeft, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingRight, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingTop, 0 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingBottom, 0 );
	
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingLeft, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingRight, 3 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingTop, 0 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingBottom, 0 );
	
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::buttonPaddingLeft, 2 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::buttonPaddingRight, 2 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::buttonPaddingTop, 0 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::buttonPaddingBottom, 0 );
	
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingLeft, 0 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingRight, 0 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingTop, 0 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingBottom, 0 );
	
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingLeft, 2 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingRight, 2 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingTop, 2 );
	guitheme->SetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingBottom, 2 );
	
	pGuiThemes.SetAt( guitheme->GetName(), guitheme.operator->() );
}

void igdeWindowMain::pLoadIGDEGameDefinition(){
	decPath path;
	path.SetFromNative( pConfiguration.GetPathShares() );
	path.AddComponent( "igde.degd" );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( new decDiskFileReader( path.GetPathNative() ) );
	
	pIGDEGameDefinition = new igdeGameDefinition( pEnvironmentIGDE );
	pIGDEGameDefinition->SetFilename( path.GetPathNative() );
	pIGDEGameDefinition->SetBasePath( pConfiguration.GetPathShares() );
	
	igdeXMLGameDefinition( pEnvironmentIGDE, GetLogger() ).Load( reader, *pIGDEGameDefinition );
	
	GetLogger()->LogInfoFormat( "IGDE", "IGDE Game Definition find content in %s",
		pConfiguration.GetPathIGDEData().GetString() );
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference diskContainer;
	diskContainer.TakeOver( new deVFSDiskDirectory( decPath::CreatePathUnix( "/igde" ),
		decPath::CreatePathNative( pConfiguration.GetPathIGDEData() ) ) );
	vfs->AddContainer( diskContainer );
	
	igdeGDClassManager foundClasses;
	pIGDEGameDefinition->FindClasses( vfs, foundClasses );
	pIGDEGameDefinition->GetClassManager()->UpdateWithElementClasses( foundClasses );

	igdeGDSkinManager foundSkins;
	pIGDEGameDefinition->FindSkins( vfs, foundSkins );
	pIGDEGameDefinition->GetSkinManager()->UpdateWithFound( foundSkins );
	
	igdeGDSkyManager foundSkies;
	pIGDEGameDefinition->FindSkies( vfs, foundSkies );
	pIGDEGameDefinition->GetSkyManager()->UpdateWithFound( foundSkies );
}

void igdeWindowMain::pAddIGDEEngineModules(){
	igdeEngineController &engineController = GetEngineController();
	deModuleSystem *modsys = engineController.GetEngine()->GetModuleSystem();
	deObjectReference module;
	
	module.TakeOver( new igdeScriptModule::cModule( modsys, *this ) );
	engineController.AddInternalModule( ( deInternalModule* )( deObject* )module );
}

void igdeWindowMain::pLoadTexturePropertyList(){
	deLogger &logger = *GetLogger();
	
	decPath pathFile;
	pathFile.SetFromUnix( "/data/texturePropertyList.xml" );
	
	if( ! pVFS->ExistsFile( pathFile ) ){
		logger.LogInfo( LOGSOURCE, "Texture property list file not found, skipped" );
		return;
	}
	
	igdeXMLLoadTexturePropertyList readXML( &logger );
	decBaseFileReaderReference reader;
	
	if( pVFS->GetFileType( pathFile ) != deVFSContainer::eftRegularFile ){
		logger.LogError( LOGSOURCE, "Texture property list file is not a regular file" );
		DETHROW( deeInvalidParam );
	}
	
	logger.LogInfo( LOGSOURCE, "Reading texture property list file" );
	reader.TakeOver( pVFS->OpenFileForReading( pathFile ) );
	readXML.ReadFromFile( *pTexturePropertyList, reader );
}

void igdeWindowMain::pLoadTemplates(){
	deLogger &logger = *GetLogger();
	logger.LogInfo( LOGSOURCE, "Loading project templates" );
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	const decPath basePath( decPath::CreatePathNative( pConfiguration.GetPathIGDETemplates() ) );
	deVFSContainerReference container;
	container.TakeOver( new deVFSDiskDirectory( basePath ) );
	vfs->AddContainer( container );
	
	// find all templates located in top level directories inside the templates directories
	deCollectDirectorySearchVisitor collectDirectories;
	vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collectDirectories );
	
	const dePathList &dirList = collectDirectories.GetDirectories();
	const int dirCount = dirList.GetCount();
	dePathList pathList;
	int i;
	
	for( i=0; i<dirCount; i++ ){
		decPath pathXml( dirList.GetAt( i ) );
		pathXml.AddComponent( "template.xml" );
		if( vfs->ExistsFile( pathXml ) ){
			pathList.Add( pathXml );
		}
	}
	
	// load found templates
	igdeLoadTemplate loadTemplate( &logger );
	const int count = pathList.GetCount();
	decBaseFileReaderReference reader;
	
	for( i=0; i<count; i++ ){
		igdeTemplate *atemplate = NULL;
		
		decPath path( basePath );
		path.Add( pathList.GetAt( i ) );
		const decString pathXml( path.GetPathNative() );
		path.RemoveLastComponent();
		const decString pathTemplate( path.GetPathNative() );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading project template '%s'",
			path.GetLastComponent().GetString() );
		
		try{
			reader.TakeOver( new decDiskFileReader( pathXml ) );
			
			atemplate = new igdeTemplate;
			atemplate->SetBasePath( pathTemplate );
			loadTemplate.Load( reader, *atemplate );
			
		}catch( const deException &e ){
			if( atemplate ){
				atemplate->FreeReference();
			}
			logger.LogException( LOGSOURCE, e );
			continue;
		}
		
		if( atemplate->GetName().IsEmpty() ){
			logger.LogWarn( LOGSOURCE, "Project template has empty name, skipped" );
			atemplate->FreeReference();
			continue;
		}
		
		pTemplates->Add( atemplate );
		atemplate->FreeReference();
	}
}

void igdeWindowMain::pLoadSharedGameDefinitions(){
	deLogger &logger = *GetLogger();
	logger.LogInfo( LOGSOURCE, "Loading shared game definitions" );
	
	const decPath gameDefPath( decPath::CreatePathNative( pConfiguration.GetPathIGDEGameDefs() ) );
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	container.TakeOver( new deVFSDiskDirectory( gameDefPath ) );
	vfs->AddContainer( container );
	
	deCollectFileSearchVisitor collectFiles( "*.degd" );
	vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collectFiles );
	
	const decPath sharePath(decPath::CreatePathNative(GetEngine()->GetOS()->GetPathShare()));
	const deVirtualFileSystem::Ref &vfsAssetLibraries =
		GetEngine()->GetModuleSystem()->GetVFSAssetLibraries();
	
	const dePathList &pathList = collectFiles.GetFiles();
	igdeXMLGameDefinition loadGameDef( pEnvironmentIGDE, &logger );
	const int count = pathList.GetCount();
	decBaseFileReaderReference reader;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGameDefinition *gameDefinition = NULL;
		
		decPath path( gameDefPath );
		path.Add( pathList.GetAt( i ) );
		logger.LogInfoFormat( LOGSOURCE, "Loading shared game definition '%s'",
			pathList.GetAt( i ).GetLastComponent().GetString() );
			
		try{
			reader.TakeOver( new decDiskFileReader( path.GetPathNative() ) );
			
			gameDefinition = new igdeGameDefinition( pEnvironmentIGDE );
			gameDefinition->SetFilename( path.GetPathNative() );
			loadGameDef.Load( reader, *gameDefinition );
			
		}catch( const deException &e ){
			if( gameDefinition ){
				gameDefinition->FreeReference();
			}
			logger.LogException( LOGSOURCE, e );
			continue;
		}
		
		if( gameDefinition->GetID().IsEmpty() ){
			logger.LogWarn( LOGSOURCE, "Shared game definition has empty identifier, skipped" );
			gameDefinition->FreeReference();
			continue;
		}
		
		if( pSharedGameDefinitions->HasWithID( gameDefinition->GetID() ) ){
			logger.LogWarn( LOGSOURCE, "Shared game definition has has duplicate identifier, skipped" );
			gameDefinition->FreeReference();
			continue;
		}
		
		logger.LogInfoFormat( "IGDE", "Shared game definition find content in %s",
			gameDefinition->GetBasePath().GetString() );
		
		vfs.TakeOver( new deVirtualFileSystem );
		
		const decPath diskPath(decPath::CreatePathNative(gameDefinition->GetBasePath()));
		const decPath rootPath(decPath::CreatePathUnix(gameDefinition->GetVFSPath()));
		vfs->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(rootPath, diskPath, true)));
		
		if(sharePath.IsParentOf(diskPath) && vfsAssetLibraries->GetContainerCount() > 0){
			decPath relPath(diskPath.RelativePath(sharePath, true));
			relPath.SetPrefix("/");
			vfs->AddContainer(deVFSRedirect::Ref::New(new deVFSRedirect(
				rootPath, relPath, vfsAssetLibraries, true)));
		}
		
		igdeGDClassManager foundClasses;
		gameDefinition->FindClasses( vfs, foundClasses );
		gameDefinition->GetClassManager()->UpdateWithElementClasses( foundClasses );
		
		igdeGDSkinManager foundSkins;
		gameDefinition->FindSkins( vfs, foundSkins );
		gameDefinition->GetSkinManager()->UpdateWithFound( foundSkins );
		
		igdeGDSkyManager foundSkies;
		gameDefinition->FindSkies( vfs, foundSkies );
		gameDefinition->GetSkyManager()->UpdateWith( foundSkies );
		
		pSharedGameDefinitions->Add( gameDefinition );
		gameDefinition->FreeReference();
	}
}

void igdeWindowMain::pCreateActions(){
	pActionGameNew.TakeOver( new cActionGameNew( *this ) );
	pActionGameOpen.TakeOver( new cActionGameOpen( *this ) );
	pActionGameSave.TakeOver( new cActionGameSave( *this ) );
	pActionGameSaveAs.TakeOver( new cActionGameSaveAs( *this ) );
	pActionGameSettings.TakeOver( new cActionGameSettings( *this ) );
	pActionGameQuit.TakeOver( new cActionGameQuit( *this ) );
	pActionGameReloadXMLElementClasses.TakeOver( new cActionGameReloadXMLElementClasses( *this ) );
	
	pActionSettingsEngine.TakeOver( new cActionSettingsEngine( *this ) );
	pActionSettingsIgde.TakeOver( new cActionSettingsIgde( *this ) );
	pActionSettingsTexPropList.TakeOver( new cActionSettingsTexPropList( *this ) );
	pActionSettingsLogging.TakeOver( new cActionSettingsLogging( *this ) );
}

void igdeWindowMain::pCreateSharedModelCollisionRig(){
	class igdeWindowMain_SharedModelCollisionRigBuilder : public deRigBuilder{
	public:
		virtual void BuildRig( deRig *rig ){
			rig->SetModelCollision( true );
		}
	} builder;
	pSharedModelCollisionRig.TakeOver( GetEngineController().GetEngine()->GetRigManager()
		->CreateRig( "_#IGDE_SharedModelCollisionRig", builder ) );
}



void igdeWindowMain::pCreateToolBarGame(){
	pTBGame.TakeOver( new igdeToolBar( pEnvironmentIGDE ) );
	pUIHelper->ToolBarButton( pTBGame, pActionGameNew );
	pUIHelper->ToolBarButton( pTBGame, pActionGameOpen );
	pUIHelper->ToolBarButton( pTBGame, pActionGameSave );
	pUIHelper->ToolBarButton( pTBGame, pActionGameReloadXMLElementClasses );
}

void igdeWindowMain::pRebuildToolBarEditors(){
	pFraEditors->RemoveAllChildren();
	
	decStringList moduleIDs;
	int i;
	
	for( i=0; i<pModuleManager->GetModuleCount(); i++ ){
		const igdeEditorModuleDefinition &module = *pModuleManager->GetModuleAt( i );
		if( module.IsModuleRunning() ){
			moduleIDs.Add( module.GetID() );
		}
	}
	moduleIDs.SortAscending();
	
	const int count = moduleIDs.GetCount();
	igdeToggleButtonReference button;
	igdeActionReference action;
	
	for( i=0; i<count; i++ ){
		igdeEditorModuleDefinition &moduleDef = *pModuleManager->GetModuleWithID( moduleIDs.GetAt( i ) );
		if( ! moduleDef.IsModuleRunning() || ! moduleDef.GetModule() ){
			continue;
		}
		
		igdeIconReference icon;
		if( ! moduleDef.GetIconSmall().IsEmpty() ){
			try{
				icon.TakeOver( igdeIcon::LoadPNG( *moduleDef.GetModule(), moduleDef.GetIconSmall() ) );
				
			}catch( const deException &e ){
				GetLogger()->LogException( LOGSOURCE, e );
			}
		}
		
		if( ! icon ){
			icon = pStockIcons[ igdeEnvironment::esiWarning ];
		}
		
		//pUIHelper->ToolBarToggleButton( pTBEditors, new cActionWindowEditor( *this, moduleDef, true/*false*/ ), true );
		
		action.TakeOver( new cActionWindowEditor( *this, moduleDef, true/*false*/ ) );
		action->SetText( "" );
		
		pUIHelper->ToggleButton( button, action );
		button->SetStyle( igdeButton::ebsToolBar );
		pFraEditors->AddChild( button );
	}
}



void igdeWindowMain::pCreateMenu(){
	pMenuGame.TakeOver( new igdeMenuCascade( pEnvironmentIGDE, "Game", deInputEvent::ekcG ) );
	pCreateMenuGame( pMenuGame );
	
	pMenuSettings.TakeOver( new igdeMenuCascade( pEnvironmentIGDE, "Settings", deInputEvent::ekcS ) );
	pCreateMenuSettings( pMenuSettings );
	
	pMenuWindow.TakeOver( new igdeMenuCascade( pEnvironmentIGDE, "Window", deInputEvent::ekcW ) );
}

void igdeWindowMain::pCreateMenuGame( igdeMenuCascade &menu ){
	igdeWidgetReference entry;
	
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionGameNew ) );
	menu.AddChild( entry );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionGameOpen ) );
	menu.AddChild( entry );
	pMenuRecentProjects.TakeOver( new igdeMenuCascade( pEnvironmentIGDE,
		"Open recent project", pIconGameOpen ) );
	menu.AddChild( pMenuRecentProjects );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionGameSave ) );
	menu.AddChild( entry );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionGameSaveAs ) );
	menu.AddChild( entry );
	
	entry.TakeOver( new igdeMenuSeparator( pEnvironmentIGDE ) );
	menu.AddChild( entry );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionGameSettings ) );
	menu.AddChild( entry );
	
	entry.TakeOver( new igdeMenuSeparator( pEnvironmentIGDE ) );
	menu.AddChild( entry );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionGameReloadXMLElementClasses ) );
	menu.AddChild( entry );
	
	entry.TakeOver( new igdeMenuSeparator( pEnvironmentIGDE ) );
	menu.AddChild( entry );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionGameQuit ) );
	menu.AddChild( entry );
}

void igdeWindowMain::pCreateMenuSettings( igdeMenuCascade &menu ){
	igdeWidgetReference entry;
	
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionSettingsEngine ) );
	menu.AddChild( entry );
// 	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionSettingsIgde ) );
// 	menu.AddChild( entry );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionSettingsTexPropList ) );
	menu.AddChild( entry );
	entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, pActionSettingsLogging ) );
	menu.AddChild( entry );
}

void igdeWindowMain::pUpdateMenuRecentProjects( igdeMenuCascade &menu ){
	const decStringList &list = pConfiguration.GetRecentProjectList();
	const int count = list.GetCount();
	igdeActionReference action;
	igdeWidgetReference entry;
	int i;
	
	menu.RemoveAllChildren();
	
	for( i=0; i<count; i++ ){
		action.TakeOver( new cActionGameOpenRecent( *this, list.GetAt( i ) ) );
		entry.TakeOver( new igdeMenuCommand( pEnvironmentIGDE, action ) );
		menu.AddChild( entry );
	}
}

void igdeWindowMain::pUpdateMenuWindow( igdeMenuCascade &menu ){
	// remove all windows from menu
	menu.RemoveAllChildren();
	
	// find all running modules and sort them by name
	const int moduleCount = pModuleManager->GetModuleCount();
	decStringList addModules;
	int i;
	
	for( i=0; i<moduleCount; i++ ){
		const igdeEditorModuleDefinition &module = *pModuleManager->GetModuleAt( i );
		if( module.IsModuleRunning() ){
			addModules.Add( module.GetID() );
		}
	}
	addModules.SortAscending();
	
	// add a menu entry for each module
	const int addModuleCount = addModules.GetCount();
	igdeActionReference action;
	igdeWidgetReference entry;
	
	for( i=0; i<addModuleCount; i++ ){
		action.TakeOver( new cActionWindowEditor( *this,
			*pModuleManager->GetModuleWithID( addModules.GetAt( i ) ), true ) );
		entry.TakeOver( new igdeMenuOption( pEnvironmentIGDE, action ) );
		menu.AddChild( entry );
	}
}



void igdeWindowMain::pUpdatePauseUpdating(){
	pPauseUpdating = GetWindowState() == ewsMinimized;
}

void igdeWindowMain::pLoadXMLElementClasses( igdeGameProject &gameProject ){
	decTimer timer;
	igdeXMLElementClass loader( GetLogger() );
	gameProject.GetXMLEClassGameDefinition()->GetClassManager()->RemoveAll();
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	decPath pathData( decPath::CreatePathNative( gameProject.GetDirectoryPath() ) );
	pathData.AddUnixPath( gameProject.GetPathData() );
	container.TakeOver( new deVFSDiskDirectory( pathData ) );
	vfs->AddContainer( container );
	
	const decStringList &pathList = gameProject.GetProjectGameDefinition()->GetClassManager()->GetAutoFindPath();
	const int pathCount = pathList.GetCount();
	int i;
	for( i=0; i<pathCount; i++ ){
		const decString &path = pathList.GetAt( i );
		GetLogger()->LogInfoFormat( LOGSOURCE, "Load XML Element Classes: %s", path.GetString() );
		loader.LoadElementClasses(*gameProject.GetXMLEClassGameDefinition()->GetClassManager(),
			vfs, decPath::CreatePathUnix(path));
	}
	GetLogger()->LogInfoFormat( LOGSOURCE, "Load XML Element Classes done: %.1fs (%d found)",
		timer.GetElapsedTime(), gameProject.GetXMLEClassGameDefinition()->GetClassManager()->GetCount() );
	
	// DEBUG
	/*
	const igdeGDClassManager &gdClasses = *gameProject.GetXMLEClassGameDefinition()->GetClassManager();
	const int count = gdClasses.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const igdeGDClass &gdClass = *gdClasses.GetAt( i );
		GetLogger()->LogInfoFormat( LOGSOURCE, "XMLEClass: %s%s", gdClass.GetName().GetString(),
			projectClasses.HasNamed( gdClass.GetName() ) ? "(existing in project)" : "" );
	}
	*/
	// DEBUG
}

void igdeWindowMain::pFindAndAddSkins( igdeGameProject &gameProject ){
	decTimer timer;
	const igdeFilePatternList &patterns = pLoadSaveSystem->GetOpenFilePatternList( igdeLoadSaveSystem::efplSkin );
	igdeGameDefinition &gameDefinition = *gameProject.GetFoundGameDefinition();
	
	igdeGDSkinManager &gdskins = *gameDefinition.GetSkinManager();
	gdskins.RemoveAllSkins();
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	decPath pathData( decPath::CreatePathNative( gameProject.GetDirectoryPath() ) );
	pathData.AddUnixPath( gameProject.GetPathData() );
	container.TakeOver( new deVFSDiskDirectory( pathData ) );
	vfs->AddContainer( container );
	
	const decStringList &pathList = gameProject.GetProjectGameDefinition()->GetSkinManager()->GetAutoFindPath();
	const int pathCount = pathList.GetCount();
	int i, j;
	
	for( i=0; i<pathCount; i++ ){
		const decString &path = pathList.GetAt( i );
		GetLogger()->LogInfoFormat( LOGSOURCE, "Find Skins: %s", path.GetString() );
		
		const int patternCount = patterns.GetFilePatternCount();
		const decPath searchPath( decPath::CreatePathUnix( path ) );
		
		for( j=0; j<patternCount; j++ ){
			const igdeFilePattern &pattern = *patterns.GetFilePatternAt( j );
			gdskins.FindAndAddSkins( vfs, searchPath, pattern.GetPattern() );
		}
	}
	GetLogger()->LogInfoFormat( LOGSOURCE, "Find Skins done: %.1fs (%d found)",
		timer.GetElapsedTime(), gdskins.GetSkinCount() );
}

void igdeWindowMain::pFindAndAddSkies( igdeGameProject &gameProject ){
	decTimer timer;
	const igdeFilePatternList &patterns = pLoadSaveSystem->GetOpenFilePatternList( igdeLoadSaveSystem::efplSky );
	igdeGameDefinition &gameDefinition = *gameProject.GetFoundGameDefinition();
	
	igdeGDSkyManager &gdskies = *gameDefinition.GetSkyManager();
	gdskies.RemoveAllSkies();
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	decPath pathData( decPath::CreatePathNative( gameProject.GetDirectoryPath() ) );
	pathData.AddUnixPath( gameProject.GetPathData() );
	container.TakeOver( new deVFSDiskDirectory( pathData ) );
	vfs->AddContainer( container );
	
	const decStringList &pathList = gameProject.GetProjectGameDefinition()->GetSkyManager()->GetAutoFindPath();
	const int pathCount = pathList.GetCount();
	int i, j;
	
	for( i=0; i<pathCount; i++ ){
		const decString &path = pathList.GetAt( i );
		GetLogger()->LogInfoFormat( LOGSOURCE, "Find Skies: %s", path.GetString() );
		
		const int patternCount = patterns.GetFilePatternCount();
		const decPath searchPath( decPath::CreatePathUnix( path ) );
		
		for( j=0; j<patternCount; j++ ){
			const igdeFilePattern &pattern = *patterns.GetFilePatternAt( j );
			gdskies.FindAndAddSkies( vfs, searchPath, pattern.GetPattern() );
		}
	}
	GetLogger()->LogInfoFormat( LOGSOURCE, "Find Skies done: %.1fs (%d found)",
		timer.GetElapsedTime(), gdskies.GetSkyList().GetCount() );
}
