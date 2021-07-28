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

#ifndef _DEGLDIALOGENGINEPROPS_H_
#define _DEGLDIALOGENGINEPROPS_H_

#include "../foxtoolkit.h"

class deglWindowMain;



/**
 * Engine Properties Panel.
 */
class deglDialogEngineProps : public FXDialogBox{
	FXDECLARE( deglDialogEngineProps )
protected:
	deglDialogEngineProps();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		ID_CB_ACTIVE_PROFILE,
		ID_BTN_EDIT_PROFILES,
		ID_LAST
	};
	
private:
	deglWindowMain *pWindowMain;
	
	FXTabBook *pTabPanels;
	
	FXTextField *pEditPathConfig;
	FXTextField *pEditPathShare;
	FXTextField *pEditPathLib;
	
	FXComboBox *pCBActiveProfile;
	FXButton *pBtnEditProfiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	deglDialogEngineProps( deglWindowMain *windowMain, FXWindow *owner );
	
	/** Clean up dialog. */
	virtual ~deglDialogEngineProps();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Set values from engine. */
	void SetFromEngine();
	
	/** Update profile list. */
	void UpdateProfileList();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCBProfilesChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnEditProfiles( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
