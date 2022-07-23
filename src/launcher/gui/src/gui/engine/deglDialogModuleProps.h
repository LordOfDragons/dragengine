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

#ifndef _DEGLDIALOGMODULEPROPS_H_
#define _DEGLDIALOGMODULEPROPS_H_

#include "../foxtoolkit.h"

#include <delauncher/engine/modules/delEngineModule.h>

class deglWindowMain;



/**
 * Module Properties Panel.
 */
class deglDialogModuleProps : public FXDialogBox{
	FXDECLARE( deglDialogModuleProps )
protected:
	deglDialogModuleProps();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		ID_BTN_INFO,
		ID_LAST
	};
	
private:
	deglWindowMain *pWindowMain;
	const delEngineModule::Ref pModule;
	
	FXTabBook *pTabPanels;
	
	FXTextField *pEditName;
	FXTextField *pEditType;
	FXTextField *pEditStatus;
	FXText *pTextDescription;
	FXTextField *pEditAuthor;
	FXTextField *pEditVersion;
	FXTextField *pEditPattern;
	FXCheckButton *pChkIsFallback;
	
	FXTextField *pEditErrorCode;
	FXButton *pBtnInfoErrorCode;
	FXTextField *pEditLibFileName;
	FXTextField *pEditLibFileSizeShould;
	FXTextField *pEditLibFileSizeIs;
	FXTextField *pEditLibFileHashShould;
	FXTextField *pEditLibFileHashIs;
	FXTextField *pEditLibFileEntryPoint;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	deglDialogModuleProps( deglWindowMain *windowMain, delEngineModule *module, FXWindow *owner );
	/** Cleans up the dialog. */
	virtual ~deglDialogModuleProps();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	/** Retrieves the module. */
	inline delEngineModule *GetModule() const{ return pModule; }
	
	/** Set values from moduel. */
	void SetFromModule();
	/*@}*/
	
	/** \name Events */
	/*@{*/
	long onBtnInfo( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

// end of include only once
#endif
