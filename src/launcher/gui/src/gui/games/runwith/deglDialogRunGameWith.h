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

#ifndef _DEGLDIALOGRUNGAMEWITH_H_
#define _DEGLDIALOGRUNGAMEWITH_H_

#include "../../foxtoolkit.h"

#include <delauncher/game/delGame.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/string/decString.h>

class deglWindowMain;



/**
 * Run Game With Panel.
 */
class deglDialogRunGameWith : public FXDialogBox{
	FXDECLARE( deglDialogRunGameWith )
protected:
	deglDialogRunGameWith();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		
		ID_CB_PROFILE,
		ID_BTN_EDIT_PROFILES,
		
		ID_LAST
	};
	
private:
	deglWindowMain *pWindowMain;
	const delGame::Ref pGame;
	delGameProfile::Ref pProfile;
	
	FXLabel *pLabIcon;
	FXComboBox *pCBProfile;
	FXButton *pBtnEdit;
	FXLabel *pLabProblem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	deglDialogRunGameWith( deglWindowMain *windowMain, delGame *game, FXWindow *owner );
	
	/** Clean up dialog. */
	virtual ~deglDialogRunGameWith();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Game. */
	inline delGame *GetModule() const{ return pGame; }
	
	/** Update game values. */
	void UpdateGame();
	
	/** Profile. */
	inline delGameProfile *GetProfile() const{ return pProfile; }
	
	/** Set profile. */
	void SetProfile( delGameProfile *profile );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCBProfileChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnEditProfiles( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
