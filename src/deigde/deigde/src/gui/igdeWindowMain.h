/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWINDOWMAIN_H_
#define _IGDEWINDOWMAIN_H_

#include "../configuration/igdeConfiguration.h"
#include "../configuration/igdeConfigurationLocal.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/logger/igdeLoggerHistory.h>
#include <deigde/gui/igdeMainWindow.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/igdeProgressBarReference.h>
#include <deigde/gui/igdeStatusBarReference.h>
#include <deigde/gui/igdeTimerReference.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/igdeToolBarDockReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/logger/igdeWindowLoggerReference.h>
#include <deigde/gui/menu/igdeMenuBarReference.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/resources/igdeIconReference.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/theme/igdeGuiThemeReference.h>
#include <deigde/resourceloader/igdeResourceLoader.h>

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/logger/deLoggerReference.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/resources/rig/deRigReference.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/skin/deSkin.h>


class igdeEditorModuleManager;
class igdeEnvironmentIGDE;
class igdeGDPreviewManager;
class igdeGameProject;
class igdeLoadSaveSystem;
class igdeTaskSyncGameDefinition;
class igdeTemplateList;
class igdeTexturePropertyList;
class igdeSharedFontList;
class igdeUIHelper;

class deException;
class decTimer;
class igdeGameDefinition;
class igdeGameDefinitionList;
class decUnicodeStringList;



/**
 * IGDE Main Window.
 */
class igdeWindowMain : public igdeMainWindow{
private:
	igdeEnvironmentIGDE &pEnvironmentIGDE;
	igdeConfiguration pConfiguration;
	igdeConfigurationLocal pConfigurationLocal;
	igdeEditorModuleManager *pModuleManager;
	igdeLoadSaveSystem *pLoadSaveSystem;
	igdeGDPreviewManager *pGDPreviewManager;
	igdeTexturePropertyList *pTexturePropertyList;
	igdeLoggerHistory::Ref pLoggerHistory;
	igdeWindowLoggerReference pWindowLogger;
	deVirtualFileSystemReference pVFS;
	igdeTemplateList *pTemplates;
	igdeGameDefinitionList *pSharedGameDefinitions;
	igdeGameDefinition *pIGDEGameDefinition;
	igdeGameProject *pGameProject;
	igdeGuiThemeReference pDefaultGuiTheme;
	decObjectDictionary pGuiThemes;
	igdeSharedFontList *pSharedFontList;
	deRigReference pSharedModelCollisionRig;
	igdeResourceLoader *pResourceLoader;
	igdeUIHelper *pUIHelper;
	igdeUIHelper *pUIHelperProperties;
	igdeTimerReference pTimerFrameUpdate;
	igdeTimerReference pTimerSyncProject;
	
	
	igdeIconReference pIconApplication;
	
	igdeIconReference pIconGameNew;
	igdeIconReference pIconGameOpen;
	igdeIconReference pIconGameSave;
	igdeIconReference pIconGameSaveAs;
	igdeIconReference pIconGameExit;
	igdeIconReference pIconGameReloadXMLElementClasses;
	igdeIconReference pIconSettingsIGDE;
	igdeIconReference pIconSettingsEngine;
	igdeIconReference pIconSettingsTexPropList;
	
	static const int pStockImageCount = igdeEnvironment::esiSmallWarning + 1;
	igdeIconReference pStockIcons[ pStockImageCount ];
	
	static const int pStockSkinCount = igdeEnvironment::essEditRimOutline + 1;
	deSkin::Ref pStockSkins[ pStockSkinCount ];
	
	static const int pStockRigCount = igdeEnvironment::esrGhostCollision + 1;
	deRig::Ref pStockRigs[ pStockRigCount ];
	
	igdeActionReference pActionGameNew;
	igdeActionReference pActionGameOpen;
	igdeActionReference pActionGameSave;
	igdeActionReference pActionGameSaveAs;
	igdeActionReference pActionGameSettings;
	igdeActionReference pActionGameQuit;
	igdeActionReference pActionGameReloadXMLElementClasses;
	igdeActionReference pActionSettingsIgde;
	igdeActionReference pActionSettingsEngine;
	igdeActionReference pActionSettingsModules;
	igdeActionReference pActionSettingsTexPropList;
	igdeActionReference pActionSettingsLogging;
	
