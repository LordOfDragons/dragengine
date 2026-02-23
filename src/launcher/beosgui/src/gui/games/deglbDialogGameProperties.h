/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEGLB_DIALOGGAMEPROPERTIES_H_
#define _DEGLB_DIALOGGAMEPROPERTIES_H_

#include <Window.h>
#include <Button.h>
#include <TextControl.h>
#include <TextView.h>
#include <ScrollView.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <ListView.h>
#include <StringView.h>
#include <OS.h>

#include <delauncher/game/delGame.h>

class deglbWindowMain;


/**
 * \brief Game Properties Dialog.
 */
class deglbDialogGameProperties : public BWindow{
public:
	enum eMessages{
		MSG_OK = 'gpok',
		MSG_CANCEL = 'gpca',
		MSG_PROFILE_CHANGED = 'gppc',
		MSG_EDIT_PROFILES = 'gpep',
		MSG_DROP_CUSTOM_PROFILE = 'gpdc',
		MSG_PATCH_CHANGED = 'gppx',
		MSG_SCRMODINFO = 'gpsm'
	};
	
	
private:
	deglbWindowMain *pWindowMain;
	delGame::Ref pGame;
	
	// Info tab
	BTextControl *pEditIdentifier;
	BTextControl *pEditAliasIdentifier;
	BTextControl *pEditTitle;
	BTextView *pTextDescription;
	BTextControl *pEditCreator;
	BTextControl *pEditHomepage;
	BTextControl *pEditGameDir;
	BTextControl *pEditDelgaFile;
	BTextControl *pEditDataDir;
	BTextControl *pEditScriptDir;
	BTextControl *pEditScriptModule;
	BTextControl *pEditScriptModuleVersion;
	
	// Settings tab
	BMenuField *pMenuProfile;
	BPopUpMenu *pPopupProfile;
	BStringView *pLabProfileProblems;
	BTextControl *pEditRunArgs;
	BMenuField *pMenuPatch;
	BPopUpMenu *pPopupPatch;
	
	// File Formats tab
	BListView *pListFileFormats;
	
	sem_id pSem;
	bool pResult;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	deglbDialogGameProperties(deglbWindowMain *windowMain, delGame *game);
	
	/** \brief Clean up dialog. */
	~deglbDialogGameProperties() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Game. */
	inline delGame &GetGame() const{ return *pGame; }
	
	/** \brief Run dialog, returns true if user confirmed changes. */
	bool Go();
	
	/** \brief Update game values. */
	void UpdateGame();
	
	/** \brief Update file format list. */
	void UpdateFileFormatList();
	
	/** \brief Update profile list. */
	void UpdateProfileList();
	
	/** \brief Update patch list. */
	void UpdatePatchList();
	/*@}*/
	
	
	
	/** \name BWindow */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	bool QuitRequested() override;
	/*@}*/
};

#endif
