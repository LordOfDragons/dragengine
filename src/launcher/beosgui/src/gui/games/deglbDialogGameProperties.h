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
#include <ColumnListView.h>
#include <ColumnTypes.h>

#include "../deglbIconView.h"
#include "../deglbIconTabView.h"

#include <delauncher/game/delGame.h>
#include <delauncher/game/fileformat/delFileFormat.h>

class deglbWindowMain;


/**
 * \brief Game Properties Dialog.
 */
class deglbDialogGameProperties : public BWindow{
public:
	/** \brief File format list item. */
	class cFileFormatListItem : public BRow{
	public:
		const delFileFormat &fileFormat;
		BBitmap *icon;
		
		cFileFormatListItem(const delFileFormat &fileFormat, deglbWindowMain &windowMain);
		~cFileFormatListItem() override;
	};
	
	enum eMessages{
		MSG_OK = 'gpok',
		MSG_CANCEL = 'gpca',
		MSG_PROFILE_CHANGED = 'gppc',
		MSG_EDIT_PROFILES = 'gpep',
		MSG_EDIT_PROFILES_DONE = 'gpeP',
		MSG_DROP_CUSTOM_PROFILE = 'gpdc',
		MSG_PATCH_CHANGED = 'gppx',
		MSG_SCRMODINFO = 'gpsi'
	};
	
	
private:
	deglbWindowMain *pWindowMain;
	BMessenger pResultTarget;
	int pResultMessage;
	bool pResultValue;
	
	delGame::Ref pGame;
	
	// Info tab
	deglbIconTabView *pTabInfo;
	
	BTextView *pEditIdentifier;
	BTextView *pEditAliasIdentifier;
	BTextView *pEditTitle;
	BTextView *pTextDescription;
	deglbIconView *pIconGame;
	BTextView *pEditCreator;
	BTextView *pEditHomepage;
	
	BTextView *pEditGameDir;
	deglbIconView *pIconGameDir;
	BTextView *pEditGameDirProblem;
	
	BTextView *pEditDelgaFile;
	deglbIconView *pIconDelgaFile;
	BTextView *pEditDelgaFileProblem;
	
	BTextView *pEditDataDir;
	deglbIconView *pIconDataDir;
	BTextView *pEditDataDirProblem;
	
	BTextView *pEditScriptDir;
	deglbIconView *pIconScriptDir;
	BTextView *pEditScriptDirProblem;
	
	BTextView *pEditScriptModule;
	deglbIconView *pIconScriptModule;
	BTextView *pEditScriptModuleProblem;
	BTextView *pEditScriptModuleVersion;
	
	// Settings tab
	deglbIconTabView *pTabSettings;
	
	BMenuField *pMenuProfile;
	deglbIconView *pIconProfile;
	BPopUpMenu *pPopupProfile;
	BTextView *pLabProfileProblems;
	BTextControl *pEditRunArgs;
	BMenuField *pMenuPatch;
	BPopUpMenu *pPopupPatch;
	
	// File Formats tab
	deglbIconTabView *pTabFormats;
	
	BColumnListView *pListFileFormats;
	
	// Disc usage tab
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	deglbDialogGameProperties(deglbWindowMain *windowMain, delGame *game,
		const BMessenger &resultTarget, int resultMessage);
	
	/** \brief Clean up dialog. */
	~deglbDialogGameProperties() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Game. */
	inline delGame &GetGame() const{ return *pGame; }
	
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
