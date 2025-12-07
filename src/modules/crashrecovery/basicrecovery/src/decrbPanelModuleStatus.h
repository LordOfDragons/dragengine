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
	FXDECLARE(decrbPanelModuleStatus)
	
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
	decrbPanelModuleStatus(decrbWindowMain *windowMain, FXComposite *container);
	virtual ~decrbPanelModuleStatus();
	
	// management
	void UpdateModuleStatus();
	void UpdateModulesList();
	
	// events
	long onCBModuleChanged(FXObject *sender, FXSelector selector, void *data);
	
private:
};

// end of include only once
#endif
