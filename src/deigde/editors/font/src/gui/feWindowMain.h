/* 
 * Drag[en]gine IGDE Font Editor
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

#ifndef _FEWINDOWMAIN_H_
#define _FEWINDOWMAIN_H_

#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeIconReference.h>

class feWindowMainListener;
class feConfiguration;
class feViewFontImage;
class feFont;
class feClipboard;
class feWindowProperties;
class feLoadSaveSystem;



/**
 * \brief Editor window.
 */
class feWindowMain : public igdeEditorWindow{
private:
	feWindowMainListener *pListener;
	
	//igdeIconReference pIconEditPaste;
	
	igdeActionReference pActionFontNew;
	igdeActionReference pActionFontOpen;
	igdeActionReference pActionFontSave;
	igdeActionReference pActionFontSaveAs;
	igdeActionReference pActionFontGenerate;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	feConfiguration *pConfiguration;
	feClipboard *pClipboard;
	feLoadSaveSystem *pLoadSaveSystem;
	
	feViewFontImage *pViewFontImage;
	feWindowProperties *pWndProps;
	
	feFont *pFont;
	
	igdeFont::sConfiguration pGenFontConfig;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create editor window. */
	feWindowMain( igdeEditorModule &module );
	
protected:
	/** \brief Clean up editor window. */
	virtual ~feWindowMain();
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Icons. */
	//inline igdeIcon* GetIconFontNew() const{ return pIconFontNew; }
	
	/** \brief Actions. */
	inline igdeAction *GetActionFontNew() const{ return pActionFontNew; }
	inline igdeAction *GetActionFontOpen() const{ return pActionFontOpen; }
	inline igdeAction *GetActionFontSave() const{ return pActionFontSave; }
	inline igdeAction *GetActionFontSaveAs() const{ return pActionFontSaveAs; }
	inline igdeAction *GetActionFontGenerate() const{ return pActionFontGenerate; }
	
	inline igdeActionUndo *GetActionEditUndo() const{ return pActionEditUndo; }
	inline igdeActionRedo *GetActionEditRedo() const{ return pActionEditRedo; }
	inline igdeAction *GetActionEditCut() const{ return pActionEditCut; }
	inline igdeAction *GetActionEditCopy() const{ return pActionEditCopy; }
	inline igdeAction *GetActionEditPaste() const{ return pActionEditPaste; }
	
	/** Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	/** Resets the views. */
	void ResetViews();
	
	/** Retrieves the configuration. */
	inline feConfiguration &GetConfiguration() const{ return *pConfiguration; }
	/** Retrieves the clipboard. */
	inline feClipboard &GetClipboard() const{ return *pClipboard; }
	/** Retrieves the load save system. */
	inline feLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** \breif Font generation font configuration. */
	inline const igdeFont::sConfiguration &GetGenFontConfig() const{ return pGenFontConfig; }
	
	/** \brief Set font generation font configuration. */
	void SetGenFontConfig( const igdeFont::sConfiguration &config );
	
	/** Retrieves the rig. */
	inline feFont *GetFont() const{ return pFont; }
	/** Sets the font. */
	void SetFont( feFont *font );
	/** Creates a new font. */
	void CreateNewFont();
	/** Saves the font under the given file. */
	void SaveFont( const char *filename );
	
	/** \brief Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** \brief Module is has been activated. */
	virtual void OnActivate();
	
	/** \brief Module has been deactivated. */
	virtual void OnDeactivate();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief List of unsaved open documents. */
	virtual void GetChangedDocuments( decStringList &list );
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument( const char *filename );
	
	/** \brief Request document to be saved. */
	virtual bool SaveDocument( const char *filename );
	
	/**
	 * \brief Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/** \brief Game project has changed. */
	virtual void OnGameProjectChanged();
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuFont( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
};

#endif
