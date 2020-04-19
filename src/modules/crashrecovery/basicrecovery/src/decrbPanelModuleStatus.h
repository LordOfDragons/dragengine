/* 
 * Drag[en]gine Basic Crash Recovery Module
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

// include only once
#ifndef _DECRBPANELMODULESTATUS_H_
#define _DECRBPANELMODULESTATUS_H_

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "foxtoolkit.h"

// predefinitions
class decrbWindowMain;



/**
 * Panel showing the status of a module in the game engine.
 */
class decrbPanelModuleStatus : public FXVerticalFrame{
	FXDECLARE( decrbPanelModuleStatus )
	
protected:
	decrbPanelModuleStatus();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXTopWindow::ID_LAST,
		ID_CBMODULE,
		ID_LAST
	};
	
private:
	decrbWindowMain *pWndMain;
	
	FXComboBox *pCBModule;
	FXTextField *pEditType;
	FXText *pEditDescription;
	FXTextField *pEditAuthor;
	FXTextField *pEditVersion;
	FXTextField *pEditPattern;
	FXTextField *pEditDefaultExtension;
	FXTextField *pEditDirName;
	FXCheckButton *pChkFallback;
	FXTextField *pEditStatus;
	
	FXTextField *pEditLibName;
	FXTextField *pEditLibSize;
	FXTextField *pEditLibHash;
	
public:
	// constructor, destructor
	decrbPanelModuleStatus( decrbWindowMain *windowMain, FXComposite *container );
	virtual ~decrbPanelModuleStatus();
	
	// management
	void UpdateModuleStatus();
	void UpdateModulesList();
	
	// events
	long onCBModuleChanged( FXObject *sender, FXSelector selector, void *data );
	
private:
};

// end of include only once
#endif
