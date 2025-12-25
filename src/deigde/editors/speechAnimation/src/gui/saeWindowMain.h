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

#ifndef _SAEWINDOWMAIN_H_
#define _SAEWINDOWMAIN_H_

#include "saeWindowMainListener.h"
#include "saeViewSAnimation.h"
#include "properties/saeWindowProperties.h"
#include "../sanimation/saeSAnimation.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/string/decStringList.h>

class saeConfiguration;
class saeLoadSaveSystem;
class saeIGDEModule;
class igdeStepableTask;



/**
 * Main Editor Window.
 */
class saeWindowMain : public igdeEditorWindow{
public:
	typedef deTObjectReference<saeWindowMain> Ref;
	
	
private:
	saeWindowMainListener::Ref pListener;
	
	igdeAction::Ref pActionFileNew;
	igdeAction::Ref pActionFileOpen;
	igdeAction::Ref pActionFileSave;
	igdeAction::Ref pActionFileSaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	
	igdeAction::Ref pActionViewDispModeViseme;
	igdeAction::Ref pActionViewDispModePhoneme;
	igdeAction::Ref pActionViewDispModeWord;
	
	igdeAction::Ref pActionVisemeAdd;
	igdeAction::Ref pActionVisemeRemove;
	
	igdeAction::Ref pActionPhonemeAdd;
	igdeAction::Ref pActionPhonemeRemove;
	
	igdeAction::Ref pActionWordAdd;
	igdeAction::Ref pActionWordRemove;
	igdeAction::Ref pActionWordAddList;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	saeConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	saeLoadSaveSystem *pLoadSaveSystem;
	
	saeViewSAnimation::Ref pViewSAnimation;
	saeWindowProperties::Ref pWindowProperties;
	
	saeSAnimation::Ref pSAnimation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create window. */
	saeWindowMain(saeIGDEModule &module);
	
protected:
	/** Cleanup window. */
	virtual ~saeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** Reset views. */
	void ResetViews();
	
	
	
	/** Configuration. */
	inline saeConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	
	/** Load save system. */
	inline saeLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** Properties window. */
	inline saeWindowProperties &GetWindowProperties() const{ return *pWindowProperties; }
	
	
	
	/** Speech animation. */
	inline const saeSAnimation::Ref &GetSAnimation() const{ return pSAnimation; }
	
	/** Set speech animation. */
	void SetSAnimation(saeSAnimation *sanimation);
	
	/** Create new speech animation. */
	void CreateNewSAnimation();
	
	/** Save speech animation to file. */
	void SaveSAnimation(const char *filename);
	
	
	
	/** Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** Module has been activated. */
	virtual void OnActivate();
	
	/** Module has been deactivated. */
	virtual void OnDeactivate();
	
	/** Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/**
	 * Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments(decStringList &list);
	
	/**
	 * Requests a document to be loaded.
	 */
	virtual void LoadDocument(const char *filename);
	
	/**
	 * Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument(const char *filename);
	
	/**
	 * Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/**
	 * The game project has changed.
	 * 
	 * Notification send to the editor modules after a new game project has been set.
	 * The editor module has to discard all open documents and all references held of
	 * objects related to the previous game project. The old game project is kept
	 * alive until all editor modules have processed the OnGameProjectChanged().
	 */
	virtual void OnGameProjectChanged();
	
	/**
	 * Project game definition changed.
	 * 
	 * Called after an editor changed the game definition. The old game definition used so
	 * far is replaced by a new game definition. The module has to update everything
	 * using the old game definition. This process can be potentially lengthy. For this
	 * reason the module has to return a steppable task to do the processing. If the module
	 * does not need any update \em nullptr can be returned. The caller delets the task once
	 * finished processing.
	 * 
	 * The default implementation returns \em nullptr.
	 */
	virtual igdeStepableTask::Ref OnGameDefinitionChanged();
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuFile(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuView(igdeMenuCascade &menu);
	void pCreateMenuPhoneme(igdeMenuCascade &menu);
	void pCreateMenuWord(igdeMenuCascade &menu);
};

#endif
