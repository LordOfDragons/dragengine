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

#include "../configuration/seConfiguration.h"
#include "../loadsave/seLoadSaveSystem.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>


#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>

class seIGDEModule;
class seSource;
class seEffect;
class seSynthesizer;
class seViewSynthesizer;
class seWindowMainListener;



/**
 * \brief Main window.
 */
class seWindowMain : public igdeEditorWindow{
private:
	seWindowMainListener::Ref pListener;
	
	igdeIcon::Ref pIconSourceSound;
	igdeIcon::Ref pIconSourceWave;
	igdeIcon::Ref pIconSourceChain;
	igdeIcon::Ref pIconSourceGroup;
	igdeIcon::Ref pIconSourceSynthesizer;
	
	igdeIcon::Ref pIconEffectStretch;
	
	igdeIcon::Ref pIconPlay;
	igdeIcon::Ref pIconPause;
	igdeIcon::Ref pIconStop;
	
	igdeAction::Ref pActionFileNew;
	igdeAction::Ref pActionFileOpen;
	igdeAction::Ref pActionFileSave;
	igdeAction::Ref pActionFileSaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	
	igdeAction::Ref pActionControllerAdd;
	igdeAction::Ref pActionControllerRemove;
	igdeAction::Ref pActionControllerUp;
	igdeAction::Ref pActionControllerDown;
	
	igdeAction::Ref pActionSourceAddSound;
	igdeAction::Ref pActionSourceAddWave;
	igdeAction::Ref pActionSourceAddChain;
	igdeAction::Ref pActionSourceAddGroup;
	igdeAction::Ref pActionSourceAddSynthesizer;
	
	igdeAction::Ref pActionSourceInsertSound;
	igdeAction::Ref pActionSourceInsertWave;
	igdeAction::Ref pActionSourceInsertChain;
	igdeAction::Ref pActionSourceInsertGroup;
	igdeAction::Ref pActionSourceInsertSynthesizer;
	
	igdeAction::Ref pActionSourceGroupAddSound;
	igdeAction::Ref pActionSourceGroupAddWave;
	igdeAction::Ref pActionSourceGroupAddChain;
	igdeAction::Ref pActionSourceGroupAddGroup;
	igdeAction::Ref pActionSourceGroupAddSynthesizer;
	
	igdeAction::Ref pActionSourceRemove;
	igdeAction::Ref pActionSourceUp;
	igdeAction::Ref pActionSourceDown;
	
	igdeAction::Ref pActionEffectAddStretch;
	
	igdeAction::Ref pActionEffectInsertStretch;
	
	igdeAction::Ref pActionEffectRemove;
	igdeAction::Ref pActionEffectUp;
	igdeAction::Ref pActionEffectDown;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	igdeClipboard pClipboard;
	seConfiguration pConfiguration;
	seLoadSaveSystem pLoadSaveSystem;
	
	seViewSynthesizer *pViewSynthesizer;
	
	seSynthesizer::Ref pSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	seWindowMain(seIGDEModule &module);
	
protected:
	/** \brief Clean up window. */
	virtual ~seWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reset views. */
	void ResetViews();
	
	
	
	/** \brief Configuration. */
	inline seConfiguration &GetConfiguration(){ return pConfiguration; }
	inline const seConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** \brief Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const{ return pClipboard; }
	
	/** \brief Load save system. */
	inline seLoadSaveSystem &GetLoadSaveSystem(){ return pLoadSaveSystem; }
	inline const seLoadSaveSystem &GetLoadSaveSystem() const{ return pLoadSaveSystem; }
	
	
	
	/** \brief Synthesizer. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer. */
	void SetSynthesizer(seSynthesizer::Ref synthesizer);
	
	
	
	/** \brief Create synthesizer. */
	void CreateSynthesizer();
	
	/** \brief Save synthesizer. */
	void SaveSynthesizer(const char *filename);
	
	
	
	/** \brief Create a new source of a given type using the matching undo action. */
	void CreateSource(const decString &name, deSynthesizerSourceVisitorIdentify::eSourceTypes type,
		bool insert, bool group);
	
	/** \brief Create a new effect of a given type using the matching undo action. */
	void CreateEffect(deSynthesizerEffectVisitorIdentify::eEffectTypes type, bool insert);
	
	
	
