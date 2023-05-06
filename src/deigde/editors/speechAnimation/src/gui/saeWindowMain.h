/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAEWINDOWMAIN_H_
#define _SAEWINDOWMAIN_H_

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeIconReference.h>

class decStringList;
class saeWindowMainListener;
class saeViewSAnimation;
class saeConfiguration;
class saeSAnimation;
class saeWindowProperties;
class saeLoadSaveSystem;
class saeIGDEModule;
class igdeStepableTask;



/**
 * Main Editor Window.
 */
class saeWindowMain : public igdeEditorWindow{
private:
	saeWindowMainListener *pListener;
	
	igdeActionReference pActionFileNew;
	igdeActionReference pActionFileOpen;
	igdeActionReference pActionFileSave;
	igdeActionReference pActionFileSaveAs;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	
	igdeActionReference pActionViewDispModeViseme;
	igdeActionReference pActionViewDispModePhoneme;
	igdeActionReference pActionViewDispModeWord;
	
	igdeActionReference pActionVisemeAdd;
	igdeActionReference pActionVisemeRemove;
	
	igdeActionReference pActionPhonemeAdd;
	igdeActionReference pActionPhonemeRemove;
	
	igdeActionReference pActionWordAdd;
	igdeActionReference pActionWordRemove;
	igdeActionReference pActionWordAddList;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	saeConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	saeLoadSaveSystem *pLoadSaveSystem;
	
	saeViewSAnimation *pViewSAnimation;
	saeWindowProperties *pWindowProperties;
	
	saeSAnimation *pSAnimation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create window. */
	saeWindowMain( saeIGDEModule &module );
	
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
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	
	/** Set speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** Create new speech animation. */
	void CreateNewSAnimation();
	
	/** Save speech animation to file. */
	void SaveSAnimation( const char *filename );
	
	
	
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
	virtual void OnFrameUpdate( float elapsed );
	
	/**
	 * Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments( decStringList &list );
	
	/**
	 * Requests a document to be loaded.
	 */
	virtual void LoadDocument( const char *filename );
	
	/**
	 * Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument( const char *filename );
	
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
	void pCreateMenuFile( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
	void pCreateMenuView( igdeMenuCascade &menu );
	void pCreateMenuViseme( igdeMenuCascade &menu );
	void pCreateMenuPhoneme( igdeMenuCascade &menu );
	void pCreateMenuWord( igdeMenuCascade &menu );
};

#endif