	igdeMenuCascadeReference pMenuGame;
	igdeMenuCascadeReference pMenuRecentProjects;
	igdeMenuCascadeReference pMenuSettings;
	igdeMenuCascadeReference pMenuWindow;
	
	igdeMenuBarReference pMenuBar;
	
	igdeToolBarDockReference pToolBarDockLeft;
	igdeToolBarDockReference pToolBarDockTop;
	igdeToolBarDockReference pToolBarDockRight;
	igdeToolBarDockReference pToolBarDockBottom;
	igdeStatusBarReference pStatusBar;
	igdeProgressBarReference pSBProgress;
	igdeSwitcherReference pSwiContent;
	
	igdeToolBarReference pTBGame;
	igdeContainerReference pFraEditors;
	
	float pElapsedTime;
	float pMinUpdateTime;
	decTimer *pTimer;
	
	bool pDisplayFPSInfo;
	float pFPSAccum;
	int pFPSFrames;
	int pFPSRate;
	
	bool pPauseUpdating;
	bool pFirstEngineRun;
	
	igdeTaskSyncGameDefinition *pTaskSyncGameDefinition;
	
	decUnicodeStringList pAfterLoadArguments;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create main window. */
	igdeWindowMain( igdeEnvironmentIGDE &environment );
	
protected:
	/** Clean up main window. */
	virtual ~igdeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Configuration. */
	inline igdeConfiguration &GetConfiguration(){ return pConfiguration; }
	inline const igdeConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** Local Configuration. */
	inline igdeConfigurationLocal &GetConfigurationLocal(){ return pConfigurationLocal; }
	inline const igdeConfigurationLocal &GetConfigurationLocal() const{ return pConfigurationLocal; }
	
	/** Retrieves the editor module manager. */
	inline igdeEditorModuleManager &GetModuleManager() const{ return *pModuleManager; }
	
	/** Retrieves the load save system. */
	inline igdeLoadSaveSystem *GetLoadSaveSystem() const{ return pLoadSaveSystem; }
	/** Retrieves the game definition preview manager. */
	inline igdeGDPreviewManager *GetGDPreviewManager() const{ return pGDPreviewManager; }
	/** Retrieves the texture property list. */
	inline igdeTexturePropertyList *GetTexturePropertyList() const{ return pTexturePropertyList; }
	/** Retrieves the history logger. */
	inline igdeLoggerHistory *GetLoggerHistory() const{ return pLoggerHistory; }
	
	/** Virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** Project templates. */
	inline igdeTemplateList &GetTemplates() const{ return *pTemplates; }
	
	/** Shared game definitions. */
	inline igdeGameDefinitionList &GetSharedGameDefinitions() const{ return *pSharedGameDefinitions; }
	
	/** Retrieves the igde game definition. */
	inline igdeGameDefinition *GetIGDEGameDefinition() const{ return pIGDEGameDefinition; }
	
	/** Logger window or NULL if not visible. */
	inline igdeWindowLogger *GetWindowLogger() const{ return pWindowLogger; }
	
	/** Named GuiTheme or default if not found. */
	igdeGuiTheme *GetGuiThemeNamed( const char *name );
	
	/** Default GuiTheme. */
	inline igdeGuiTheme *GetDefaultGuiTheme() const{ return pDefaultGuiTheme; }
	
	/** Shared font list. */
	inline igdeSharedFontList &GetSharedFontList() const{ return *pSharedFontList; }
	
	/** Shared model collision rig. */
	inline deRig *GetSharedModelCollisionRig() const{ return pSharedModelCollisionRig; }
	
	/** Resource loader. */
	inline igdeResourceLoader &GetResourceLoader() const{ return *pResourceLoader; }
	
	/** UI Helper. */
	inline igdeUIHelper &GetUIHelper() const{ return *pUIHelper; }
	
