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

#include <dragengine/deObjectReference.h>
#include <dragengine/common/string/decString.h>

class deglWindowMain;
class deglGame;
class deglGameProfileList;
class deglGameProfile;



/**
 * \brief Run Game With Panel.
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
	deglGame *pGame;
	
	deObjectReference pProfile;
	
	FXLabel *pLabIcon;
	FXComboBox *pCBProfile;
	FXButton *pBtnEdit;
	FXLabel *pLabProblem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	deglDialogRunGameWith( deglWindowMain *windowMain, deglGame *game, FXWindow *owner );
	
	/** \brief Clean up dialog. */
	virtual ~deglDialogRunGameWith();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Game. */
	inline deglGame *GetModule() const{ return pGame; }
	
	/** \brief Update game values. */
	void UpdateGame();
	
	/** \brief Profile. */
	inline deglGameProfile *GetProfile() const{ return ( deglGameProfile* )( deObject* )pProfile; }
	
	/** \brief Set profile. */
	void SetProfile( deglGameProfile *profile );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCBProfileChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnEditProfiles( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
