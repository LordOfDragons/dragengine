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

#ifndef _DEGLDIALOGRUNGAME_H_
#define _DEGLDIALOGRUNGAME_H_

#include "../../foxtoolkit.h"

#include <delauncher/game/delGame.h>
#include <delauncher/game/profile/delGameProfile.h>

class deglWindowMain;
class deglDGPPanelSystem;
class deglDGPPanelFileFormats;



/**
 * Game Problems Dialog.
 * Dialog showing problems preventing a game from being run. Provides also
 * support to solve the problems.
 */
class deglDialogGameProblems : public FXDialogBox{
	FXDECLARE( deglDialogGameProblems )
protected:
	deglDialogGameProblems();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		
		ID_LAST
	};
	
private:
	deglWindowMain *pWindowMain;
	const delGameProfile::Ref pProfileOrg;
	const delGameProfile::Ref pProfileWork;
	const delGame::Ref pGame;
	bool pStatusWorking;
	
	FXColor pClrValidBack;
	FXColor pClrValidText;
	FXColor pClrProblemBack;
	FXColor pClrProblemText;
	
	FXTabBook *pTabPanels;
	
	FXHorizontalFrame *pFraStatus;
	FXLabel *pLabStatus;
	
	FXTabItem *pTabSystems;
	deglDGPPanelSystem *pPanelSystems;
	
	FXTabItem *pTabFileFormats;
	deglDGPPanelFileFormats *pPanelFileFormats;
	
	FXTabItem *pTabGame;
	
	FXButton *pBtnRunGame;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	deglDialogGameProblems( deglWindowMain *windowMain,
		delGame *game, delGameProfile *profile, FXWindow *owner );
	
	/** Clean up dialog. */
	virtual ~deglDialogGameProblems();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Game. */
	inline delGame *GetGame() const{ return pGame; }
	
	/** Original game profile. */
	inline delGameProfile *GetOriginalProfile() const{ return pProfileOrg; }
	
	/** Working game profile. */
	inline delGameProfile *GetWorkingProfile() const{ return pProfileWork; }
	
	/** Game is working. */
	inline bool GetStatusWorking() const{ return pStatusWorking; }
	
	/** Background color for valid elements. */
	inline FXColor GetBackColorValid() const{ return pClrValidBack; }
	
	/** Background color for problem elements. */
	inline FXColor GetBackColorProblem() const{ return pClrProblemBack; }
	
	/** Text color for valid elements. */
	inline FXColor GetTextColorValid() const{ return pClrValidText; }
	
	/** Text color for problem elements. */
	inline FXColor GetTextColorProblem() const{ return pClrProblemText; }
	
	/** Update modules. */
	void UpdateSystemModuleLists();
	
	/** Verify game and profile. */
	void VerifyGameAndProfile();
	
	/** Update panels. */
	void UpdatePanels();
	
	/** Update game panel. */
	void UpdatePanelGame();
	/*@}*/
	
	
	
private:
	void pCreateTabGame();
};

#endif
