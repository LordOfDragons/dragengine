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

class deglWindowMain;
class deglGameProfile;
class deglGame;
class deglDGPPanelSystem;
class deglDGPPanelFileFormats;



/**
 * @brief Game Problems Dialog.
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
	deglGameProfile *pProfileOrg;
	deglGameProfile *pProfileWork;
	deglGame *pGame;
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	deglDialogGameProblems( deglWindowMain *windowMain, deglGame *game, deglGameProfile *profile, FXWindow *owner );
	/** Cleans up the dialog. */
	virtual ~deglDialogGameProblems();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	/** Retrieves the game. */
	inline deglGame *GetGame() const{ return pGame; }
	/** Retrieves the original game profile. */
	inline deglGameProfile *GetOriginalProfile() const{ return pProfileOrg; }
	/** Retrieves the working game profile. */
	inline deglGameProfile *GetWorkingProfile() const{ return pProfileWork; }
	/** Determines if the game is working. */
	inline bool GetStatusWorking() const{ return pStatusWorking; }
	
	/** Retrieves the background color for valid elements. */
	inline FXColor GetBackColorValid() const{ return pClrValidBack; }
	/** Retrieves the background color for problem elements. */
	inline FXColor GetBackColorProblem() const{ return pClrProblemBack; }
	/** Retrieves the text color for valid elements. */
	inline FXColor GetTextColorValid() const{ return pClrValidText; }
	/** Retrieves the text color for problem elements. */
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
	
	/** @name Events */
	/*@{*/
	/*@}*/
	
private:
	void pCreateTabGame();
};

// end of include only once
#endif
