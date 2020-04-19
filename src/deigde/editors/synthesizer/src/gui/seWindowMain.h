/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SEWINDOWMAIN_H_
#define _SEWINDOWMAIN_H_

#include "../configuration/seConfiguration.h"
#include "../loadsave/seLoadSaveSystem.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/igdeTabBookReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeIconReference.h>


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
	seWindowMainListener *pListener;
	
	igdeIconReference pIconSourceSound;
	igdeIconReference pIconSourceWave;
	igdeIconReference pIconSourceChain;
	igdeIconReference pIconSourceGroup;
	igdeIconReference pIconSourceSynthesizer;
	
	igdeIconReference pIconEffectStretch;
	
	igdeIconReference pIconPlay;
	igdeIconReference pIconPause;
	igdeIconReference pIconStop;
	
	igdeActionReference pActionFileNew;
	igdeActionReference pActionFileOpen;
	igdeActionReference pActionFileSave;
	igdeActionReference pActionFileSaveAs;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	
	igdeActionReference pActionControllerAdd;
	igdeActionReference pActionControllerRemove;
	igdeActionReference pActionControllerUp;
	igdeActionReference pActionControllerDown;
	
	igdeActionReference pActionSourceAddSound;
	igdeActionReference pActionSourceAddWave;
	igdeActionReference pActionSourceAddChain;
	igdeActionReference pActionSourceAddGroup;
	igdeActionReference pActionSourceAddSynthesizer;
	
	igdeActionReference pActionSourceInsertSound;
	igdeActionReference pActionSourceInsertWave;
	igdeActionReference pActionSourceInsertChain;
	igdeActionReference pActionSourceInsertGroup;
	igdeActionReference pActionSourceInsertSynthesizer;
	
	igdeActionReference pActionSourceGroupAddSound;
	igdeActionReference pActionSourceGroupAddWave;
	igdeActionReference pActionSourceGroupAddChain;
	igdeActionReference pActionSourceGroupAddGroup;
	igdeActionReference pActionSourceGroupAddSynthesizer;
	
	igdeActionReference pActionSourceRemove;
	igdeActionReference pActionSourceUp;
	igdeActionReference pActionSourceDown;
	
	igdeActionReference pActionEffectAddStretch;
	
	igdeActionReference pActionEffectInsertStretch;
	
	igdeActionReference pActionEffectRemove;
	igdeActionReference pActionEffectUp;
	igdeActionReference pActionEffectDown;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	igdeClipboard pClipboard;
	seConfiguration pConfiguration;
	seLoadSaveSystem pLoadSaveSystem;
	
	seViewSynthesizer *pViewSynthesizer;
	
	seSynthesizer *pSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	seWindowMain( seIGDEModule &module );
	
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
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	
	
	/** \brief Create synthesizer. */
	void CreateSynthesizer();
	
	/** \brief Save synthesizer. */
	void SaveSynthesizer( const char *filename );
	
	
	
	/** \brief Create a new source of a given type using the matching undo action. */
	void CreateSource( const decString &name, deSynthesizerSourceVisitorIdentify::eSourceTypes type,
		bool insert, bool group );
	
	/** \brief Create a new effect of a given type using the matching undo action. */
	void CreateEffect( deSynthesizerEffectVisitorIdentify::eEffectTypes type, bool insert );
	
	
	
	/** \brief Set visibility of the progress bar in the status bar. */
	void SetProgressVisible( bool visible );
	
	/** \brief Set progress bar progress. */
	void SetProgress( float progress );
	
	/** \brief Set progress text. */
	void SetProgressText( const char *text );
	
	
	
	/** \brief Icons. */
	inline igdeIcon *GetIconSourceSound() const{ return pIconSourceSound; }
	inline igdeIcon *GetIconSourceWave() const{ return pIconSourceWave; }
	inline igdeIcon *GetIconSourceChain() const{ return pIconSourceChain; }
	inline igdeIcon *GetIconSourceGroup() const{ return pIconSourceGroup; }
	inline igdeIcon *GetIconSourceSynthesizer() const{ return pIconSourceSynthesizer; }
	igdeIcon *GetSourceIcon( deSynthesizerSourceVisitorIdentify::eSourceTypes type ) const;
	
	inline igdeIcon *GetIconEffectStretch() const{ return pIconEffectStretch; }
	igdeIcon *GetEffectIcon( deSynthesizerEffectVisitorIdentify::eEffectTypes type ) const;
	
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
	virtual void OnFrameUpdate( float elapsed );
	
	/**
	 * \brief Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments( decStringList &list );
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument( const char *filename );
	
	/**
	 * \brief Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument( const char *filename );
	
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
	void pCreateMenuSynthesizer( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
	void pCreateMenuController( igdeMenuCascade &menu );
	void pCreateMenuSource( igdeMenuCascade &menu );
	void pCreateMenuEffect( igdeMenuCascade &menu );
};

#endif
