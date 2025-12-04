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

#ifndef _LPEWINDOWMAIN_H_
#define _LPEWINDOWMAIN_H_

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeIcon.h>

class lpeLangPackEntry;
class lpeWindowMainListener;
class lpeWindowProperties;
class lpeViewLangPack;
class lpeConfiguration;
class lpeLangPack;
class lpeLoadSaveSystem;
class lpeIGDEModule;



/**
 * Main window.
 */
class lpeWindowMain : public igdeEditorWindow{
private:
	lpeWindowMainListener *pListener;
	
	//igdeIcon::Ref pIconLangPackNew;
	
	igdeAction::Ref pActionLangPackNew;
	igdeAction::Ref pActionLangPackOpen;
	igdeAction::Ref pActionLangPackSave;
	igdeAction::Ref pActionLangPackSaveAs;
	igdeAction::Ref pActionLangPackOpenRef;
	igdeAction::Ref pActionEntryAdd;
	igdeAction::Ref pActionEntryRemove;
	igdeAction::Ref pActionEntryNextMissing;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	
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
	/** Create window. */
	lpeWindowMain( igdeEditorModule &module );
	
protected:
	/** Clean up window. */
	virtual ~lpeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Icons. */
	//inline igdeIcon* GetIconLangPackNew() const{ return pIconLangPackNew; }
	
	/** Actions. */
	inline igdeAction *GetActionLangPackNew() const{ return pActionLangPackNew; }
	inline igdeAction *GetActionLangPackOpen() const{ return pActionLangPackOpen; }
	inline igdeAction *GetActionLangPackSave() const{ return pActionLangPackSave; }
	inline igdeAction *GetActionLangPackSaveAs() const{ return pActionLangPackSaveAs; }
	inline igdeAction *GetActionLangPackOpenRef() const{ return pActionLangPackOpenRef; }
	inline igdeAction *GetActionEntryAdd() const{ return pActionEntryAdd; }
	inline igdeAction *GetActionEntryRemove() const{ return pActionEntryRemove; }
	inline igdeAction *GetActionEntryNextMissing() const{ return pActionEntryNextMissing; }
	
	inline igdeActionUndo *GetActionEditUndo() const{ return pActionEditUndo; }
	inline igdeActionRedo *GetActionEditRedo() const{ return pActionEditRedo; }
	inline igdeAction *GetActionEditCut() const{ return pActionEditCut; }
	inline igdeAction *GetActionEditCopy() const{ return pActionEditCopy; }
	inline igdeAction *GetActionEditPaste() const{ return pActionEditPaste; }
	
	/** Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** Configuration. */
	inline lpeConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	
	/** Load save system. */
	inline lpeLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** Language pack. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	
	/** Set language pack. */
	void SetLangPack( lpeLangPack *langpack );
	
	/** Create new language pack. */
	void CreateNewLangPack();
	
	/** Save language pack under the given file. */
	void SaveLangPack( const char *filename );
	
	/** Get reference language pack. */
	lpeLangPack *GetReferenceLangPack() const;
	
	/** Set reference language pack. */
	void SetReferenceLangPack( lpeLangPack *langpack );
	
	/** Select next missing language pack entry. */
	void SelectNextMissingEntry();
	
	/** List of unsaved open documents. */
	void GetChangedDocuments( decStringList &list );
	
	/**
	 * Requests a document to be loaded.
	 */
	virtual void LoadDocument( const char *filename );
	
	/** Request document to be saved. */
	bool SaveDocument( const char *filename );
	
	/**
	 * Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/** Game project changed. */
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
