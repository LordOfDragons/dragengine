/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#ifndef _LPEWINDOWMAIN_H_
#define _LPEWINDOWMAIN_H_

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeIconReference.h>

class lpeLangPackEntry;
class lpeWindowMainListener;
class lpeWindowProperties;
class lpeViewLangPack;
class lpeConfiguration;
class lpeLangPack;
class lpeLoadSaveSystem;
class lpeIGDEModule;



/**
 * \brief Main window.
 */
class lpeWindowMain : public igdeEditorWindow{
private:
	lpeWindowMainListener *pListener;
	
	//igdeIconReference pIconLangPackNew;
	
	igdeActionReference pActionLangPackNew;
	igdeActionReference pActionLangPackOpen;
	igdeActionReference pActionLangPackSave;
	igdeActionReference pActionLangPackSaveAs;
	igdeActionReference pActionEntryAdd;
	igdeActionReference pActionEntryRemove;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	lpeConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	lpeLoadSaveSystem *pLoadSaveSystem;
	
	lpeWindowProperties *pWindowProperties;
	lpeViewLangPack *pViewLangPack;
	
	lpeLangPack *pLangPack;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	lpeWindowMain( igdeEditorModule &module );
	
protected:
	/** \brief Clean up window. */
	virtual ~lpeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Icons. */
	//inline igdeIcon* GetIconLangPackNew() const{ return pIconLangPackNew; }
	
	/** \brief Actions. */
	inline igdeAction *GetActionLangPackNew() const{ return pActionLangPackNew; }
	inline igdeAction *GetActionLangPackOpen() const{ return pActionLangPackOpen; }
	inline igdeAction *GetActionLangPackSave() const{ return pActionLangPackSave; }
	inline igdeAction *GetActionLangPackSaveAs() const{ return pActionLangPackSaveAs; }
	inline igdeAction *GetActionEntryAdd() const{ return pActionEntryAdd; }
	inline igdeAction *GetActionEntryRemove() const{ return pActionEntryRemove; }
	
	inline igdeActionUndo *GetActionEditUndo() const{ return pActionEditUndo; }
	inline igdeActionRedo *GetActionEditRedo() const{ return pActionEditRedo; }
	inline igdeAction *GetActionEditCut() const{ return pActionEditCut; }
	inline igdeAction *GetActionEditCopy() const{ return pActionEditCopy; }
	inline igdeAction *GetActionEditPaste() const{ return pActionEditPaste; }
	
	/** \brief Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Configuration. */
	inline lpeConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	
	/** \brief Load save system. */
	inline lpeLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** \brief Language pack. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	
	/** \brief Set language pack. */
	void SetLangPack( lpeLangPack *langpack );
	
	/** \brief Create new language pack. */
	void CreateNewLangPack();
	
	/** \brief Save language pack under the given file. */
	void SaveLangPack( const char *filename );
	
	/** \brief List of unsaved open documents. */
	void GetChangedDocuments( decStringList &list );
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument( const char *filename );
	
	/** \brief Request document to be saved. */
	bool SaveDocument( const char *filename );
	
	/**
	 * \brief Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/** \brief Game project changed. */
	void OnGameProjectChanged();
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuLangPack( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
	void pCreateMenuEntry( igdeMenuCascade &menu );
};

#endif
