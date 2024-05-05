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