	/** UI Helper for properties panels. */
	inline igdeUIHelper &GetUIHelperProperties() const{ return *pUIHelperProperties; }
	
	/**
	 * Show logger window.
	 * 
	 * If not present yet it is created otherwise it is focused and raised.
	 */
	void ShowWindowLogger();
	
	/**
	 * Process command line.
	 * \returns True to keep the application running or false to shut it down.
	 */
	bool ProcessCommandLine( const decUnicodeStringList &arguments );
	
	/** Retrieves the active game project. */
	inline igdeGameProject *GetGameProject() const{ return pGameProject; }
	/** Sets the active game project. */
	void SetGameProject( igdeGameProject *project );
	/**
	 * Creates a new game project.
	 * \returns True of the project has been created or false otherwise.
	 */
	bool CreateNewGameProject();
	/** Creates a placeholder game project. */
	void CreatePlaceholderGameProject();
	/** Load game project. */
	bool LoadGameProject( const char *filename );
	/** Save game project. */
	void SaveGameProject( const char *filename );
	/** Add entry to recently loaded file list and update the menus. */
	void AddRecentGameProject( const char *filename );
	
	/** Create a new game definition. */
	igdeGameDefinition *CreateNewGameDefinition();
	
	/** Displays an exception error. */
	void DisplayException( const deException &exception );
	
	/** Active module shared menus changed. */
	void ActiveModuleSharedMenusChanged();
	
	/** Active module shared toolbars changed. */
	void ActiveModuleSharedToolBarsChanged();
	
	/** Activate editor and bring it to the front. */
	void ActivateEditor( igdeEditorModule *editor );
	
	/** Rebuilds the menu using the given game definition. */
	void RebuildMenu();
	
	/** Rebuild toolbars. */
	void RebuildToolBars();
	
	/** Switch to module window. */
	void SwitchToModuleWindow();
	
	/** Rebuilds the window menu with modules. */
	void RebuildWindowMenu();
	/** Update recent project menu. */
	void UpdateRecentProjectMenu();
	
	/** Sync game definition task is running. */
	bool IsSyncGameDefTaskRunning() const;
	
	/** Retrieves the menu bar. */
	inline igdeMenuBar *GetMenuBar() const{ return pMenuBar; }
	
	/** Toolbar dock sites. */
	inline igdeToolBarDock *GetDockSiteLeft() const{ return pToolBarDockLeft; }
	inline igdeToolBarDock *GetDockSiteTop() const{ return pToolBarDockTop; }
	inline igdeToolBarDock *GetDockSiteRight() const{ return pToolBarDockRight; }
	inline igdeToolBarDock *GetDockSiteBottom() const{ return pToolBarDockBottom; }
	
	/** Content switcher. */
	inline igdeSwitcher *GetContentSwitcher() const{ return pSwiContent; }
	
	/** Game toolbar. */
	inline igdeToolBar *GetToolBarGame() const{ return pTBGame; }
	
	
	
	/** Icons. */
	inline igdeIcon *GetIconApplication() const{ return pIconApplication; }
	
	inline igdeIcon *GetIconGameNew() const{ return pIconGameNew; }
	inline igdeIcon *GetIconGameOpen() const{ return pIconGameOpen; }
	inline igdeIcon *GetIconGameSave() const{ return pIconGameSave; }
	inline igdeIcon *GetIconGameSaveAs() const{ return pIconGameSaveAs; }
	inline igdeIcon *GetIconGameExit() const{ return pIconGameExit; }
	inline igdeIcon *GetIconGameReloadXMLElementClasses() const{ return pIconGameReloadXMLElementClasses; }
	inline igdeIcon *GetIconSettingsIgde() const{ return pIconSettingsIGDE; }
	inline igdeIcon *GetIconSettingsEngine() const{ return pIconSettingsEngine; }
	inline igdeIcon *GetIconSettingsTexPropList() const{ return pIconSettingsTexPropList; }
	
	
	
	/** Reload XML Element Classes. */
	void ReloadXMLElementClasses();
	
	/** Re-find and add skins. */
	void ReFindAndAddSkins();
	
