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
#ifndef _DECRBPANELPARAMETERS_H_
#define _DECRBPANELPARAMETERS_H_

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "foxtoolkit.h"

// predefinitions
class decrbWindowMain;
class deModuleParameter;



/**
 * Panel showing the parameters of modules allowing the user to
 * also change them.
 */
class decrbPanelParameters : public FXVerticalFrame{
	FXDECLARE( decrbPanelParameters )
	
protected:
	decrbPanelParameters();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXTopWindow::ID_LAST,
		ID_CBMODULE,
		ID_CBPARAMETER,
		ID_BTNSET,
		ID_BTNRESET,
		ID_LAST
	};
	
private:
	decrbWindowMain *pWndMain;
	deModuleParameter *pParameterInfo;
	
	FXComboBox *pCBModule;
	FXComboBox *pCBParameter;
	FXText *pEditDescription;
	FXTextField *pEditType;
	FXText *pEditAllowedValues;
	FXTextField *pEditValue;
	
	FXButton *pBtnSet;
	FXButton *pBtnReset;
	
public:
	// constructor, destructor
	decrbPanelParameters( decrbWindowMain *windowMain, FXComposite *container );
	virtual ~decrbPanelParameters();
	
	// management
	void UpdateParameter();
	void UpdateParametersList();
	void UpdateModulesList();
	
	// events
	long onCBModuleChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBParameterChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnSetCommand( FXObject *sender, FXSelector selector, void *data );
	long onBtnResetCommand( FXObject *sender, FXSelector selector, void *data );
	
private:
};

// end of include only once
#endif
