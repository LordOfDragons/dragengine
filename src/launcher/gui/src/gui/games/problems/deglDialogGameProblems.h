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
	FXDECLARE(deglDialogGameProblems)
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
	deglDialogGameProblems(deglWindowMain *windowMain,
		delGame *game, delGameProfile *profile, FXWindow *owner);
	
	/** Clean up dialog. */
	virtual ~deglDialogGameProblems();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{return pWindowMain;}
	
	/** Game. */
	inline delGame *GetGame() const{return pGame;}
	
	/** Original game profile. */
	inline delGameProfile *GetOriginalProfile() const{return pProfileOrg;}
	
	/** Working game profile. */
	inline delGameProfile *GetWorkingProfile() const{return pProfileWork;}
	
	/** Game is working. */
	inline bool GetStatusWorking() const{return pStatusWorking;}
	
	/** Background color for valid elements. */
	inline FXColor GetBackColorValid() const{return pClrValidBack;}
	
	/** Background color for problem elements. */
	inline FXColor GetBackColorProblem() const{return pClrProblemBack;}
	
	/** Text color for valid elements. */
	inline FXColor GetTextColorValid() const{return pClrValidText;}
	
	/** Text color for problem elements. */
	inline FXColor GetTextColorProblem() const{return pClrProblemText;}
	
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
