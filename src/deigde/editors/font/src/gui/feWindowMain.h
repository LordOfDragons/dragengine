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

#include "feWindowMainListener.h"
#include "fontimage/feViewFontImage.h"
#include "properties/feWindowProperties.h"
#include "../font/feFont.h"

#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeIcon.h>

class feConfiguration;
class feClipboard;
class feLoadSaveSystem;



/**
 * \brief Editor window.
 */
class feWindowMain : public igdeEditorWindow{
public:
	typedef deTObjectReference<feWindowMain> Ref;
	
	
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
	~feWindowMain() override;
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Icons. */
	//inline igdeIcon* GetIconFontNew() const{ return pIconFontNew; }
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionFontNew() const{ return pActionFontNew; }
	inline const igdeAction::Ref &GetActionFontOpen() const{ return pActionFontOpen; }
	inline const igdeAction::Ref &GetActionFontSave() const{ return pActionFontSave; }
	inline const igdeAction::Ref &GetActionFontSaveAs() const{ return pActionFontSaveAs; }
	inline const igdeAction::Ref &GetActionFontGenerate() const{ return pActionFontGenerate; }
	
	inline const igdeActionUndo::Ref &GetActionEditUndo() const{ return pActionEditUndo; }
	inline const igdeActionRedo::Ref &GetActionEditRedo() const{ return pActionEditRedo; }
	inline const igdeAction::Ref &GetActionEditCut() const{ return pActionEditCut; }
	inline const igdeAction::Ref &GetActionEditCopy() const{ return pActionEditCopy; }
	inline const igdeAction::Ref &GetActionEditPaste() const{ return pActionEditPaste; }
	
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
	void OnBeforeEngineStart() override;
	
	/** \brief Game engine has been started. */
	void OnAfterEngineStart() override;
	
	/** \brief Game engine is about to be stopped. */
	void OnBeforeEngineStop() override;
	
	/** \brief Game engine has been stopped. */
	void OnAfterEngineStop() override;
	
	/** \brief Module is has been activated. */
	void OnActivate() override;
	
	/** \brief Module has been deactivated. */
	void OnDeactivate() override;
	
	/** \brief Game like frame update. */
	void OnFrameUpdate(float elapsed) override;
	
	/** \brief List of unsaved open documents. */
	void GetChangedDocuments(decStringList &list) override;
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	void LoadDocument(const char *filename) override;
	
	/** \brief Request document to be saved. */
	bool SaveDocument(const char *filename) override;
	
	/**
	 * \brief Recent files changed.
	 */
	void RecentFilesChanged() override;
	
	/** \brief Game project has changed. */
	void OnGameProjectChanged() override;
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
