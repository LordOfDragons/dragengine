/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLDIALOGGAMEPROPERTIES_H_
#define _DEGLDIALOGGAMEPROPERTIES_H_

#include "../../foxtoolkit.h"

#include <dragengine/deObjectReference.h>


class deglWindowMain;
class deglGame;
class deglGameProfileList;
class deglGameProfile;
class deglCalculateDirectorySize;



/**
 * \brief Game properties panel.
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
	deObjectReference pGame;
	deObjectReference pIconBig;
	
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
	/** Creates a new dialog. */
	deglDialogGameProperties( deglWindowMain *windowMain, deglGame *game, FXWindow *owner );
	/** Cleans up the dialog. */
	virtual ~deglDialogGameProperties();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	/** Retrieves the game. */
	inline deglGame &GetGame() const{ return ( deglGame& )( deObject& )pGame; }
	
	/** Update game values. */
	void UpdateGame();
	
	/** \brief Update file format list. */
	void UpdateFileFormatList();
	
	/** \brief Update disc usage. */
	void UpdateDiscUsage();
	
	/** \brief Update cache list. */
	void UpdateCacheList();
	
	/** \brief Switch to game properties. */
	void SwitchToProperties();
	
	/** \brief Switch to file formats. */
	void SwitchToFileFormats();
	
	/** \brief Switch to caches. */
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
};

#endif
