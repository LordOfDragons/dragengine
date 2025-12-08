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

#ifndef _FEWINDOWMAIN_H_
#define _FEWINDOWMAIN_H_

#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeIcon.h>
#include "feFont.h"
#include "feWindowMainListener.h"
#include "feWindowProperties.h"
#include "feViewFontImage.h"

class feConfiguration;
class feClipboard;
class feLoadSaveSystem;



/**
 * \brief Editor window.
 */
class feWindowMain : public igdeEditorWindow{
private:
	feWindowMainListener::Ref pListener;
	
	//igdeIcon::Ref pIconEditPaste;
	
	igdeAction::Ref pActionFontNew;
	igdeAction::Ref pActionFontOpen;
	igdeAction::Ref pActionFontSave;
	igdeAction::Ref pActionFontSaveAs;
	igdeAction::Ref pActionFontGenerate;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	feConfiguration *pConfiguration;
	feClipboard *pClipboard;
	feLoadSaveSystem *pLoadSaveSystem;
	
	feViewFontImage::Ref pViewFontImage;
	feWindowProperties::Ref pWndProps;
	
	feFont::Ref pFont;
	
	igdeFont::sConfiguration pGenFontConfig;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create editor window. */
	feWindowMain(igdeEditorModule &module);
	
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
	void SetGenFontConfig(const igdeFont::sConfiguration &config);
	
	/** Retrieves the rig. */
	inline const feFont::Ref &GetFont() const{ return pFont; }
	/** Sets the font. */
	void SetFont(feFont *font);
	/** Creates a new font. */
	void CreateNewFont();
	/** Saves the font under the given file. */
	void SaveFont(const char *filename);
	
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
	virtual void OnFrameUpdate(float elapsed);
	
	/** \brief List of unsaved open documents. */
	virtual void GetChangedDocuments(decStringList &list);
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument(const char *filename);
	
	/** \brief Request document to be saved. */
	virtual bool SaveDocument(const char *filename);
	
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
	void pCreateMenuFont(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
};

#endif
