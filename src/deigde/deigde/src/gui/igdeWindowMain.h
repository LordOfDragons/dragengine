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

#ifndef _IGDEWINDOWMAIN_H_
#define _IGDEWINDOWMAIN_H_

#include "../configuration/igdeConfiguration.h"
#include "../configuration/igdeConfigurationLocal.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/logger/igdeLoggerHistory.h>
#include <deigde/gui/igdeMainWindow.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeProgressBar.h>
#include <deigde/gui/igdeStatusBar.h>
#include <deigde/gui/igdeTimer.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/logger/igdeWindowLogger.h>
#include <deigde/gui/menu/igdeMenuBar.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/theme/igdeGuiTheme.h>
#include <deigde/resourceloader/igdeResourceLoader.h>

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/skin/deSkin.h>


class igdeEditorModuleManager;
class igdeEnvironmentIGDE;
class igdeGDPreviewManager;
#include <deigde/gameproject/igdeGameProject.h>
class igdeLoadSaveSystem;
class igdeTaskSyncGameDefinition;
class igdeTemplateList;
class igdeTexturePropertyList;
class igdeSharedFontList;
class igdeUIHelper;

class deException;
class decTimer;
#include <deigde/gamedefinition/igdeGameDefinition.h>
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
	igdeWindowLogger::Ref pWindowLogger;
	deVirtualFileSystem::Ref pVFS;
	igdeTemplateList *pTemplates;
	igdeGameDefinitionList *pSharedGameDefinitions;
	igdeGameDefinition::Ref pIGDEGameDefinition;
	igdeGameProject::Ref pGameProject;
	igdeGuiTheme::Ref pDefaultGuiTheme;
	decObjectDictionary pGuiThemes;
	igdeSharedFontList *pSharedFontList;
	deRig::Ref pSharedModelCollisionRig;
	igdeResourceLoader *pResourceLoader;
	igdeUIHelper *pUIHelper;
	igdeUIHelper *pUIHelperProperties;
	igdeTimer::Ref pTimerFrameUpdate;
	igdeTimer::Ref pTimerSyncProject;
	
	
	igdeIcon::Ref pIconApplication;
	
	igdeIcon::Ref pIconGameNew;
	igdeIcon::Ref pIconGameOpen;
	igdeIcon::Ref pIconGameSave;
	igdeIcon::Ref pIconGameSaveAs;
	igdeIcon::Ref pIconGameExit;
	igdeIcon::Ref pIconGameReloadXMLElementClasses;
	igdeIcon::Ref pIconSettingsIGDE;
	igdeIcon::Ref pIconSettingsEngine;
	igdeIcon::Ref pIconSettingsTexPropList;
	
	static const int pStockImageCount = igdeEnvironment::esiConfig + 1;
	igdeIcon::Ref pStockIcons[pStockImageCount];
	
	static const int pStockSkinCount = igdeEnvironment::essEditRimOutline + 1;
	deSkin::Ref pStockSkins[pStockSkinCount];
	
	static const int pStockRigCount = igdeEnvironment::esrGizmoMove + 1;
	deRig::Ref pStockRigs[pStockRigCount];
	
	static const int pStockModelCount = igdeEnvironment::esmGizmoMove + 1;
	deModel::Ref pStockModels[pStockModelCount];
	
	igdeAction::Ref pActionGameNew;
	igdeAction::Ref pActionGameOpen;
	igdeAction::Ref pActionGameSave;
	igdeAction::Ref pActionGameSaveAs;
	igdeAction::Ref pActionGameSettings;
	igdeAction::Ref pActionGameQuit;
	igdeAction::Ref pActionGameReloadXMLElementClasses;
	igdeAction::Ref pActionSettingsIgde;
	igdeAction::Ref pActionSettingsEngine;
	igdeAction::Ref pActionSettingsModules;
	igdeAction::Ref pActionSettingsTexPropList;
	igdeAction::Ref pActionSettingsLogging;
	
	igdeMenuCascade::Ref pMenuGame;
	igdeMenuCascade::Ref pMenuRecentProjects;
	igdeMenuCascade::Ref pMenuSettings;
	igdeMenuCascade::Ref pMenuWindow;
	
	igdeMenuBar::Ref pMenuBar;
	
	igdeToolBarDock::Ref pToolBarDockLeft;
	igdeToolBarDock::Ref pToolBarDockTop;
	igdeToolBarDock::Ref pToolBarDockRight;
	igdeToolBarDock::Ref pToolBarDockBottom;
	igdeStatusBar::Ref pStatusBar;
	igdeProgressBar::Ref pSBProgress;
	igdeSwitcher::Ref pSwiContent;
	
	igdeToolBar::Ref pTBGame;
	igdeContainer::Ref pFraEditors;
	
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
	igdeWindowMain(igdeEnvironmentIGDE &environment);
	
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
	inline const igdeLoggerHistory::Ref &GetLoggerHistory() const{ return pLoggerHistory; }
	
	/** Virtual file system. */
	inline const deVirtualFileSystem::Ref &GetVirtualFileSystem() const{ return pVFS; }
	
	/** Project templates. */
	inline igdeTemplateList &GetTemplates() const{ return *pTemplates; }
	
	/** Shared game definitions. */
	inline igdeGameDefinitionList &GetSharedGameDefinitions() const{ return *pSharedGameDefinitions; }
	
	/** Retrieves the igde game definition. */
	inline const igdeGameDefinition::Ref &GetIGDEGameDefinition() const{ return pIGDEGameDefinition; }
	
	/** Logger window or NULL if not visible. */
	inline const igdeWindowLogger::Ref &GetWindowLogger() const{ return pWindowLogger; }
	
	/** Named GuiTheme or default if not found. */
	igdeGuiTheme *GetGuiThemeNamed(const char *name);
	
	/** Default GuiTheme. */
	inline const igdeGuiTheme::Ref &GetDefaultGuiTheme() const{ return pDefaultGuiTheme; }
	
	/** Shared font list. */
	inline igdeSharedFontList &GetSharedFontList() const{ return *pSharedFontList; }
	
	/** Shared model collision rig. */
	inline const deRig::Ref &GetSharedModelCollisionRig() const{ return pSharedModelCollisionRig; }
	
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
	bool ProcessCommandLine(const decUnicodeStringList &arguments);
	
	/** Retrieves the active game project. */
	inline const igdeGameProject::Ref &GetGameProject() const{ return pGameProject; }
	/** Sets the active game project. */
	void SetGameProject(igdeGameProject *project);
	/**
	 * Creates a new game project.
	 * \returns True of the project has been created or false otherwise.
	 */
	bool CreateNewGameProject();
	/** Creates a placeholder game project. */
	void CreatePlaceholderGameProject();
	/** Load game project. */
	bool LoadGameProject(const char *filename);
	/** Save game project. */
	void SaveGameProject(const char *filename);
	/** Add entry to recently loaded file list and update the menus. */
	void AddRecentGameProject(const char *filename);
	
	/** Create a new game definition. */
	igdeGameDefinition *CreateNewGameDefinition();
	
	/** Displays an exception error. */
	void DisplayException(const deException &exception);
	
	/** Active module shared menus changed. */
	void ActiveModuleSharedMenusChanged();
	
	/** Active module shared toolbars changed. */
	void ActiveModuleSharedToolBarsChanged();
	
	/** Activate editor and bring it to the front. */
	void ActivateEditor(igdeEditorModule *editor);
	
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
	inline const igdeMenuBar::Ref &GetMenuBar() const{ return pMenuBar; }
	
	/** Toolbar dock sites. */
	inline const igdeToolBarDock::Ref &GetDockSiteLeft() const{ return pToolBarDockLeft; }
	inline const igdeToolBarDock::Ref &GetDockSiteTop() const{ return pToolBarDockTop; }
	inline const igdeToolBarDock::Ref &GetDockSiteRight() const{ return pToolBarDockRight; }
	inline const igdeToolBarDock::Ref &GetDockSiteBottom() const{ return pToolBarDockBottom; }
	
	/** Content switcher. */
	inline const igdeSwitcher::Ref &GetContentSwitcher() const{ return pSwiContent; }
	
	/** Game toolbar. */
	inline const igdeToolBar::Ref &GetToolBarGame() const{ return pTBGame; }
	
	
	
	/** Icons. */
	inline const igdeIcon::Ref &GetIconApplication() const{ return pIconApplication; }
	
	inline const igdeIcon::Ref &GetIconGameNew() const{ return pIconGameNew; }
	inline const igdeIcon::Ref &GetIconGameOpen() const{ return pIconGameOpen; }
	inline const igdeIcon::Ref &GetIconGameSave() const{ return pIconGameSave; }
	inline const igdeIcon::Ref &GetIconGameSaveAs() const{ return pIconGameSaveAs; }
	inline const igdeIcon::Ref &GetIconGameExit() const{ return pIconGameExit; }
	inline const igdeIcon::Ref &GetIconGameReloadXMLElementClasses() const{ return pIconGameReloadXMLElementClasses; }
	inline const igdeIcon::Ref &GetIconSettingsIgde() const{ return pIconSettingsIGDE; }
	inline const igdeIcon::Ref &GetIconSettingsEngine() const{ return pIconSettingsEngine; }
	inline const igdeIcon::Ref &GetIconSettingsTexPropList() const{ return pIconSettingsTexPropList; }
	
	
	
	/** Reload XML Element Classes. */
	void ReloadXMLElementClasses();
	
	/** Re-find and add skins. */
	void ReFindAndAddSkins();
	
	/** Re-find and add skies. */
	void ReFindAndAddSkies();
	
	
	
	/** Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible(bool visible);
	/** Sets the progress bar progress. */
	void SetProgress(float progress);
	/** Sets the progress text. */
	void SetProgressText(const char *text);
	
	/** Retrieves the minimum frame update time. */
	inline float GetMinUpdateTime() const{ return pMinUpdateTime; }

	/** Sets the minimum frame update time. */
	void SetMinUpdateTime(float seconds);
	
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
	bool RequestSaveDocuments(const char *title, const char* message);
	/*@}*/
	
	/** \name Stock resources */
	/*@{*/
	igdeIcon *GetStockIcon(igdeEnvironment::eStockIcons icon) const;
	const deSkin::Ref &GetStockSkin(igdeEnvironment::eStockSkins skin) const;
	const deRig::Ref &GetStockRig(igdeEnvironment::eStockRigs rig) const;
	const deModel::Ref &GetStockModel(igdeEnvironment::eStockModels model) const;
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
	void pLoadStockModels();
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
	void pCreateMenuGame(igdeMenuCascade &menu);
	void pCreateMenuSettings(igdeMenuCascade &menu);
	void pUpdateMenuRecentProjects(igdeMenuCascade &menu);
	void pUpdateMenuWindow(igdeMenuCascade &menu);
	
	void pUpdatePauseUpdating();
	
	void pLoadXMLElementClasses(igdeGameProject &gameProject);
	void pFindAndAddSkins(igdeGameProject &gameProject);
	void pFindAndAddSkies(igdeGameProject &gameProject);
};

#endif