	/** Re-find and add skies. */
	void ReFindAndAddSkies();
	
	
	
	/** Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible( bool visible );
	/** Sets the progress bar progress. */
	void SetProgress( float progress );
	/** Sets the progress text. */
	void SetProgressText( const char *text );
	
	/** Retrieves the minimum frame update time. */
	inline float GetMinUpdateTime() const{ return pMinUpdateTime; }

	/** Sets the minimum frame update time. */
	void SetMinUpdateTime( float seconds );
	
	/** Hook method called before the engine starts. */
	virtual void OnBeforeEngineStart();
	/** Hook method called after the engine has been started. */
	virtual void OnAfterEngineStart();
	/** Hook method called before the engine is shut down. */
	virtual void OnBeforeEngineStop();
	/** Hook method called before the engine is shut down. */
	virtual void OnAfterEngineStop();
	
	/** Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	
	/**
	 * Processes frame update messages.
	 * 
	 * Called every now and then if the message loop has ceased processing messages.
	 */
	virtual void OnFrameUpdate();
	
	/**
	 * Project game definition changed.
	 * 
	 * For use by game definition editor. All modules are notified the game definition change.
	 * This process is potentially lengthy. User input is blocked until all modules finished
	 * processing the changes.
	 */
	void OnProjectGameDefinitionChanged();
	
	/** Arm the update timer. */
	void ArmUpdateTimer();
	
	/**
	 * Update frame.
	 * \details Called by the IGDE Script Module during a single frame update. This allows
	 *          the use of deEngine::RunSingleFrame(). Think of it as a callback.
	 */
	void UpdateFrame();
	
	/** Update synchronize project. */
	void UpdateSyncProject();
	
	/**
	 * Request saving unsaved documents.
	 * 
	 * If unsaved documents are present asks the user if he wants to save them. The user
	 * is free to decide which documents to save. He can also abort the saving.
	 * 
	 * Checks first game project changed or any editor module has changed documents. If
	 * this is the case a list is shown to the user with all changed documents. He can
	 * then enable those documents he would like to save. If there are any documents
	 * selected the respective modules are told to save the documents.
	 * 
	 * \param[in] title Title of the unsaved documents dialog if shown.
	 * \param[in] message Message of the unsaved documents dialog if shown.
	 *                    Message is optional and can be \em NULL.
	 * \returns \em true if user wants to continue the action or \em false to abort.
	 */
	bool RequestSaveDocuments( const char *title, const char* message );
	/*@}*/
	
	/** \name Stock resources */
	/*@{*/
	igdeIcon *GetStockIcon( igdeEnvironment::eStockIcons icon ) const;
	const deSkin::Ref &GetStockSkin( igdeEnvironment::eStockSkins skin ) const;
	const deRig::Ref &GetStockRig( igdeEnvironment::eStockRigs rig ) const;
	/*@}*/
	
	
	
	/** Request to close window due to clicking on close button. */
	virtual bool CloseWindow();
	
	/** Window state changed. */
	virtual void OnWindowState();
	
	
	
public:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitLogger();
	void pLoadStockIcons();
	void pLoadStockSkins();
	void pLoadStockRigs();
	void pCreateGuiThemes();
	void pLoadIGDEGameDefinition();
	void pAddIGDEEngineModules();
	void pLoadTexturePropertyList();
	void pLoadTemplates();
	void pLoadSharedGameDefinitions();
	void pCreateActions();
	void pCreateSharedModelCollisionRig();
	
	void pCreateToolBarGame();
	void pRebuildToolBarEditors();
	
	void pCreateMenu();
	void pCreateMenuGame( igdeMenuCascade &menu );
	void pCreateMenuSettings( igdeMenuCascade &menu );
	void pUpdateMenuRecentProjects( igdeMenuCascade &menu );
	void pUpdateMenuWindow( igdeMenuCascade &menu );
	
	void pUpdatePauseUpdating();
	
	void pLoadXMLElementClasses( igdeGameProject &gameProject );
	void pFindAndAddSkins( igdeGameProject &gameProject );
	void pFindAndAddSkies( igdeGameProject &gameProject );
};

#endif