	/** \brief Set visibility of the progress bar in the status bar. */
	void SetProgressVisible(bool visible);
	
	/** \brief Set progress bar progress. */
	void SetProgress(float progress);
	
	/** \brief Set progress text. */
	void SetProgressText(const char *text);
	
	
	
	/** \brief Icons. */
	inline igdeIcon *GetIconSourceSound() const{ return pIconSourceSound; }
	inline igdeIcon *GetIconSourceWave() const{ return pIconSourceWave; }
	inline igdeIcon *GetIconSourceChain() const{ return pIconSourceChain; }
	inline igdeIcon *GetIconSourceGroup() const{ return pIconSourceGroup; }
	inline igdeIcon *GetIconSourceSynthesizer() const{ return pIconSourceSynthesizer; }
	igdeIcon *GetSourceIcon(deSynthesizerSourceVisitorIdentify::eSourceTypes type) const;
	
	inline igdeIcon *GetIconEffectStretch() const{ return pIconEffectStretch; }
	igdeIcon *GetEffectIcon(deSynthesizerEffectVisitorIdentify::eEffectTypes type) const;
	
	inline igdeIcon *GetIconPlay() const{ return pIconPlay; }
	inline igdeIcon *GetIconPause() const{ return pIconPause; }
	inline igdeIcon *GetIconStop() const{ return pIconStop; }
	
	
	
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
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionControllerAdd() const{ return pActionControllerAdd; }
	inline igdeAction *GetActionControllerRemove() const{ return pActionControllerRemove; }
	inline igdeAction *GetActionControllerUp() const{ return pActionControllerUp; }
	inline igdeAction *GetActionControllerDown() const{ return pActionControllerDown; }
	
	inline igdeAction *GetActionSourceAddSound() const{ return pActionSourceAddSound; }
	inline igdeAction *GetActionSourceAddWave() const{ return pActionSourceAddWave; }
	inline igdeAction *GetActionSourceAddChain() const{ return pActionSourceAddChain; }
	inline igdeAction *GetActionSourceAddGroup() const{ return pActionSourceAddGroup; }
	inline igdeAction *GetActionSourceAddSynthesizer() const{ return pActionSourceAddSynthesizer; }
	
	inline igdeAction *GetActionSourceGroupAddSound() const{ return pActionSourceGroupAddSound; }
	inline igdeAction *GetActionSourceGroupAddWave() const{ return pActionSourceGroupAddWave; }
	inline igdeAction *GetActionSourceGroupAddChain() const{ return pActionSourceGroupAddChain; }
	inline igdeAction *GetActionSourceGroupAddGroup() const{ return pActionSourceGroupAddGroup; }
	inline igdeAction *GetActionSourceGroupAddSynthesizer() const{ return pActionSourceGroupAddSynthesizer; }
	
	inline igdeAction *GetActionSourceInsertSound() const{ return pActionSourceInsertSound; }
	inline igdeAction *GetActionSourceInsertWave() const{ return pActionSourceInsertWave; }
	inline igdeAction *GetActionSourceInsertChain() const{ return pActionSourceInsertChain; }
	inline igdeAction *GetActionSourceInsertGroup() const{ return pActionSourceInsertGroup; }
	inline igdeAction *GetActionSourceInsertSynthesizer() const{ return pActionSourceInsertSynthesizer; }
	
	inline igdeAction *GetActionSourceRemove() const{ return pActionSourceRemove; }
	inline igdeAction *GetActionSourceUp() const{ return pActionSourceUp; }
	inline igdeAction *GetActionSourceDown() const{ return pActionSourceDown; }
	
	inline igdeAction *GetActionEffectAddStretch() const{ return pActionEffectAddStretch; }
	
	inline igdeAction *GetActionEffectInsertStretch() const{ return pActionEffectInsertStretch; }
	
	inline igdeAction *GetActionEffectRemove() const{ return pActionEffectRemove; }
	inline igdeAction *GetActionEffectUp() const{ return pActionEffectUp; }
	inline igdeAction *GetActionEffectDown() const{ return pActionEffectDown; }
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuSynthesizer(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuController(igdeMenuCascade &menu);
	void pCreateMenuSource(igdeMenuCascade &menu);
	void pCreateMenuEffect(igdeMenuCascade &menu);
};

#endif
