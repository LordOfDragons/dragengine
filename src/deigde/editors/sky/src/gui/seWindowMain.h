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

#ifndef _SEWINDOWMAIN_H_
#define _SEWINDOWMAIN_H_

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>

class decStringList;
class seWindowMainListener;
class seViewSky;
class seConfiguration;
class seSky;
class seWindowProperties;
class seWindowCurves;
class seEngineController;
class seLoadSaveSystem;
class seIGDEModule;
class igdeStepableTask;



/**
 * \brief Main Application Window.
 */
class seWindowMain : public igdeEditorWindow{
private:
	seWindowMainListener::Ref pListener;
	
	//igdeIcon::Ref pIconSkyNew;
	
	igdeAction::Ref pActionSkyNew;
	igdeAction::Ref pActionSkyOpen;
	igdeAction::Ref pActionSkySave;
	igdeAction::Ref pActionSkySaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	
	igdeAction::Ref pActionViewShowCompass;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	seConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	seLoadSaveSystem *pLoadSaveSystem;
	
	seViewSky::Ref pViewSky;
	seWindowProperties::Ref pWindowProperties;
	seWindowCurves::Ref pWindowCurves;
	
	seSky::Ref pSky;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new main window. */
	seWindowMain(igdeEditorModule &module);
	
protected:
	/** \brief Clean up the main window. */
	virtual ~seWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Icons. */
	//inline igdeIcon* GetIconSkyNew() const{ return pIconSkyNew; }
	
	/** \brief Actions. */
	inline igdeAction *GetActionSkyNew() const{ return pActionSkyNew; }
	inline igdeAction *GetActionSkyOpen() const{ return pActionSkyOpen; }
	inline igdeAction *GetActionSkySave() const{ return pActionSkySave; }
	inline igdeAction *GetActionSkySaveAs() const{ return pActionSkySaveAs; }
	
	inline igdeActionUndo *GetActionEditUndo() const{ return pActionEditUndo; }
	inline igdeActionRedo *GetActionEditRedo() const{ return pActionEditRedo; }
	inline igdeAction *GetActionEditCut() const{ return pActionEditCut; }
	inline igdeAction *GetActionEditCopy() const{ return pActionEditCopy; }
	inline igdeAction *GetActionEditPaste() const{ return pActionEditPaste; }
	
	inline igdeAction *GetActionViewShowCompass() const{ return pActionViewShowCompass; }
	
	/** \brief Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reset views. */
	void ResetViews();
	
	/** \brief configuration. */
	inline seConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	
	/** \brief load save system. */
	inline seLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** Properties window. */
	inline seWindowProperties &GetWindowProperties() const{ return *pWindowProperties; }
	
	
	
	/** \brief Sky. */
	inline const seSky::Ref &GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky(seSky *Sky);
	
	/** \brief Create sky. */
	void CreateNewSky();
	
	/** \brief Save Sky under the given file. */
	void SaveSky(const char *filename);
	
	
	
	/** \brief Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** \brief Module has been activated. */
	virtual void OnActivate();
	
	/** \brief Module has been deactivated. */
	virtual void OnDeactivate();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/**
	 * \brief Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments(decStringList &list);
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument(const char *filename);
	
	/**
	 * \brief Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument(const char *filename);
	
	/**
	 * \brief Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/**
	 * \brief The game project has changed.
	 * 
	 * Notification send to the editor modules after a new game project has been set.
	 * The editor module has to discard all open documents and all references held of
	 * objects related to the previous game project. The old game project is kept
	 * alive until all editor modules have processed the OnGameProjectChanged().
	 */
	virtual void OnGameProjectChanged();
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * Called after an editor changed the game definition. The old game definition used so
	 * far is replaced by a new game definition. The module has to update everything
	 * using the old game definition. This process can be potentially lengthy. For this
	 * reason the module has to return a steppable task to do the processing. If the module
	 * does not need any update \em NULL can be returned. The caller delets the task once
	 * finished processing.
	 * 
	 * The default implementation returns \em NULL.
	 */
	virtual igdeStepableTask *OnGameDefinitionChanged();
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuSky(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuView(igdeMenuCascade &menu);
};

#endif
