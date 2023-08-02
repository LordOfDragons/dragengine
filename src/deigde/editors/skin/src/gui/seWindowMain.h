/* 
 * Drag[en]gine IGDE Skin Editor
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

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeTabBookReference.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeIconReference.h>

class seConfiguration;
class seEngineController;
class seIGDEModule;
class seLoadSaveSystem;
class seSkin;
class seTexture;
class seMapped;
class seViewConstructed;
class seViewSkin;
class seWindowMainListener;
class seWindowProperties;
class igdeStepableTask;

class decStringList;



/**
 * \brief Main Application Window.
 */
class seWindowMain : public igdeEditorWindow{
private:
	seWindowMainListener *pListener;
	
	igdeActionReference pActionFileNew;
	igdeActionReference pActionFileNewModel;
	igdeActionReference pActionFileOpen;
	igdeActionReference pActionFileSave;
	igdeActionReference pActionFileSaveAs;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	
	igdeActionReference pActionMappedAdd;
	igdeActionReference pActionMappedRemove;
	
	igdeActionReference pActionTextureAdd;
	igdeActionReference pActionTextureRemove;
	igdeActionReference pActionTextureImportFromGDef;
	igdeActionReference pActionTextureImportFromFile;
	
	igdeActionReference pActionPropertyAdd;
	igdeActionReference pActionPropertyRemove;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	seConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	seLoadSaveSystem *pLoadSaveSystem;
	
	seWindowProperties *pWindowProperties;
	
	igdeTabBookReference pSwitcherViews;
	seViewSkin *pViewSkin;
	seViewConstructed *pViewConstructed;
	
	seSkin *pSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window. */
	seWindowMain( seIGDEModule &module );
	
protected:
	/** \brief Clean up main window. */
	virtual ~seWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reset views. */
	void ResetViews();
	
	/** \brief Configuration. */
	inline seConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const{ return pClipboard; }
	
	/** \brief Load save system. */
	inline seLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** \brief Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set Skin. */
	void SetSkin( seSkin *Skin );
	
	/** \brief Create new skin. */
	void CreateNewSkin();
	
	/** \brief Load skin. */
	void LoadSkin( const char *filename );
	
	/** \brief Save skin. */
	void SaveSkin( const char *filename );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionMappedAdd() const{ return pActionMappedAdd; }
	inline igdeAction *GetActionMappedRemove() const{ return pActionMappedRemove; }
	
	inline igdeAction *GetActionTextureAdd() const{ return pActionTextureAdd; }
	inline igdeAction *GetActionTextureRemove() const{ return pActionTextureRemove; }
	inline igdeAction *GetActionTextureImportFromGDef() const{ return pActionTextureImportFromGDef; }
	inline igdeAction *GetActionTextureImportFromFile() const{ return pActionTextureImportFromFile; }
	
	inline igdeAction *GetActionTexturePropertyAdd() const{ return pActionPropertyAdd; }
	inline igdeAction *GetActionTexturePropertyRemove() const{ return pActionPropertyRemove; }
	
	
	
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
	void pCreateMenuSkin( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
	void pCreateMenuMapped( igdeMenuCascade &menu );
	void pCreateMenuTexture( igdeMenuCascade &menu );
};

#endif
