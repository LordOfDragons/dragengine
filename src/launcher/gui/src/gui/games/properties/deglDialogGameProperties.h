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

#ifndef _DEGLDIALOGGAMEPROPERTIES_H_
#define _DEGLDIALOGGAMEPROPERTIES_H_

#include "../../foxtoolkit.h"
#include "../../deglSharedIcon.h"

#include <delauncher/game/delGame.h>

class deglWindowMain;
class delGameProfileList;
class deglCalculateDirectorySize;


/**
 * Game properties panel.
 */
class deglDialogGameProperties : public FXDialogBox{
	FXDECLARE( deglDialogGameProperties )
protected:
	deglDialogGameProperties();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		
		ID_TAB_PANELS,
		
		ID_BTN_SCRMODINFO,
		
		ID_CB_PROFILE,
		ID_BTN_EDIT_PROFILES,
		ID_BTN_DROP_CUSTOM_PROFILE,
		ID_CB_PATCHES,
		ID_BTN_PATCHES,
		ID_PU_PATCH_UNINSTALL,
		
		ID_LIST_FILEFORMATS,
		
		ID_LIST_CACHES,
		
		ID_TIMER_UPDATE_CALCSIZE,
		
		ID_LAST
	};
	
	
	
private:
	struct sCache{
		deglCalculateDirectorySize *calcSize;
		FXString name;
		uint64_t used;
		uint64_t limit;
		float fillLevel;
	};
	
	deglWindowMain *pWindowMain;
	delGame::Ref pGame;
	deglSharedIcon::Ref pIconBig;
	
	FXTabBook *pTabPanels;
	
	FXTabItem *pTabInfos;
	FXTextField *pEditIdentifier;
	FXTextField *pEditAliasIdentifier;
	FXTextField *pEditTitle;
	FXText *pTextDescription;
	FXLabel *pLabIcon;
	FXTextField *pEditCreator;
	FXTextField *pEditHomepage;
	FXTextField *pEditGameDir;
	FXLabel *pLabStatusGameDir;
	FXLabel *pLabProblemGameDir;
	FXTextField *pEditDelgaFile;
	FXLabel *pLabStatusDelgaFile;
	FXLabel *pLabProblemDelgaFile;
	FXTextField *pEditDataDir;
	FXLabel *pLabStatusDataDir;
	FXLabel *pLabProblemDataDir;
	FXTextField *pEditScriptDir;
	FXLabel *pLabStatusScriptDir;
	FXLabel *pLabProblemScriptDir;
	FXTextField *pEditScriptModule;
	FXTextField *pEditScriptModuleVersion;
	FXLabel *pLabStatusScriptModule;
	FXLabel *pLabProblemScriptModule;
	FXButton *pBtnScriptModuleInfo;
	
	FXTabItem *pTabSettings;
	
	FXLabel *pLabProfileIcon;
	FXComboBox *pCBProfile;
	FXButton *pBtnEditProfiles;
	FXButton *pBtnDropCustomProfile;
	FXLabel *pLabProfileProblems;
	
	FXTextField *pEditRunArgs;
	FXComboBox *pCBPatches;
	FXButton *pBtnPatches;
	
	FXTabItem *pTabFileFormats;
	FXIconList *pListFileFormats;
	
	FXTabItem *pTabDiscUsage;
	FXTextField *pEditSizeDelgaFile;
	FXTextField *pEditSizeDataDir;
	FXTextField *pEditSizeCaptureDir;
	FXTextField *pEditSizeConfigDir;
	FXTextField *pEditSizeCaches;
	FXIconList *pListCaches;
	sCache *pCaches;
	int pCacheCount;
	
	deglCalculateDirectorySize *pCalcSizeDataDir;
	deglCalculateDirectorySize *pCalcSizeCaptureDir;
	deglCalculateDirectorySize *pCalcSizeConfigDir;
	bool pCalcSizePending;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	deglDialogGameProperties( deglWindowMain *windowMain, delGame *game, FXWindow *owner );
	
	/** Clean up dialog. */
	virtual ~deglDialogGameProperties();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Game. */
	inline delGame &GetGame() const{ return pGame; }
	
	/** Update game values. */
	void UpdateGame();
	
	/** Update file format list. */
	void UpdateFileFormatList();
	
	/** Update disc usage. */
	void UpdateDiscUsage();
	
	/** Update cache list. */
	void UpdateCacheList();
	
	/** Switch to game properties. */
	void SwitchToProperties();
	
	/** Switch to file formats. */
	void SwitchToFileFormats();
	
	/** Switch to caches. */
	void SwitchToDiscUsage();
	
	/** Run modal invocation of the dialog. */
	virtual FXuint execute( FXuint placement = PLACEMENT_OWNER );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onTabPanelsChanged( FXObject *sender, FXSelector selector, void *data );
	
	long onBtnScriptModuleInfo( FXObject *sender, FXSelector selector, void *data );
	
	long onCBProfileChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnEditProfiles( FXObject *sender, FXSelector selector, void *data );
	long onBtnDropCustomProfile( FXObject *sender, FXSelector selector, void *data );
	long onCBPatchesChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnPatches( FXObject *sender, FXSelector selector, void *data );
	long onPUPatchUninstall( FXObject *sender, FXSelector selector, void *data );
	
	long onTimerUpdateCalcSize( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
	
	
	
private:
	void pDeleteCaches();
	
	FXString FormatSize1024( uint64_t size ) const;
	FXString FormatSize1000( uint64_t size ) const;
	
	static FXint fSortCache( const FXIconItem *item1, const FXIconItem *item2 );
	
	void pLogException( const char *source, const decStringList &exception );
};

#endif
